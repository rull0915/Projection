//====================================================//
// ファイル名  : MaterialAsset.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/05
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Types/MaterialAsset.h"
#include "Assets/Managers/AssetManager.h"

#include "Renderer/CBufferSlot.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	MaterialAsset::MaterialAsset()
		: m_pixelShader{}
		, m_vertexShader{}
		, m_constantBuffers{}
		, m_isDirty{ true }
	{
		ADD_PROPERTY(m_vertexShader);
		ADD_PROPERTY(m_pixelShader);
	}

	ShaderParam* MaterialAsset::FindParam(ShaderType stage, const std::string& name, AssetManager& assetManager)
	{
		// 指定されたステージ
		switch (stage)
		{
			// 頂点シェーダ
		case REngine::ShaderType::Vertex:
			if (ShaderAsset* vs = assetManager.Get<ShaderAsset>(m_vertexShader)) { return vs->FindParam(name); }
			break;
			// ピクセルシェーダ
		case REngine::ShaderType::Pixel:
			if (ShaderAsset* ps = assetManager.Get<ShaderAsset>(m_pixelShader)) { return ps->FindParam(name); }
			break;
		default:
			break;
		}

		// 見つからなかった場合
		return nullptr;
	}

	// 定数バッファを更新する関数
	void MaterialAsset::UpdateConstantBuffers(ID3D11Device* device, ID3D11DeviceContext* context, AssetManager& assetManager)
	{
		// 1つのステージのバッファを更新するラムダ
		auto updateStage = [&](ShaderAsset* asset, ShaderType type)
			{
				// nullptrなら何もしない
				if (!asset) return;

				// 定数バッファをループ
				for (auto& cBuffer : asset->GetBuffers())
				{
					// マテリアル管轄のスロットでなければスキップ
					if (!IsMaterialManagedSlot(cBuffer.slot)) continue;

					// マップのキーを生成
					auto key = std::make_pair(type, cBuffer.slot);

					// まだ存在しなければ
					if (!m_constantBuffers.contains(key))
					{
						// 新たに生成
						D3D11_BUFFER_DESC bd{};
						ZeroMemory(&bd, sizeof(bd));	// 0で埋める
						bd.Usage = D3D11_USAGE_DYNAMIC;		// 頻繁に更新されるためDYNAMIC
						bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
						bd.ByteWidth = cBuffer.size;		// バッファ全体のサイズを渡す
						bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとしてバインド

						// 作成
						device->CreateBuffer(&bd, nullptr, m_constantBuffers[key].GetAddressOf());	// バッファのComPtrのアドレスを渡す
					}

					// バッファが変更されていなければ何もしない
					if (!m_isDirty) continue;

					// バッファの更新

					// バッファを格納する配列
					std::vector<uint8_t> buf(cBuffer.size, 0);	// 1byteで1つの領域とするため8bit整数型を使用

					// 全パラメータを調べる
					for (auto& [paramKey, value] : m_params)
					{
						// ステージが一致しなければ何もしない
						if (paramKey.stage != type) continue;

						// 対応するパラメータをShaderから取得
						const ShaderParam* p = asset->FindParam(paramKey.name);

						if (!p ||									// 取得できなかった場合	
							p->cbSlot != cBuffer.slot ||			// 違うスロットのパラメータだった場合	
							p->type == ShaderParamType::Texture2D	// 定数バッファでなかった場合
							) continue;	// 何もしない

						// visitでどの型でも処理する
						std::visit([&](auto&& v) {

							// decay_tを使用して参照を外した素の型を取得
							using V = std::decay_t<decltype(v)>;

							// Texture以外なら
							if constexpr (!std::is_same_v<V, Handle<Texture>>)
							{
								// データを配列内のメモリにコピーする
								// vのアドレスからサイズまでの領域を
								// バッファの先頭アドレスからオフセット分ずらしたメモリ領域にコピーします
								std::memcpy(buf.data() + p->offset, &v, p->size);
							}
						}, value);
					}

					// DYNAMICなのでmapで書き換える
					D3D11_MAPPED_SUBRESOURCE mapped;

					// CPUがアクセスできる領域を取得
					context->Map(
						m_constantBuffers[key].Get(), // 書き換え対象のバッファ
						0,	// SubResource番号 Bufferの場合は0
						D3D11_MAP_WRITE_DISCARD,	// 既存の内容を破棄し新しい領域を確保
						0, &mapped
					);

					// メモリを書き換える
					std::memcpy(mapped.pData, buf.data(), buf.size());

					// アクセス終了を通知
					context->Unmap(m_constantBuffers[key].Get(), 0);
				}
			};

		// 各ステージを更新
		updateStage(assetManager.Get<ShaderAsset>(m_pixelShader), ShaderType::Pixel);
		updateStage(assetManager.Get<ShaderAsset>(m_vertexShader), ShaderType::Vertex);

		// 変更済みフラグをリセット
		m_isDirty = false;
	}

	void MaterialAsset::Bind(ID3D11DeviceContext* context, AssetManager& assetManager)
	{
		// 頂点シェーダー本体を取得
		auto* vs = assetManager.Get<ShaderAsset>(m_vertexShader);

		// 頂点シェーダがなかった場合描画不可のためリターン
		if (!vs) return;
		if (vs) vs->Bind(context);

		// ピクセルシェーダ本体を取得
		auto* ps = assetManager.Get<ShaderAsset>(m_pixelShader);

		// シェーダー本体をバインド
		if (ps) ps->Bind(context);
		else context->PSSetShader(nullptr, nullptr, 0);	// なかった場合リセットする

		// Todo: 扱うステージが増えた場合同様の処理を他のシェーダーでも行ってください。

		// 全定数バッファを走査
		for (auto& [key, buf] : m_constantBuffers)
		{
			// ステージごとに処理
			switch (key.first)
			{
				// 頂点シェーダ
			case ShaderType::Vertex:
				if (vs) context->VSSetConstantBuffers(key.second, 1, buf.GetAddressOf());
				break;
			case ShaderType::Pixel:
				if (ps) context->PSSetConstantBuffers(key.second, 1, buf.GetAddressOf());
				break;
			default:
				break;
			}
		}

		// リソースのバインド
		for (auto& [key, value] : m_params)
		{
			// Handle<Texture>として取得
			auto* t = std::get_if<Handle<Texture>>(&value);

			// 取得できなかったら次へ
			if (!t) continue;

			// テクスチャを取得
			auto* tex = assetManager.Get<Texture>(*t);

			// 取得できなかったら次へ
			if (!tex) continue;

			// Todo シェーダー管理をmap<Type, Handle<Shander>>にしてタイプから引けるように

			// 取得できたら対応するステージを調べる
			switch (key.stage)
			{
				// PS
			case ShaderType::Pixel:
			{
				if (!ps) break;	// 対応するステージがなければ処理しない
				auto* p = ps->FindParam(key.name);
				if (p && p->type == ShaderParamType::Texture2D)
					context->PSSetShaderResources(p->tSlot, 1, tex->GetAddressOf());
				break;
			}
				// VS
			case ShaderType::Vertex:
			{
				if (!vs) break;	// 対応するステージがなければ処理しない
				auto* p = vs->FindParam(key.name);
				if (p && p->type == ShaderParamType::Texture2D)
					context->VSSetShaderResources(p->tSlot, 1, tex->GetAddressOf());
				break;
			}
				// 
			default:
				break;
			}
		}
	}
}

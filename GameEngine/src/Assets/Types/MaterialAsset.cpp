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
#include "Assets/Types/Shader/SamplerList.h"

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
		, m_assetManager{ nullptr }
		, m_samplerList{ nullptr }
		, m_needRebuildParams{ false }
	{
		ADD_PROPERTY(MaterialAsset, m_vertexShader);
		ADD_PROPERTY(MaterialAsset, m_pixelShader);
	}

	ShaderParam* MaterialAsset::FindParam(ShaderType stage, const std::string& name)
	{
		// アセットマネージャーが設定されていなければ更新不可
		if (!m_assetManager) return nullptr;

		ShaderAsset* shader = GetShaderAsset(stage);

		if (shader) return shader->FindParam(name);

		return nullptr;
	}

	// 定数バッファを更新する関数
	void MaterialAsset::UpdateConstantBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		// 再構築のチェック
		CheckAndDoRebuild();

		// 1つのステージのバッファを更新するラムダ
		auto updateStage = [&](ShaderAsset* asset, ShaderType type)
			{
				// nullptrなら何もしない
				if (!asset) return;

				// バッファをループ
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

					auto map = m_params[type];

					// 全パラメータを調べる
					for (auto& [name, value] : map)
					{
						// 対応するパラメータをShaderから取得
						const ShaderParam* p = asset->FindParam(name);

						if (!p ||									// 取得できなかった場合	
							p->slot != cBuffer.slot ||			// 違うスロットのパラメータだった場合	
							p->type == ShaderParamType::Texture2D	// 定数バッファでなかった場合
							) continue;	// 何もしない

						// visitでどの型でも処理する
						std::visit([&](auto&& v) {

							// decay_tを使用して参照を外した素の型を取得
							using V = std::decay_t<decltype(v)>;

							// 定数バッファなら
							if constexpr (!std::is_same_v<V, Handle<Texture>> && !std::is_same_v<V, SamplerType>)
							{
								// データを配列内のメモリにコピーする
								// vのアドレスからサイズまでの領域を
								// バッファの先頭アドレスからオフセット分ずらしたメモリ領域にコピーします
								std::memcpy(buf.data() + p->offset, &v, p->size);
							}
						}, value.value);
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

		// アセットマネージャーが設定されていなければ更新不可
		if (!m_assetManager) return;

		// 各ステージを更新
		updateStage(m_assetManager->Get<ShaderAsset>(m_pixelShader), ShaderType::Pixel);
		updateStage(m_assetManager->Get<ShaderAsset>(m_vertexShader), ShaderType::Vertex);

		// 変更済みフラグをリセット
		m_isDirty = false;
	}

	ID3DBlob* MaterialAsset::GetBlob(ShaderType type)
	{
		ShaderAsset* shader = GetShaderAsset(type);

		return shader ? shader->GetBlob() : nullptr;
	}

	void MaterialAsset::Apply(ID3D11DeviceContext* context)
	{
		// 参照が設定されていなければ解決不可
		if (!(m_assetManager && m_samplerList)) return;

		// 頂点シェーダー本体を取得
		auto* vs = m_assetManager->Get<ShaderAsset>(m_vertexShader);

		// vsがなければ対応不可
		if (!vs) return;

		// ピクセルシェーダ本体を取得
		auto* ps = m_assetManager->Get<ShaderAsset>(m_pixelShader);

		// シェーダー本体をバインド
		vs->Bind(context);

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
				context->VSSetConstantBuffers(key.second, 1, buf.GetAddressOf());
				break;
			case ShaderType::Pixel:
				if (ps) context->PSSetConstantBuffers(key.second, 1, buf.GetAddressOf());
				break;
			default:
				break;
			}
		}

		// リソースのバインド
		for (auto& [key, map] : m_params)
		{
			// 対応するシェーダーを取得
			auto* shader = [&]() -> ShaderAsset* {
				switch (key) 
				{
				case ShaderType::Vertex:  return vs;
				case ShaderType::Pixel:  return ps;
				default: return nullptr;
				}
			}();

			// なければ次へ
			if (!shader) continue;

			for (auto& [name, value] : map)
			{
				// Handle<Texture>として取得
				if (auto* t = std::get_if<Handle<Texture>>(&value.value))
				{
					// テクスチャを取得
					auto* tex = m_assetManager->Get<Texture>(*t);

					// バインド
					BindTexture(context, shader, tex, name, key);
				}
				// SamplerTypeとして取得
				else if (auto* s = std::get_if<SamplerType>(&value.value))
				{
					// サンプラーを取得
					auto& sampler = m_samplerList->GetSampler(*s);

					// バインド
					BindSampler(context, shader, sampler, name, key);
				}
			}
		}
	}

	std::vector<Property> MaterialAsset::GetProperties()
	{
		// 再構築のチェック
		CheckAndDoRebuild();

		// デフォルトのプロパティを取得
		std::vector<Property> properties = PropertyObject::GetProperties();

		// ヘッダー装飾を追加する関数
		auto addHeader = [&](const std::string& name)
			{
				Property prop{};
				prop.name = name;
				prop.type = PropertyType::Header;

				properties.push_back(prop);
			};

		// 各ステージの全パラメータを追加するラムダ式
		auto addParams = [&](ShaderType type)
			{
				// パラメータを取得
				if (!m_params.contains(type)) return;

				auto& params = m_params[type];

				for (auto& param : params)
				{
					properties.push_back(CreatePropertyFromParameter(param.first, param.second));
				}
			};

		// VS
		addHeader("VertexShader");
		addParams(ShaderType::Vertex);

		// PS
		addHeader("PixelShader");
		addParams(ShaderType::Pixel);

		return properties;
	}

	void MaterialAsset::BindTexture(ID3D11DeviceContext* context, ShaderAsset* shader, REngine::Texture* texture, const std::string& name, ShaderType type)
	{
		// テクスチャがなければ何もしない
		if (!texture) return;

		// パラメータを取得
		auto* param = shader->FindParam(name);

		// 対応していなければ何もしない
		if (!param || param->type != ShaderParamType::Texture2D) return;

		// 取得できたら対応するステージを調べる
		switch (type)
		{
			// PS
		case ShaderType::Pixel:
		{
			context->PSSetShaderResources(param->slot, 1, texture->GetAddressOf());
			break;
		}
			// VS
		case ShaderType::Vertex:
		{
			context->VSSetShaderResources(param->slot, 1, texture->GetAddressOf());
			break;
		} 
		default:
			break;
		}
	}

	void MaterialAsset::BindSampler(ID3D11DeviceContext* context, ShaderAsset* shader, const Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler, const std::string& name, ShaderType type)
	{
		// サンプラーがなければ何もしない
		if (!sampler) return;

		// パラメータを取得
		auto* param = shader->FindParam(name);

		// 対応していなければ何もしない
		if (!param || param->type != ShaderParamType::Sampler) return;

		// 取得できたら対応するステージを調べる
		switch (type)
		{
			// PS
		case ShaderType::Pixel:
		{
			context->PSSetSamplers(param->slot, 1, sampler.GetAddressOf());
			break;
		}
			// VS
		case ShaderType::Vertex:
		{
			context->VSSetSamplers(param->slot, 1, sampler.GetAddressOf());
			break;
		}
		default:
			break;
		}
	}

	void MaterialAsset::RebuildParams()
	{
		// 新しいパラメータマップを用意
		std::unordered_map<ShaderType, std::unordered_map<std::string, Parameter>> newParams;

		// 1つのステージを作り直すラムダ式
		auto processShader = [&](ShaderAsset* shader, ShaderType stage)
			{
				if (!shader) return;

				auto& params = m_params[stage];
				auto& newOnceParams = newParams[stage];

				for (const auto& param : shader->GetParams())
				{
					// すでに旧パラメータに存在していればその値を引き継ぐ
					if (params.contains(param.name))
					{
						newOnceParams[param.name] = params[param.name];
					}

					// 新しいパラメータなら型に応じたデフォルト値を設定
					else
					{
						newOnceParams[param.name] = { param.type, GetDefaultParam(param.type) };
					}
				}
			};

		// 全シェーダーから最新パラメータを作り直す
		if (m_assetManager)
		{
			processShader(m_assetManager->Get(m_vertexShader), ShaderType::Vertex);
			processShader(m_assetManager->Get(m_pixelShader), ShaderType::Pixel);
		}

		// 古いパラメータを新しいパラメータで上書き
		m_params = std::move(newParams);

		// プロパティ変更
		m_isDirty = true;
	}

	MaterialParamVariant MaterialAsset::GetDefaultParam(ShaderParamType type)
	{
		switch (type)
		{
		case REngine::ShaderParamType::Float:
			return (float)0.0f;
		case REngine::ShaderParamType::Float2:
			return DirectX::SimpleMath::Vector2::Zero;
		case REngine::ShaderParamType::Float3:
			return DirectX::SimpleMath::Vector3::Zero;
		case REngine::ShaderParamType::Color:
			return DirectX::SimpleMath::Color{ 1, 1, 1, 1 };
		case REngine::ShaderParamType::Float4:
			return DirectX::SimpleMath::Vector4::Zero;
		case REngine::ShaderParamType::Float4x4:
			return DirectX::SimpleMath::Matrix::Identity;
		case REngine::ShaderParamType::Texture2D:
			return ERROR_HANDLE<Texture>;
		case REngine::ShaderParamType::Sampler:
			return SamplerType::None;
		default:
			return MaterialParamVariant{};
		}
	}

	Property MaterialAsset::CreatePropertyFromParameter(const std::string& name, Parameter& parameter)
	{
		Property prop;
		prop.name = name;	// 名前を取得

		// variantの中身のアドレスを取得
		std::visit([&](auto&& val) {
			using T = std::decay_t<decltype(val)>;
			prop.value = static_cast<void*>(&val);	// void*に変換して格納
			prop.type = GetPropertyType<T>();		// 型をPropertyTypeに変換
			prop.typeIndex = PropertyTypeIndex::GetTypeIndex<T>();		// type_indexを取得
			}, parameter.value
		);

		return prop;
	}

	void MaterialAsset::CheckAndDoRebuild()
	{
		// 再構築の必要性があるかを調べる
		if (!m_assetManager || !m_needRebuildParams) return;

		// 各ステージのシェーダーを取得
		auto* vs = m_assetManager->Get(m_vertexShader);
		auto* ps = m_assetManager->Get(m_pixelShader);

		// VSが存在し、かつロードが終わっているかチェック
		bool vsReady = !m_vertexShader.IsValid() || (vs && vs->GetStatus() == LoadStatus::Loaded);

		// PSは設定されていないか、設定されている場合はロードが終わっているかチェック
		bool psReady = !m_pixelShader.IsValid() || (ps && ps->GetStatus() == LoadStatus::Loaded);

		// 両方問題がなければ
		if (vsReady && psReady) {
			RebuildParams();		// 再構築
			m_needRebuildParams = false; // 再構築完了フラグのリセット
		}
	}

	ShaderAsset* MaterialAsset::GetShaderAsset(ShaderType type)
	{
		switch (type)
		{
			// 頂点シェーダ
		case REngine::ShaderType::Vertex:
			return m_assetManager->Get<ShaderAsset>(m_vertexShader);
			// ピクセルシェーダ
		case REngine::ShaderType::Pixel:
			return m_assetManager->Get<ShaderAsset>(m_pixelShader);
		default:
			return nullptr;
		}
	}
}

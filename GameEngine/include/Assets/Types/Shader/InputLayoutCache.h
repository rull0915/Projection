//====================================================//
// ファイル名   : InputLayoutCache.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/25
//
// 概要 : 入力レイアウトを保持しておくシングルトンクラス
//
// 更新履歴 :
// 2026/09/25 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <d3d11.h>
#include <cstdint>
#include <wrl/client.h>
#include <unordered_map>
#include "Assets/Types/Vertex/VertexElementInfo.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class InputLayoutCache
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// IDをキーとしたInputLayoutのマップ
		std::unordered_map<uint64_t, Microsoft::WRL::ComPtr<ID3D11InputLayout>> m_inputLayoutCache;

	private:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		InputLayoutCache() = default;
		~InputLayoutCache() = default;

	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// インスタンス取得
		static InputLayoutCache& Instance()
		{
			static InputLayoutCache instance;
			return instance;
		}

		// InputLayoutを取得|作成する関数
		Microsoft::WRL::ComPtr<ID3D11InputLayout> GetOrCreate(
			ID3D11Device* device,	// デバイス
			uint32_t vertexId,		// 頂点構造体のID
			ID3DBlob* vsBlob,		// コンパイル済みシェーダーのバイナリ
			const std::vector<VertexElementInfo>& layoutInfo	// CPU側の頂点レイアウト
		);

		// キャッシュクリア関数
		void Clear()
		{
			m_inputLayoutCache.clear();
		}

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// vsBlobから32bitハッシュを取得する関数
		uint32_t GetHash(ID3DBlob* blob)
		{
			// nullptrなら0
			if (!blob) return 0;

			// uint8_tに変換
			const uint8_t* data = static_cast<const uint8_t*>(blob->GetBufferPointer());
			size_t size = blob->GetBufferSize();

			// 32bit用のFNV-1aを使用しハッシュ計算
			uint32_t hash = 2166136261u;
			for (size_t i = 0; i < size; ++i)
			{
				hash ^= data[i];
				hash *= 16777619u;
			}
			return hash;
		}
	};
}

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
#include <unordered_map>

#include "Assets/Types/Vertex/VertexElementInfo.h"
#include "Assets/Types/Shader/ShaderAsset.h"

namespace RHEngine
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

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		InputLayoutCache() = default;
		~InputLayoutCache() = default;

	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// InputLayoutを取得|作成する関数
		Microsoft::WRL::ComPtr<ID3D11InputLayout> GetOrCreate(
			ID3D11Device* device,	// デバイス
			uint32_t vertexId,		// 頂点構造体のID
			ShaderAsset* vs,	// シェーダーアセット
			const std::vector<VertexElementInfo>& layoutInfo	// CPU側の頂点レイアウト
		);

		// キャッシュクリア関数
		void Clear()
		{
			m_inputLayoutCache.clear();
		}
	};
}

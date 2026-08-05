//====================================================//
// ファイル名   : Texture.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : 画像アセット
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Assets/Objects/AssetBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Texture : public AssetBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ShaderResourceView
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
			: m_srv{ srv }
		{
		}
		~Texture() = default;

		// SRVを取得する関数
		ID3D11ShaderResourceView* Get() const { return m_srv.Get(); }
		ID3D11ShaderResourceView* const* GetAddressOf() const { return m_srv.GetAddressOf(); }
	};
}	// namespace REngine

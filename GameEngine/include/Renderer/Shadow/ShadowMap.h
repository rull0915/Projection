//====================================================//
// ファイル名   : ShadowMap.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要 : シャドウマップ
//
// 更新履歴 :
// 2026/10/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <d3d11.h>
#include <wrl/client.h>

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ShadowMap
	{
	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------


		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;		// テクスチャ
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;	// 深度
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;	// SRV
		D3D11_VIEWPORT m_viewport{};							// ビューポート

		// 前の情報保持用
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_prevRTV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_prevDSV;
		D3D11_VIEWPORT m_prevViewport;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ShadowMap() = default;
		~ShadowMap() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

	public:

		// 初期化関数
		bool Initialize(
			ID3D11Device* device,
			UINT width,
			UINT height);

		// 開始関数
		void Begin(ID3D11DeviceContext* context);

		// 終了関数
		void End(ID3D11DeviceContext* context);

		// SRVの取得関数
		ID3D11ShaderResourceView* GetSRV() const;
	};
}

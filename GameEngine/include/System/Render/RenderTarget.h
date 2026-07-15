//====================================================//
// ファイル名   : RenderTarget.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/04
//
// 概要 : レンダーターゲットクラス
//
// 更新履歴 :
// 2026/07/04 新規作成
//====================================================//

#pragma once

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class RenderTarget
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// スクリーン
		unsigned int m_width;	// 幅	
		unsigned int m_height;	// 高さ
		D3D11_VIEWPORT m_viewPort;	// 描画領域設定

		// 色
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;			// 元のテクスチャデータ
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;		// 読み取り用
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;		// 書き込み用

		// 深度
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthTexture;		// 深度テクスチャ
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;		// 書き込み用

		// 前の情報保持用
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_prevRTV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_prevDSV;
		D3D11_VIEWPORT m_prevViewport;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		RenderTarget()
			: m_width{ 0 }
			, m_height{ 0 }
			, m_viewPort{}
			, m_texture{}
			, m_srv{}
			, m_rtv{}
			, m_depthTexture{}
			, m_dsv{}
			, m_prevRTV{}
			, m_prevDSV{}
			, m_prevViewport{}
		{};

		~RenderTarget() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

	public:

		// 作成
		void Create(
			ID3D11Device* device,
			unsigned int width, unsigned int height,
			bool useDepth = true,
			DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT
		);

		// 描画開始
		void Begin(ID3D11DeviceContext* context);
		// 描画終了
		void End(ID3D11DeviceContext* context);

		// 描画リセット
		void Clear(ID3D11DeviceContext* context, const DirectX::SimpleMath::Color& color);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ShaderResourceView
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv.Get(); }
	};
}	// namespace REngine

/**
 * @file    RenderTarget.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/04
 *
 * @brief   レンダーターゲット
 */

#pragma once

#include <d3d11.h>
#include <dxgiformat.h>
#include <wrl/client.h>

namespace RHEngine
{
	/**
	 * @brief レンダーターゲット
	 * @details バックバッファとは別のスクリーンに描画を行いたい場合に使用する
	 */
	class RenderTarget
	{
	private:

		/** @brief スクリーンの幅 */
		unsigned int m_width;

		/** @brief スクリーンの高さ */
		unsigned int m_height;

		/** @brief ビューポート */
		D3D11_VIEWPORT m_viewPort;

		/// @name カラーチャンネル
		/// @{
		
		/** @brief 元のテクスチャ */
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;

		/** @brief 読み取り用SRV */
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

		/** @brief 書き込み用RTV */
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

		/// @}

		/// @name 深度
		/// @{

		/** @brief 深度のテクスチャ */
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthTexture;

		/** @brief 深度書き込み用のDSV */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;

		/// @}

		/// @name 復元用
		/// @{
		
		/** @brief バインド前のRTV */
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_prevRTV;

		/** @brief バインド前のDSV */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_prevDSV;

		/** @brief バインド前のビューポート */
		D3D11_VIEWPORT m_prevViewport;

		/// @}

		/// @name スクリーンのリサイズ用

		/** @brief 初期化時に使用されたデバイス */
		ID3D11Device* m_pDevice;

		/** @brief 深度を扱うかどうか */
		bool m_useDepth;

		/** @brief カラーチャンネルのフォーマット */
		DXGI_FORMAT m_colorFormat;

		/** @brief 深度チャンネルのフォーマット */
		DXGI_FORMAT m_depthFormat;

	public:

		/// @name ライフサイクル
		/// @{

		RenderTarget();
		
		~RenderTarget() = default;

		/// @}

		/**
		 * @brief レンダーターゲットを作成する
		 * @param pDevice デバイス
		 * @param width スクリーンの幅
		 * @param height スクリーンの高さ
		 * @param useDepth 深度を扱うかどうか
		 * @param colorFormat 色のフォーマット
		 * @param depthFormat 深度のフォーマット
		 */
		void Create(
			ID3D11Device* pDevice,
			unsigned int width, unsigned int height,
			bool useDepth = true,
			DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT
		);

		/**
		 * @brief レンダーターゲットへの描画を開始する
		 * @param pContext コンテキスト
		 */
		void Begin(ID3D11DeviceContext* pContext);


		/**
		 * @brief レンダーターゲットへの描画を終了する
		 * @param pContext コンテキスト
		 */
		void End(ID3D11DeviceContext* pContext);

		/**
		 * @brief レンダーターゲットのスクリーンをクリアする
		 * @param pContext コンテキスト
		 * @param color 塗りつぶす色
		 */
		void Clear(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Color& color);

		/**
		 * @brief スクリーンをリサイズする
		 * @param width リサイズ後の幅
		 * @param height リサイズ後の高さ
		 */
		void Resize(int width, int height);

		/**
		 * @brief SRVを取得する
		 */
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv.Get(); }
	};
}	// namespace RHEngine

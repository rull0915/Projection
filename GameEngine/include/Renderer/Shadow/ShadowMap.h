/**
 * @file    ShadowMap.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 *
 * @brief   シャドウマップ
 */

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <Windows.h>

namespace RHEngine
{
	/**
	 * @brief シャドウマップテクスチャを保持する
	 */
	class ShadowMap
	{
	private:

		/** @brief テクスチャ */
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;

		/** @brief 震度バッファ */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;

		/** @brief 読み込み時用SRV */
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

		/** @brief ビューポート */
		D3D11_VIEWPORT m_viewport{};

		/** @brief バインド前のRTV */
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_prevRTV;

		/** @brief バインド前のDSV */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_prevDSV;

		/** @brief バインド前のビューポート */
		D3D11_VIEWPORT m_prevViewport;

	public:

		/// @name ライフサイクル
		/// @{

		ShadowMap() = default;

		~ShadowMap() = default;

		/// @}

		/**
		 * @brief 各バッファの生成を行う
		 * @param pDevice デバイス
		 * @param width ウィンドウ幅
		 * @param height ウィンドウ高さ
		 * @return 成功したかどうか
		 */
		bool Initialize(
			ID3D11Device* pDevice,
			UINT width,
			UINT height);

		/**
		 * @brief シャドウマップへの描画を開始する
		 * @param pContext コンテキスト
		 */
		void Begin(ID3D11DeviceContext* pContext);

		/**
		 * @brief シャドウマップへの描画を終了する
		 * @param pContext コンテキスト
		 */
		void End(ID3D11DeviceContext* pContext);

		/**
		 * @brief シャドウマップが書かれたSRVを取得する
		 */
		ID3D11ShaderResourceView* GetSRV() const;
	};
}	// namespace RHEngine

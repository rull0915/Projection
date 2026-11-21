/**
 * @file    GraphicsManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/18
 *
 * @brief   描画を管理するシングルトンクラス
 */

#pragma once

#include <memory>
#include <d3d11.h>

#include "System/DeviceResources.h"
#include "InputLayoutCache.h"

#include "SamplerList.h"
#include "DepthStencilStateList.h"
#include "BlendStateList.h"
#include "RasterizerStateList.h"

namespace RHEngine
{
	/**
	 * @brief 描画関係のリソースを管理する
	 */
	class GraphicsManager
	{
	private:

		/** @brief デバイスリソース */
		DX::DeviceResources* m_pDeviceResources;

		/** @brief インプットレイアウトキャッシュ  */
		InputLayoutCache m_inputLayoutCache;

		/** @brief サンプラーリスト */
		SamplerList m_samplerList;

		/** @brief 深度ステンシルステートリスト */
		DepthStencilStateList m_depthStencilSateList;

		/** @brief ブレンドステートリスト */
		BlendStateList m_blendStateList;

		/** @brief ラスタライザステートリスト */
		RasterizerStateList m_rasterizerList;

	public:

		/// @name ライフサイクル
		/// @{

		GraphicsManager(DX::DeviceResources* pDeviceResources)
			: m_pDeviceResources{ pDeviceResources }
			, m_inputLayoutCache{}
			, m_samplerList{}
		{
		};

		~GraphicsManager() = default;

		/// @}

		/**
		 * @brief 初期化が必要なリソースの初期化を行う
		 */
		void Initialize()
		{
			// デバイス取得
			ID3D11Device* pDevice = m_pDeviceResources->GetD3DDevice();

			// サンプラーリストの初期化
			m_samplerList.Initialize(pDevice);

			// 深度ステンシルステートの初期化
			m_depthStencilSateList.Initialize(pDevice);

			// ブレンドステートの初期化
			m_blendStateList.Initialize(pDevice);

			// ラスタライザステートの初期化
			m_rasterizerList.Initialize(pDevice);
		}

		/// @name ゲッター
		/// @{

		/** @brief デバイスリソース */
		DX::DeviceResources* GetDeviceResources() const { return m_pDeviceResources; }

		/** @brief インプットレイアウトキャッシュ */
		InputLayoutCache& GetInputLayoutCache() { return m_inputLayoutCache; };

		/** @brief サンプラーリスト */
		const SamplerList& GetSamplerList() const { return m_samplerList; };

		/** @brief 深度ステンシルステートリスト */
		DepthStencilStateList& GetDepthStencilList() { return m_depthStencilSateList; }
		
		/** @brief ブレントステートリスト */
		BlendStateList& GetBlendList() { return m_blendStateList; }

		/** @brief ラスタライザステートリスト */
		RasterizerStateList& GetRasterizerList() { return m_rasterizerList; }

		/// @}
	};
}	// namespace RHEngine

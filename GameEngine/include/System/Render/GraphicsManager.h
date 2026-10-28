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

namespace RHEngine
{
	/**
	 * @brief 描画関係のリソースを管理する
	 */
	class GraphicsManager
	{
	private:

		/** @brief デバイスリソース */
		std::unique_ptr<DX::DeviceResources> m_deviceResources;

		/** @brief インプットレイアウトキャッシュ  */
		InputLayoutCache m_inputLayoutCache;

		/** @brief サンプラーリスト */
		SamplerList m_samplerList;

	private:

		/// @name ライフサイクル
		/// @{

		GraphicsManager()
			: m_deviceResources{ std::make_unique<DX::DeviceResources>() }
			, m_inputLayoutCache{}
			, m_samplerList{}
		{
		};

		~GraphicsManager() = default;

		/// @}

	public:

		/**
		 * @brief インスタンスを取得する
		 * @return 静的なインスタンス
		 */
		static GraphicsManager& Instance()
		{
			static GraphicsManager instance;
			return instance;
		}

		/**
		 * @brief 初期化が必要なリソースの初期化を行う
		 */
		void Initialize()
		{
			// デバイス取得
			ID3D11Device* pDevice = m_deviceResources->GetD3DDevice();

			// サンプラーリストの初期化
			m_samplerList.Initialize(pDevice);
		}

		/// @name ゲッター
		/// @{

		/** @brief デバイスリソース */
		DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }

		/** @brief インプットレイアウトキャッシュ */
		InputLayoutCache& GetInputLayoutCache() { return m_inputLayoutCache; };

		/** @brief サンプラーリスト */
		SamplerList& GetSamplerList() { return m_samplerList; };

		/// @}
	};
}	// namespace RHEngine

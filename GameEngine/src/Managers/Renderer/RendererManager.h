/**
 * @file    RendererManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/03
 *
 * @brief   描画コンポーネントを管理するクラス
 */


#pragma once

#include "../ReserveContainer.h"
#include "Components/World/Renderer/RendererBase.h"

namespace RHEngine
{
	/**
	 * @brief 描画を行うコンポーネントを管理する
	 */
	class RendererManager : public ReserveContainer<RendererBase>
	{
	public:

		/// @name ライフサイクル
		/// @{

		RendererManager();

		~RendererManager();

		/// @}

		/**
		 * @brief 全描画コンポーネントを描画する
		 * @param renderer 描画担当
		 */
		void DrawAll(Renderer& renderer);
	};
}	// namespace RHEngine

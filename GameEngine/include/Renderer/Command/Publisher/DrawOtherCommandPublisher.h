/**
 * @file    DrawOtherCommandPublisher.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/07
 *
 * @brief   その他描画コマンドを発行するクラス
 */

#pragma once

#include <SimpleMath.h>

#include "Renderer/Command/DrawCommandContainer.h"
#include "Renderer/GraphicsSystem.h"
#include "Assets/Types/Model/Mesh.h"

namespace RHEngine
{
	/**
	 * @brief メッシュ描画コマンドを発行する
	 */
	class DrawOtherCommandPublisher
	{
	private:

		/** @brief コマンドのコンテナ */
		DrawCommandContainer& m_commandContainer;

		/** @brief 描画システム */
		GraphicsSystem& m_graphicSystem;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param container コマンドコンテナ
		 * @param graphicSystem 描画システム
		 */
		DrawOtherCommandPublisher(DrawCommandContainer& container, GraphicsSystem& graphicSystem)
			: m_commandContainer{ container }
			, m_graphicSystem{ graphicSystem }
		{}

		~DrawOtherCommandPublisher() = default;

		/// @}

		/**
		 * @brief スカイボックス描画コマンドを発行する
		 * @param pMesh 描画するメッシュ
		 * @param scale 描画するスケール
		 */
		void DrawSkybox(Mesh* pMesh, float scale = 1.0f)
		{
			// コマンドの生成
			auto& command = m_commandContainer.AddSkybox();

			command.pMesh = pMesh;
			command.material = m_graphicSystem.GetMaterial();
			command.scale = scale;
		}
	};
}	// namespace RHEngine

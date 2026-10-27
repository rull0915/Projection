/**
 * @file    DrawMeshCommandPublisher.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/01
 *
 * @brief   モデルを描画のコマンドを発行するクラス
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
	class DrawMeshCommandPublisher
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
		DrawMeshCommandPublisher(DrawCommandContainer& container, GraphicsSystem& graphicSystem)
			: m_commandContainer{ container }
			, m_graphicSystem{ graphicSystem }
		{}

		~DrawMeshCommandPublisher() = default;

		/// @}

		/**
		 * @brief メッシュ描画コマンドを発行する
		 * @param pMesh 描画するメッシュ
		 * @param subMeshIndex サブメッシュのインデックス
		 * @param world ワールド行列
		 */
		void DrawMesh(Mesh* pMesh, size_t subMeshIndex, DirectX::SimpleMath::Matrix world)
		{
			// コマンドの生成
			auto& command = m_commandContainer.AddModel();

			command.pMesh = pMesh;
			command.subMeshIndex = subMeshIndex;
			command.world = world;
			command.material = m_graphicSystem.GetMaterial();
		}
	};
}	// namespace RHEngine

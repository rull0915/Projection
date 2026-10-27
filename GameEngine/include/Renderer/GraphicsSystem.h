/**
 * @file    GraphicsSystem.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/29
 *
 * @brief   レンダーが使用する情報を管理するクラス
 */

#pragma once

#include <SimpleMath.h>

#include "Command/DrawCommandContainer.h"
#include "Command/DrawCommandExecutor.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Objects/Handle.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	class AssetManager;
	class RenderInfoManager;

	/**
	 * @brief レンダーが使用する情報を管理する
	 */
	class GraphicsSystem
	{
	private:

		/** @brief ワールド行列 */
		DirectX::SimpleMath::Matrix m_world;

		/** @brief シーンデータ */
		SceneData m_sceneData;

		/** @brief 使用中のマテリアルハンドル */
		Handle<MaterialAsset> m_material;

		/** @brief コマンドをため込む */
		DrawCommandContainer m_drawCommandContainer;

		/** @brief コマンドを実行する */
		DrawCommandExecutor m_drawCommandExecutor;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetManager アセットマネージャー
		 * @param infoManager 
		 */
		GraphicsSystem(AssetManager& assetManager, RenderInfoManager& infoManager)
			: m_world		{ DirectX::SimpleMath::Matrix::Identity }
			, m_material	{ ERROR_HANDLE<MaterialAsset> }
			, m_drawCommandContainer{}
			, m_drawCommandExecutor{ assetManager, infoManager }
		{
		};
		~GraphicsSystem() = default;
		
		/// @}

		/**
		 * @brief 初期化
		 */
		void Initialize()
		{
			// コマンド実行者の初期化
			m_drawCommandExecutor.Initialize();

			// 初期PrimitiveCommandを生成しておく
			auto& cmd = m_drawCommandContainer.AddPrimitive();
			cmd.world = DirectX::SimpleMath::Matrix::Identity;
		}

		/**
		 * @brief 貯めたコマンドを実行する
		 */
		void ExecuteCommands()
		{
			// 貯めたコマンドを実行
			m_drawCommandExecutor.DrawCommandExecute(m_drawCommandContainer);

			// コマンドのリセット
			m_drawCommandContainer.Clear();

			// PrimitiveCommandを初期化
			auto& p = m_drawCommandContainer.AddPrimitive();
			p.world = m_world;
			p.material = m_material;
		}

		/// @name ゲッター
		/// @{

		/** @brief マテリアルハンドル */
		Handle<MaterialAsset> GetMaterial() const { return m_material; }

		/** @brief コマンドコンテナ */
		DrawCommandContainer& GetCommandContainer() { return m_drawCommandContainer; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief ワールド行列 */
		void SetWorld(const DirectX::SimpleMath::Matrix& world)
		{
			if (world != m_world)
			{
				m_world = world;

				// PrimitiveCommandを更新
				auto& p = m_drawCommandContainer.AddPrimitive();
				p.world = world;
				p.material = m_material;
			}
		}

		/** @brief マテリアルハンドル */
		void SetMaterial(Handle<MaterialAsset> handle)
		{
			// 違うマテリアルを設定するとき
			if (handle != m_material)
			{
				m_material = handle;

				// PrimitiveCommandを更新
				auto& p = m_drawCommandContainer.AddPrimitive();
				p.world = m_world;
				p.material = handle;
			}
		}

		/// @}
	};
}	// namespace RHEngine

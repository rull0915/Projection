/**
 * @file    UpdatePipeline.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/05
 *
 * @brief   各管理クラスの所有 更新順序の管理をするクラス
 */
#pragma once

#include <memory>

#include "Managers/ObjectManager.h"
#include "Managers/CameraManager.h"
#include "Managers/Renderer/RendererManager.h"
#include "Managers/Renderer/LightManager.h"
#include "Managers/2DManagers/PhysicsManager2D.h"
#include "Managers/2DManagers/Collider/CollideManager2D.h"
#include "Managers/3DManagers/PhysicsManager.h"
#include "Managers/Sounds/SoundManager.h"
#include "Managers/3DManagers/Collider/CollideManager.h"
#include "Managers/UI/UIManager.h"
#include "Managers/System/CollideEventSystem.h"

#include "Scene/UpdateMode.h"

namespace RHEngine
{
	class Scene;
	class GameTimer;

	class UpdatePipeline
	{
		friend class ComponentRegister;
	private:

		/** @brief 自身を所有しているシーンのポインタ */
		Scene* m_pScene;

		/// @name 各マネージャーの実体
		/// @{
		
		std::unique_ptr<PhysicsManager> m_physicsManager;		///< 3D物理
		std::unique_ptr<PhysicsManager2D> m_physicsManager2D;	///< 2D物理
		std::unique_ptr<CollideEventSystem> m_colEvent;			///< 衝突時のイベント
		std::unique_ptr<CameraManager> m_cameraManager;		///< カメラ
		std::unique_ptr<RendererManager> m_rendererManager;	///< 描画
		std::unique_ptr<LightManager> m_lightManager;		///< ライト
		std::unique_ptr<SoundManager> m_soundManager;		///< 音
		std::unique_ptr<ObjectManager> m_objectManager;		///< オブジェクト
		std::unique_ptr<UIManager> m_uiManager;				///< UI

		/// @}

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene 自身を所有するシーンポインタ
		 */
		UpdatePipeline(Scene* pScene);

		~UpdatePipeline() = default;

		/// @}

		/**
		 * @brief 更新する 各マネージャーを適切な順序で更新する
		 * @param timer タイマー
		 * @param mode 更新モード
		 */
		void Update(const GameTimer& timer, UpdateMode mode);

		/**
		 * @brief ワールド空間を描画する
		 */
		void DrawWorld(Renderer& renderer);

		/**
		 * @brief UI空間を描画する
		 */
		void DrawUI(Renderer& renderer);

		/**
		 * @brief UIデバッグ描画 (未実装)
		 * @TODO
		 */
		void DrawRects(Renderer& renderer, DirectX::SimpleMath::Color color);

		/**
		 * @brief 終了時に各マネージャーを解放する
		 */
		void Finalize();

		/// @name ゲッター
		/// @{

		/** @brief オブジェクトマネージャー */
		ObjectManager* GetObjectManager() { return m_objectManager.get(); }
		/** @brief 3D物理マネージャー */
		PhysicsManager* GetPhysicsManager() { return m_physicsManager.get(); };
		/** @brief 描画マネージャー */
		RendererManager* GetRendererManager() { return m_rendererManager.get(); }
		/** @brief UIマネージャー */
		UIManager* GetUIManager() { return m_uiManager.get(); }
		/** @brief カメラマネージャー */
		CameraManager* GetCameraManager() { return m_cameraManager.get(); }
		/** @brief ライトマネージャー */
		LightManager* GetLightManager() { return m_lightManager.get(); }

		/// @}
	};
}	// namespace RHEngine

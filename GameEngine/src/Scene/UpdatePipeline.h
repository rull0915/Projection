//====================================================//
// ファイル名   : UpdatePipeline.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/05
//
// 概要 : 各管理クラスの所有 更新順序の管理をするクラス
//
// 更新履歴 :
// 2026/07/05 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
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

	//====================================================//
	// クラス宣言
	//====================================================//
	class UpdatePipeline
	{
		friend class ComponentRegister;
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// シーンのポインタ
		Scene* m_pScene;

		// 各マネージャーの実体
		std::unique_ptr<PhysicsManager> m_physicsManager;		// 3D物理
		std::unique_ptr<PhysicsManager2D> m_physicsManager2D;	// 2D物理
		std::unique_ptr<CollideEventSystem> m_colEvent;			// 衝突時のイベント
		std::unique_ptr<CameraManager> m_cameraManager;		// カメラ
		std::unique_ptr<RendererManager> m_rendererManager;	// 描画
		std::unique_ptr<LightManager> m_lightManager;		// ライト
		std::unique_ptr<SoundManager> m_soundManager;		// 音
		std::unique_ptr<ObjectManager> m_objectManager;		// オブジェクト
		std::unique_ptr<UIManager> m_uiManager;				// UI

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		UpdatePipeline(Scene* pScene);
		~UpdatePipeline() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Update(const GameTimer& timer, UpdateMode mode);

		// 各マネージャーを介した描画
		void DrawWorld(Renderer& renderer);
		void DrawUI(Renderer& renderer);
		void DrawRects(Renderer& renderer, DirectX::SimpleMath::Color color);

		// 終了処理
		void Finalize();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// 管理クラスの取得

		// オブジェクト
		ObjectManager* GetObjectManager() { return m_objectManager.get(); }
		// 3D物理
		PhysicsManager* GetPhysicsManager() { return m_physicsManager.get(); };
		// 描画
		RendererManager* GetRendererManager() { return m_rendererManager.get(); }
		// UI
		UIManager* GetUIManager() { return m_uiManager.get(); }
		// Camera
		CameraManager* GetCameraManager() { return m_cameraManager.get(); }
		// ライト
		LightManager* GetLightManager() { return m_lightManager.get(); }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	};
}	// namespace RHEngine

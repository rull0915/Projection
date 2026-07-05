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
#include "Managers/2DManagers/PhysicsManager2D.h"
#include "Managers/2DManagers/Collider/CollideManager2D.h"
#include "Managers/3DManagers/PhysicsManager.h"
#include "Managers/Sounds/SoundManager.h"
#include "Managers/3DManagers/Collider/CollideManager.h"
#include "Managers/UI/UIManager.h"
#include "Managers/System/CollideEventSystem.h"

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

	void Initialize();

	void Update(const GameTimer& timer);

	// 各マネージャーを介した描画
	void DrawWorld(Renderer& renderer);
	void DrawUI(Renderer& renderer);
	void DrawColliders(Renderer& renderer, DirectX::SimpleMath::Color color);
	void DrawRects(Renderer& renderer, DirectX::SimpleMath::Color color);

	// 終了処理
	void Finalize();

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

	// 管理クラスの取得

	// オブジェクト
	ObjectManager* GetObjectManager() const { return m_objectManager.get(); }
	// 3D物理
	PhysicsManager* GetPhysicsManager() const { return m_physicsManager.get(); };
	// UI
	UIManager* GetUIManager() const { return m_uiManager.get(); }
	// Camera
	CameraManager* GetCameraManager() const { return m_cameraManager.get(); }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

};

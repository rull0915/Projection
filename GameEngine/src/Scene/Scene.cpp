//====================================================//
// ファイル名  : Scene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/02/17
//
// 概要        :  シーンの基底クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/Scene.h"

// 各管理クラス
#include "Scene/UpdatePipeline.h"
#include "Managers/3DManagers/Ray/RaySystem.h"
#include "System/WindowManager.h"
#include "ReferenceRegistry.h"

// 各コンポーネント
#include "Components/ComponentBase.h"
#include "Components/World/Camera/CameraBase.h"
#include "Components/Interface/IAssetDependent.h"

#include "Renderer/Renderer.h"
#include "Timer/GameTimer.h"
#include "Scene/SceneRenderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	Scene::Scene(AssetManager& assetManager)
		: m_updatePipeline{ std::make_unique<UpdatePipeline>(this) }
		, m_componentRegister{ std::make_unique<ComponentRegister>(m_updatePipeline.get()) }
		, m_objectFinder{ std::make_unique<ObjectFinder>(this) }
		, m_sceneRenderer{ std::make_unique<SceneRenderer>(this) }
		, m_objectFactory{ std::make_unique<ObjectFactory>(this) }
		, m_mainScreen{ std::make_unique<MainScreen>() }
		, m_referenceRegistry{ std::make_unique<ReferenceRegistry>() }
		, m_updateMode{ UpdateMode::Play }
		, m_assetManager{ assetManager }
	{
		// 初期化
		m_updatePipeline->Initialize();
	}

	// デストラクタ
	Scene::~Scene()
	{}

	// 初期化関数
	void Scene::Initialize()
	{
		// 待機中の参照を解決
		for (auto& ref : m_waitingRefBases)
		{
			ResolveRef(ref);
		}

		m_waitingRefBases.clear();
	}

	// 更新関数
	void Scene::Update(const GameTimer& gameTimer)
	{
		// 各管理クラスの更新
		m_updatePipeline->Update(gameTimer, m_updateMode);
	}

	// 描画関数
	void Scene::Render(Renderer& renderer)
	{
		// メインの描画
		m_sceneRenderer->RenderWithContext(
			{ GetMainCamera(), m_mainScreen->GetRenderTarget(), WindowManager::Instance().GetBackGroundColor(),
				DrawFlag::World | DrawFlag::UI | DrawFlag::WorldDebug | DrawFlag::UIDebug },
			renderer
		);
	}

	// 実際の画面へ描画する関数
	void Scene::RenderOnScreen(Renderer& renderer)
	{
		// メインスクリーンを描画
		m_mainScreen->Render(renderer);
	}

	/// <summary>
	/// 基底終了関数
	/// </summary>
	void Scene::Finalize()
	{
		ResetObjects();

		m_waitingRefBases.clear();
	}

	// オブジェクトのリセット関数
	void Scene::ResetObjects()
	{
		m_updatePipeline->Finalize();
	}

	void Scene::RegisterComponent(ComponentBase* component)
	{
		// 各マネージャーへ通知
		m_componentRegister->RegisterComponent(component);

		// コンポーネントがアセットに依存するなら
		if (auto c = dynamic_cast<IAssetDependent*>(component))
		{
			// AssetManagerを渡す
			c->ReceiveAssetManager(m_assetManager);
		}
	}

	void Scene::UnRegsisterComponent(ComponentBase* component)
	{
		// 各マネージャーへ通知
		m_componentRegister->UnRegisterComponent(component);

		// 参照レジストリに通知
		m_referenceRegistry->RemoveObj(component);
	}

	void Scene::OnGameObjectDestroy(GameObject* obj)
	{
		// 参照レジストリに通知
		m_referenceRegistry->RemoveObj(obj);
	}

	// メインカメラ設定関数
	void Scene::SetMainCamera(CameraBase* camera)
	{
		m_updatePipeline->GetCameraManager()->SetMainCamera(camera);
	}

	// メインカメラ取得関数
	CameraBase* Scene::GetMainCamera() const
	{
		return m_updatePipeline->GetCameraManager()->GetMainCamera();
	}

	// Rayの衝突を調べる
	bool Scene::RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask)
	{
		return
			RaySystem::RayCast(
				m_updatePipeline->GetPhysicsManager()->GetCollideManager()->GetAllColliders(),
				ray, max, hit, layerMask
			);
	}

	bool Scene::ResolveRef(RefBase* ref)
	{
		// Componentを探索
		PropertyObject* obj = m_componentRegister->GetComponentFromUUID<ComponentBase>(ref->GetUUID());

		// 見つからなければGameObjectを探索
		if (!obj) obj = m_objectFinder->FindWithUUID(ref->GetUUID());

		// どちらかが見つかった場合
		if (obj)
		{
			// オブジェクトをセット
			if (!ref->SetObj(obj)) 
			{
				// 失敗したらfalse
				return false;
			}

			// 成功したらレジストリに登録
			m_referenceRegistry->AddPair(ref, obj);

			return true;
		}

		return false;
	}

	void Scene::RegisterLateResolve(RefBase* ref)
	{
		m_waitingRefBases.push_back(ref);
	}

	// オブジェクトマネージャーの取得関数
	ObjectManager* Scene::GetObjectManager() const
	{
		return m_updatePipeline->GetObjectManager();
	}
}	// namespace REngine

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
#include "ReferenceRegistry.h"

// 各コンポーネント
#include "Components/ComponentBase.h"
#include "Components/Interface/IAssetDependent.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shadow/ShadowRenderer.h"
#include "Timer/GameTimer.h"
#include "Scene/SceneRenderer.h"

#include "System/Render/GraphicsManager.h"
#include "System/WindowManager.h"

namespace RHEngine
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
		, m_referenceRegistry{ std::make_unique<ReferenceRegistry>() }
		, m_updateMode{ UpdateMode::Play }
		, m_assetManager{ assetManager }
		, m_sceneData{}
		, m_gameView{}
	{
		// デバイスの取得
		ID3D11Device* device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

		// ゲームビューの作成
		m_mainWorld.Create(device, WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight());
		m_gameView.Create(device, WindowManager::Instance().GetWidth(), WindowManager::Instance().GetHeight());

		// シャドウマップを作成
		m_shadowMap.Initialize(device, 2048, 2048);
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

		// シーンデータの更新

		// ライトデータの更新
		m_updatePipeline->GetLightManager()->RebuildLightData();
		m_sceneData.lightData = m_updatePipeline->GetLightManager()->GetLightData();

		// 時間データの更新
		m_sceneData.timeData.time = gameTimer.GetTime();
		m_sceneData.timeData.elapsedTime = gameTimer.GetElapsedTime();

		// メインカメラを取得
		CameraBase* mainCamera = m_updatePipeline->GetCameraManager()->GetMainCamera();

		// 影データの更新
		// Todo : 複数ライト、スポットライト・ポイントライトへの対応
		// 初期実装は平行光源の0番目の身を扱います
		auto lightManager = m_updatePipeline->GetLightManager();
		LightComponent* dLight = lightManager->GetLight(LightType::Directional);

		// シャドウ描画用コンテキストの作成
		SceneCameraData camData = mainCamera ? mainCamera->CreateCameraData() : SceneCameraData{};
		m_sceneData.shadowData.lightCameraData = lightManager->GetLightData(camData, dLight, 2048.0f);
		m_sceneData.shadowData.shadowBias = 0.001f;	// Todo : マジックナンバーの解消
	}

	// 描画関数
	void Scene::Render(Renderer& renderer, ShadowRenderer& shadowRenderer)
	{
		m_updatePipeline->GetCameraManager()->ReflectReserves();

		// Todo: ポストエフェクトなどの処理
		ID3D11DeviceContext* context = GraphicsManager::Instance().GetDeviceResources()->GetD3DDeviceContext();

		// コンテキストの作成
		RenderContext ctx{};

		// ToRefactor カメラ毎に異なる背景色に対応
		ctx.backColor = WindowManager::Instance().GetBackGroundColor();
		ctx.flags = DrawFlag::World | DrawFlag::UI;
		ctx.clearTarget = true;

		// シャドウパスの描画
		{
			// シャドウ描画用コンテキストの作成
			ShadowRenderContext shadowCtx{};
			shadowCtx.cameraData = m_sceneData.shadowData.lightCameraData;
			shadowCtx.pShadowMap = &m_shadowMap;

			// シャドウパスの描画
			m_sceneRenderer->DrawShadowPass(shadowCtx, shadowRenderer);
		}

		// Test: シャドウマップ用スロットにバインド
		auto shadowMapSRV = m_shadowMap.GetSRV();
		context->PSSetShaderResources(10, 1, &shadowMapSRV);

		auto& sampler = GraphicsManager::Instance().GetSamplerList().GetSampler(SamplerType::PointWrap);
		context->PSSetSamplers(10, 1, sampler.GetAddressOf());

		// 描画の開始
		renderer.Begin(m_sceneData);

		// ワールドパスの描画
		{
			// 全カメラを取得
			for (auto& cmp : m_componentRegister->GetAllComponents<CameraBase>())
			{
				CameraBase* camera = static_cast<CameraBase*>(cmp);

				// RenderTargetが設定されていれば
				if (RenderTarget* target = camera->GetRenderTarget())
				{
					// 描画コンテキストを更新
					ctx.target = target;
					ctx.cameraData = camera->CreateCameraData();

					// WorldPassの描画
					m_sceneRenderer->DrawWorldPass(ctx, renderer);
				}
			}

			// メインカメラを描画
			if (CameraBase* mainCamera = m_updatePipeline->GetCameraManager()->GetMainCamera())
			{
				// 描画コンテキストを更新
				ctx.target = &m_mainWorld;
				ctx.cameraData = mainCamera->CreateCameraData();

				// WorldPassの描画
				m_sceneRenderer->DrawWorldPass(ctx, renderer);
			}
		}
			
		// Test: シャドウマップ用スロットをアンバインド
		ID3D11ShaderResourceView* nullSRV = nullptr;
		context->PSSetShaderResources(10, 1, &nullSRV);

		// メインカメラをフルスクリーン描画するcommandを発行
		renderer.Draw().Sprite().DrawFullScreen(m_mainWorld.GetShaderResourceView());

		// UIパスの描画
		{
			ctx.target = &m_gameView;
			ctx.cameraData = {};

			m_sceneRenderer->DrawUIPass(ctx, renderer);

			// メインカメラをフルスクリーン描画するcommandを発行
			renderer.Draw().Sprite().DrawFullScreen(m_gameView.GetShaderResourceView());
		}
	}

	void Scene::DrawOnceCamera(const RenderContext& data, Renderer& renderer, bool useShadow)
	{
		ID3D11DeviceContext* context = GraphicsManager::Instance().GetDeviceResources()->GetD3DDeviceContext();

		if (useShadow)
		{
			// Test: シャドウマップ用スロットにバインド
			auto shadowMapSRV = m_shadowMap.GetSRV();
			context->PSSetShaderResources(10, 1, &shadowMapSRV);

			auto& sampler = GraphicsManager::Instance().GetSamplerList().GetSampler(SamplerType::PointWrap);
			context->PSSetSamplers(10, 1, sampler.GetAddressOf());
		}

		// WorldPassの描画
		m_sceneRenderer->DrawWorldPass(data, renderer);

		if (useShadow)
		{
			// Test: シャドウマップ用スロットをアンバインド
			ID3D11ShaderResourceView* nullSRV = nullptr;
			context->PSSetShaderResources(10, 1, &nullSRV);
		}
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

	// Rayの衝突を調べる
	bool Scene::RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask)
	{
		return
			RaySystem::RayCast(
				m_updatePipeline->GetPhysicsManager()->GetCollideManager()->GetObjects(),
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
}	// namespace RHEngine

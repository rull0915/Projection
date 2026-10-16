//====================================================//
// ファイル名   : Scene.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/02/06
//
// 概要 : Sceneの基底クラス
//
// 更新履歴 :
// 2026/02/06 新規作成
// 2026/05/04 コンポーネントのマネージャーを所持するように変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>
#include <vector>

#include "Physics/Ray.h"
#include "Physics/RaycastHit.h"
#include "UpdateMode.h"

#include "ObjectFactory.h"
#include "ComponentRegister.h"
#include "ObjectFinder.h"
#include "Assets/Managers/AssetManager.h"
#include "Common/ObjectReference.h"
#include "System/Render/RenderTarget.h"
#include "System/Render/RenderContext.h"
#include "Renderer/Shadow/ShadowMap.h"
#include "SceneInfo.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//

	class Renderer;
	class ShadowRenderer;
	class CameraBase;
	class Canvas;

	class SceneRenderer;
	class UpdatePipeline;
	class ObjectManager;
	class ReferenceRegistry;

	//====================================================//
	// クラス宣言
	//====================================================//
	class Scene final
	{
	private:

		// マネージャー管理をするクラス
		std::unique_ptr<UpdatePipeline> m_updatePipeline;

		// コンポーネント登録システム
		std::unique_ptr<ComponentRegister> m_componentRegister;

		// オブジェクト検索
		std::unique_ptr<ObjectFinder> m_objectFinder;

		// シーン描画
		std::unique_ptr<SceneRenderer> m_sceneRenderer;

		// ファクトリ
		std::unique_ptr<ObjectFactory> m_objectFactory;

		// 参照レジストリ
		std::unique_ptr<ReferenceRegistry> m_referenceRegistry;

		// 更新状態
		UpdateMode m_updateMode;

		// アセットマネージャー
		AssetManager& m_assetManager;

		// 参照解決待ちリスト
		std::vector<RefBase*> m_waitingRefBases;

		// シーンデータ
		SceneData m_sceneData;

		// ゲームビュー
		RenderTarget m_mainWorld;	// メインカメラがワールド空間を描画するターゲット
		RenderTarget m_gameView;	// ゲームビュー

		// ShadowMap
		ShadowMap m_shadowMap;	// Todo : シャドウ管理クラスに委譲

	public:

		// コンストラクタ
		Scene(AssetManager& assetManager);

		// デストラクタ
		virtual ~Scene();

		// 初期化処理
		void Initialize();

		// 更新処理
		void Update(const GameTimer& gameTimer);

		// 描画処理
		void Render(Renderer& renderer, ShadowRenderer& shadowRenderer);

		// カメラ情報から描画する関数
		void DrawOnceCamera(const RenderContext& data, Renderer& renderer);

		// 終了処理
		void Finalize();

		// 全オブジェクトの削除関数
		void ResetObjects();

	public:

		// コンポーネントを登録する関数
		void RegisterComponent(ComponentBase* component);

		// コンポーネントの登録を解除する関数
		void UnRegsisterComponent(ComponentBase* component);

		// オブジェクトの削除が通知される関数
		void OnGameObjectDestroy(GameObject* obj);

		// RayCast関数
		bool RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);

		// RefBaseの参照を作成する関数
		bool ResolveRef(RefBase* ref);

		// RefBaseの遅延解決を予約する関数
		void RegisterLateResolve(RefBase* ref);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// 管理クラス
		ObjectManager* GetObjectManager() const;

		// コンポーネント管理者
		ComponentRegister* GetComponentRegister() const { return m_componentRegister.get(); }

		// オブジェクトファインダー
		ObjectFinder* GetObjectFinder() const { return m_objectFinder.get(); }

		// パイプライン
		UpdatePipeline* GetPipeline() const { return m_updatePipeline.get(); }

		// ファクトリ
		ObjectFactory* GetFactory() const { return m_objectFactory.get(); }

		// アセットマネージャー
		AssetManager& GetAssetManager() const { return m_assetManager; }

		// シーンデータ
		SceneData& GetSceneData() { return m_sceneData; }

		// ゲームビューの取得
		ID3D11ShaderResourceView* GetGameView() const { return m_gameView.GetShaderResourceView(); }

		// ToDelete Debug用 シャドウマップテクスチャの取得
		ID3D11ShaderResourceView* GetShadowMapTexture() const { return m_shadowMap.GetSRV(); }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// 更新モード
		void SetUpdateMode(UpdateMode mode) { m_updateMode = mode; }
	};
} // namespace REngine

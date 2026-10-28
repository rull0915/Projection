/**
 * @file    Scene.h
 * @author  Hoshino Ryunosuke
 * @date    2026/02/06
 *
 * @brief   Sceneの基底クラス
 */

#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <d3d11.h>

#include "UpdateMode.h"
#include "Common/ObjectReference.h"
#include "System/Render/RenderTarget.h"
#include "System/Render/RenderContext.h"
#include "Renderer/Shadow/ShadowMap.h"
#include "SceneInfo.h"

namespace RHEngine
{
	class Renderer;
	class ShadowRenderer;

	class SceneRenderer;
	class UpdatePipeline;
	class ReferenceRegistry;
	class ComponentRegister;
	class ObjectFinder;
	class ObjectFactory;

	class ObjectManager;

	class Ray;
	struct RaycastHit;

	class ComponentBase;
	class GameObject;

	class AssetManager;
	class GameTimer;

	/**
	 * @brief シーンクラス
	 */
	class Scene final
	{
	private:

		/** @brief マネージャー管理をするクラス */
		std::unique_ptr<UpdatePipeline> m_updatePipeline;

		/** @brief コンポーネント登録システム */
		std::unique_ptr<ComponentRegister> m_componentRegister;

		/** @brief オブジェクト検索 */
		std::unique_ptr<ObjectFinder> m_objectFinder;

		/** @brief シーン描画 */
		std::unique_ptr<SceneRenderer> m_sceneRenderer;

		/** @brief ファクトリ */
		std::unique_ptr<ObjectFactory> m_objectFactory;

		/** @brief 参照レジストリ */
		std::unique_ptr<ReferenceRegistry> m_referenceRegistry;

		/** @brief 更新状態 */
		UpdateMode m_updateMode;

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

		/** @brief 参照解決待ちリスト */
		std::vector<RefBase*> m_waitingRefBases;

		/** @brief シーンデータ */
		SceneData m_sceneData;

		/** @brief ワールド空間を描画するターゲット*/
		RenderTarget m_mainWorld;

		/** @brief ゲームビュー */
		RenderTarget m_gameView;

		/** 
		 * @brief ShadowMap
		 * @TODO シャドウパス管理クラスへ委譲
		 */
		ShadowMap m_shadowMap;

		/** @brief 描画する情報の一覧 */
		std::vector<RenderContext> m_renderContexts;

	public:

		/// @name ライフサイクル
		/// @{
		
		/**
		 * @param assetManager アセットマネージャー
		 */
		Scene(AssetManager& assetManager);

		virtual ~Scene();

		/// @}

		/**
		 * @brief Ref参照の解決を行う
		 */
		void Initialize();

		/**
		 * @brief シーン全体の更新を行う
		 * @param gameTimer タイマー
		 */
		void Update(const GameTimer& gameTimer);

		/**
		 * @brief ウィンドウサイズの変更を各コンポーネントに通知する
		 * @param width 変更後のウィンドウ幅
		 * @param height 変更後のウィンドウ高さ
		 */
		void OnWindowSizeChanged(int width, int height);

		/**
		 * @brief 描画準備
		 */
		void PrepareRender();

		/**
		 * @brief シーン全体の描画を行う
		 * @param renderer 描画クラス
		 * @param shadowRenderer 影描画クラス
		 */
		void Render(Renderer& renderer, ShadowRenderer& shadowRenderer);

		/**
		 * @brief 1カメラを描画する
		 * @param data 描画データ
		 * @param renderer 描画関数
		 * @param useShadow 影を描画するかどうか
		 */
		void DrawOnceCamera(const RenderContext& data, Renderer& renderer, bool useShadow);

		/**
		 * @brief 終了処理
		 */
		void Finalize();

		/**
		 * @brief 全オブジェクトを削除する
		 */
		void ResetObjects();

		/**
		 * @brief コンポーネントを登録する
		 * @param pComponent 登録されるコンポーネント
		 */
		void RegisterComponent(ComponentBase* pComponent);

		/**
		 * @brief コンポーネントを登録解除する
		 * @param pComponent 解除されるコンポーネント
		 */
		void UnRegsisterComponent(ComponentBase* pComponent);

		/**
		 * @brief オブジェクト削除時の処理
		 * @param pObj 削除されるオブジェクト
		 */
		void OnGameObjectDestroy(GameObject* pObj);

		/**
		 * @brief Rayの衝突を調べる
		 * @param ray 調べるRay
		 * @param max Rayが届く最大の距離
		 * @param hit 衝突情報を書き込む変数
		 * @param layerMask 衝突するレイヤー
		 * @return 
		 */
		bool RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);

		/**
		 * @brief Refの参照を解決する
		 * @param pRef 解決対象のRef
		 * @return 成功したかどうか
		 */
		bool ResolveRef(RefBase* pRef);

		/**
		 * @brief Refの遅延解決を要請する
		 * @param pRef 対象のRef
		 */
		void RegisterLateResolve(RefBase* pRef);

		/// @name ゲッター
		/// @{

		/** @brief オブジェクト管理クラス */
		ObjectManager* GetObjectManager() const;

		/** @brief コンポーネントレジスタ */
		ComponentRegister* GetComponentRegister() const { return m_componentRegister.get(); }

		/** @brief オブジェクトファインダー */
		ObjectFinder* GetObjectFinder() const { return m_objectFinder.get(); }

		/** @brief パイプライン */
		UpdatePipeline* GetPipeline() const { return m_updatePipeline.get(); }

		/** @brief ファクトリ */
		ObjectFactory* GetFactory() const { return m_objectFactory.get(); }

		/** @brief アセットマネージャー */
		AssetManager& GetAssetManager() const { return m_assetManager; }

		/** @brief シーンデータ */
		SceneData& GetSceneData() { return m_sceneData; }

		/** @brief ゲームビューの取得 */
		ID3D11ShaderResourceView* GetGameView() const { return m_gameView.GetShaderResourceView(); }

		/// @}

		/// @name セッター
		/// @{

		/** @brief 更新モード */
		void SetUpdateMode(UpdateMode mode) { m_updateMode = mode; }

		/// @}
	};
} // namespace RHEngine

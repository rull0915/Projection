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
#include "GameObject/GameObject.h"

#include "Transition/TransitionBase.h"
#include "Physics/Ray.h"
#include "Physics/RaycastHit.h"

#include "System/Render/RenderContext.h"
#include "System/Render/RenderTarget.h"

#include "ComponentRegister.h"
#include "ObjectFinder.h"

//====================================================//
// 前方宣言
//====================================================//

class SceneManager;
class UpdatePipeline;

class Renderer;
class CameraBase;
class Canvas;

class ObjectManager;
class UIManager;

//====================================================//
// クラス宣言
//====================================================//
class Scene
{
private:

	// シーンマネージャーへのポインタ
	SceneManager* m_pSceneManager;

	// マネージャー管理をするクラス
	std::unique_ptr<UpdatePipeline> m_updatePipeline;

	// コンポーネント登録システム
	std::unique_ptr<ComponentRegister> m_componentRegister;

	// デフォルトで使用する描画ターゲット
	std::unique_ptr<RenderTarget> m_defaultRenderTarget;

	// オブジェクト検索
	std::unique_ptr<ObjectFinder> m_objectFinder;

	// メインスクリーンの描画設定
	bool m_drawMainScreen;	// 描画するかどうか

	DirectX::SimpleMath::Vector2 m_startPoint;	// 始点	
	DirectX::SimpleMath::Vector2 m_scale;		// 拡大率

	// 再生フラグ
	bool m_play;

public:

	// コンストラクタ
	Scene(SceneManager* pSceneManager);

	// デストラクタ
	virtual ~Scene();

	// 初期化処理
	void BaseInitialize();

	// 更新処理
	void BaseUpdate(const GameTimer& gameTimer);

	// 描画処理
	void BaseRender(Renderer& renderer);

	// スクリーン本体への描画処理
	void BaseRenderOnScreen(Renderer& renderer);

	// RenderContext指定
	void RenderWithContext(const RenderContext& context, Renderer& renderer);

	// 終了処理
	void BaseFinalize();

	// 全オブジェクトの削除関数
	void ResetObjects();

public:

	// オブジェクトを生成する関数
	GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });
	// キャンバスを生成する関数
	Canvas* GenerateCanvas();

	// コンポーネントを登録する関数
	void RegisterComponent(ComponentBase* component);

	// コンポーネントの登録を解除する関数
	void UnRegsisterComponent(ComponentBase* component);

	// RayCast関数
	bool RayCast(Ray& ray, float max, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// メインカメラを取得する関数
	CameraBase* GetMainCamera() const;

	// メインの描画ターゲットを取得する関数
	const RenderTarget* GetMainRenderTarget() const { return m_defaultRenderTarget.get(); }

	// 二大管理クラスの取得
	ObjectManager* GetObjectManager() const;
	UIManager* GetUIManager() const;

	// オブジェクトファインダー
	const ObjectFinder* GetObjectFinder() const { return m_objectFinder.get(); }

	// メインスクリーンの位置を反映したマウス位置
	DirectX::SimpleMath::Vector2 GetMousePointOnMainScreen();

	// スクリーン設定
	DirectX::SimpleMath::Vector2 GetMainScreenStartPoint() const { return m_startPoint; }
	DirectX::SimpleMath::Vector2 GetMainScreenScale() const { return m_scale; }

	// 全コンポーネントを取得する
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
	std::vector<ComponentBase*>& GetAllComponents() const { return m_componentRegister->GetAllComponents<T>(); }

	// 1コンポーネントを取得する
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
	ComponentBase* GetComponent() const { return m_componentRegister->GetComponent<T>(); }

	// カテゴリ指定
	std::vector<ComponentBase*>& GetAllComponentsWithCategory(ComponentCategory c) const { return m_componentRegister->GetAllComponentsWithCategory(c); }

	// 1コンポーネントを取得する
	ComponentBase* GetComponentWithCategory(ComponentCategory category) const { return m_componentRegister->GetComponentWithCategory(category); }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	
	// メインカメラをセットする関数
	void SetMainCamera(CameraBase* camera);

	// 描画フラグ
	void SetDrawMainScreen(bool f) { m_drawMainScreen = f; }

	// 再生フラグ
	void SetPlayFlag(bool f) { m_play = f; }

	void SetMainScreenStartPoint(DirectX::SimpleMath::Vector2 p) { m_startPoint = p; };
	void SetMainScreenScale(DirectX::SimpleMath::Vector2 p) { m_scale = p; };

protected:

	// シーンの変更
	void ChangeScene(const std::string& nextSceneName, std::unique_ptr<Transition::Base> outTrans, std::unique_ptr<Transition::Base> inTrans);
	void ChangeScene(const std::string& nextSceneName);

private:

	// 派生クラスへ通知する関数
	virtual void Initialize() = 0;
	virtual void Update(const GameTimer& gameTimer) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void RenderOnScreen(Renderer& renderer) {};
	virtual void Finalize() = 0;

	virtual void RegisterComponentOnDerived([[maybe_unused]] ComponentBase* component) {};
	virtual void UnRegisterComponentOnDerived([[maybe_unused]] ComponentBase* component) {};
};

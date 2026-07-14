//====================================================//
// ファイル名   : Canvas.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要 : キャンバスクラス
//
// 更新履歴 :
// 2026/05/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameObject/GameObject.h"
#include "Common/PropertyObject.h"
#include "Components/UI/RectTransform/RectTransform.h"

//====================================================//
// 前方宣言
//====================================================//
class UIManager;
class Scene;

//====================================================//
// クラス宣言
//====================================================//
class Canvas : public PropertyObject
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// アクティブフラグ
	bool m_isActive;

	// 削除フラグ
	bool m_destroy;

	// 名前
	std::string m_canvasName;

	// 描画順
	int m_drawOrder;

	// 所有するUIManagerへのポインタ
	UIManager* m_pUIManager;   

	// 所属するシーンへのポインタ
	Scene* m_pScene;

	// ルートオブジェクト
	std::unique_ptr<GameObject> m_rootObject;

	// 生成予約中のオブジェクトリスト
	std::vector<std::unique_ptr<GameObject>> m_reservations;

	// 保持するUIObjectのリスト
	std::vector<std::unique_ptr<GameObject>> m_uiObjects;

	// コンポーネントリストを保持する用の変数
	std::vector<ComponentBase*> m_components;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	Canvas(UIManager* uiManager);
	~Canvas() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Update(const GameTimer& gameTimer, bool playing);
	void LateUpdate(const GameTimer& gameTimer, bool playing);

	void Draw(Renderer& renderer);

	void RemoveDeadComponent();

	void Finalize();

	// 生成関数
	GameObject* Generate();

	// 削除関数
	void Remove(GameObject* obj);

	// 点と衝突するRectTransformを返す関数
	RectTransform* HitTest(const DirectX::SimpleMath::Vector2& point) { return HitTestChild(m_rootObject->GetComponent<RectTransform>(), point); }

	// クリック時関数
	void OnMouseDown();
	void OnMouseUp();

	// Destroyのオブジェクトを削除する関数
	void RemoveReserves()
	{
		// Deadフラグがオンのオブジェクトを削除
		std::erase_if(
			m_uiObjects,
			[](const std::unique_ptr<GameObject>& object)
			{
				// フラグがオンなら
				if (object->IsDead())
				{
					// 終了処理
					object->Finalize();

					// true
					return true;
				}
				return false;
			}
		);
	}

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	int GetDrawOrder() const { return m_drawOrder; }

	bool IsActive() const { return m_isActive; }

	bool IsDestroy() const { return m_destroy; }

	// 全オブジェクト
	const std::vector<std::unique_ptr<GameObject>>& GetAllObjects() const { return m_uiObjects; }
	const std::vector<std::unique_ptr<GameObject>>& GetAllReserves() const { return m_reservations; }

	// ルートオブジェクト
	GameObject* GetRootObject() const { return m_rootObject.get(); }

	// キャンバス名
	const std::string GetName() const { return m_canvasName; }
	
	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetDrawOrder(int order);

	void SetIsActive(bool f) { m_isActive = f; }

	void Destroy() { m_destroy = true; }

	// キャンバス名
	void SetCanvasName(const std::string& name) { m_canvasName = name; }

private:
	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 再帰的に描画を行う関数
	void DrawChild(RectTransform* child, Renderer& renderer);

	// 再帰的にマウスとの衝突を判定する関数
	RectTransform* HitTestChild(RectTransform* child, const DirectX::SimpleMath::Vector2& point);
	
	// 押されたことを再帰的に通知する関数
	void MouseCheckChild(RectTransform* child, bool dowm);

	// 予約リストから生成予約されたオブジェクトを登録する関数
	void AddReserves()
	{
		// 予約リストにあるオブジェクトを全て登録
		for (auto& reservation : m_reservations) m_uiObjects.push_back(std::move(reservation));

		// 予約リストをクリア
		m_reservations.clear();
	}

	// 全オブジェクトの削除フラグを立てる関数
	void AllDestroy()
	{
		// 管理リストを全て調べる
		for (auto& obj : m_uiObjects) obj->Destroy(); 
	}
};

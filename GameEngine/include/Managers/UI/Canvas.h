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
#include "Components/UI/RectTransform/RectTransform.h"

//====================================================//
// 前方宣言
//====================================================//
class UIManager;
class Scene;

//====================================================//
// クラス宣言
//====================================================//
class Canvas
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// アクティブフラグ
	bool m_isActive;

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

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	Canvas(UIManager* uiManager);
	~Canvas() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Update(const GameTimer& gameTimer);
	void LateUpdate(const GameTimer& gameTimer);

	void Draw(Renderer& renderer);

	void RemoveDeadComponent();

	void Finalize();

	// 生成関数
	GameObject* Generate();

	// 削除関数
	void Remove(GameObject* obj);

	// 点と衝突するかどうか
	RectTransform* HitTest(const DirectX::SimpleMath::Vector2& point);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	int GetDrawOrder() const { return m_drawOrder; }

	bool IsActive() const { return m_isActive; }

	// 全オブジェクト
	const std::vector<std::unique_ptr<GameObject>>& GetAllObjects() const { return m_uiObjects; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetDrawOrder(int order);

	void SetIsActive(bool f) { m_isActive = f; }

	
	// Destroyのオブジェクトを削除する関数
	void RemoveReserves()
	{
		// 管理リストを全て調べる
		for (int i = 0; i < m_uiObjects.size(); i++)
		{
			GameObject* obj = m_uiObjects[i].get();

			// オブジェクトの死亡フラグがオンなら
			if (obj->IsDead())
			{
				obj->BaseFinalize();

				// リストから削除
				m_uiObjects.erase(m_uiObjects.begin() + i);

				i--;
			}
		}
	}

private:
	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	void DrawChild(RectTransform* child, Renderer& renderer);

	RectTransform* HitTestChild(RectTransform* child, const DirectX::SimpleMath::Vector2& point);

	// 予約リストから生成予約されたオブジェクトを登録する関数
	void AddReserves()
	{
		// 予約リストにあるオブジェクトを全て登録
		for (auto& reservation : m_reservations)
		{
			m_uiObjects.push_back(std::move(reservation));
		}
		m_reservations.clear();
	}

	// 全オブジェクトの削除フラグを立てる関数
	void AllDestroy()
	{
		// 管理リストを全て調べる
		for (auto& obj : m_uiObjects)
		{
			obj->Destroy();
		}
	}
};

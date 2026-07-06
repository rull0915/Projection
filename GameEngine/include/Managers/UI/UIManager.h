//====================================================//
// ファイル名   : UIManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/24
//
// 概要 : UI管理クラス
// 　　   Canvasを保持する
//
// 更新履歴 :
// 2026/05/24 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>
#include <vector>

#include "Managers/UI/Canvas.h"

//====================================================//
// 前方宣言
//====================================================//
class Renderer;
class Scene;

//====================================================//
// クラス宣言
//====================================================//
class UIManager
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 保持するキャンバスのリスト
	std::vector<std::unique_ptr<Canvas>> m_reserveCanvases;
	std::vector<std::unique_ptr<Canvas>> m_canvases;

	bool m_needSort; // キャンバスの描画順を再ソートする必要があるか

	// シーンへのポインタ
	Scene* m_pScene;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	UIManager(Scene* pScene);
	~UIManager();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Update(const GameTimer& gameTimer);
	void LateUpdate(const GameTimer& gameTimer);

	void Draw(Renderer& renderer);

	void Finalize();

	// マウスとの衝突を調べる関数
	void CheckHitRay(DirectX::SimpleMath::Vector2 position);

	// キャンバスを生成する関数
	Canvas* CreateCanvas();

	// キャンバスからオブジェクトの削除を行う関数
	void RemoveObjects();

	// デバッグ描画関数
	void DebugDraw(Renderer& renderer, DirectX::SimpleMath::Color color);

	// リセット
	void Reset()
	{
		m_reserveCanvases.clear();
		m_canvases.clear();

		m_needSort = true;
	}

	// 全キャンバスを取得する関数
	const std::vector<std::unique_ptr<Canvas>>& GetAllCanvas() const { return m_canvases; }

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// シーンポインタ
	Scene* GetScene() const { return m_pScene; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetNeedSort(bool f) { m_needSort = f; }

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// キャンバスの並び順をソートする関数
	void SortCanvas();

	// 予約済みキャンバスを登録する関数
	void RegisterReserveCanvases();
};

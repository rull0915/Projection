//====================================================//
// ファイル名   : NavigationGraphBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/24
//
// 概要 : 敵が移動できる有向グラフを作成する基底クラス
//
// 更新履歴 :
// 2026/06/24 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//


//====================================================//
// 前方宣言
//====================================================//
class Renderer;

//====================================================//
// クラス宣言
//====================================================//
class NavigationGraphBase
{
public:
	// グラフの辺
	struct Edge
	{
		// 開始ノードのインデックス
		size_t ownIndex;
		// ゴールノードのインデックス
		size_t goalIndex;

		// スタートの足場の点インデックス
		size_t startPoint;
		// ゴールの足場の点インデックス
		size_t goalPoint;

		// コスト(移動時間)
		float cost;

		// ==演算子オーバーロード
		bool operator==(const Edge& o)
		{   
			return (ownIndex == o.ownIndex && goalIndex == o.goalIndex);
		}
	};

private:


	// 敵のパラメータ
	float m_jumpImpulse;  // 扱うジャンプ力  
	float m_enemyMass;  // 扱う質量
	float m_horizontalVelocity; // 扱う水平方向速度

	// 計算済みの値をキャッシュする変数
	float m_maxHeight;  // 届く最大値

	float m_gravity;    // 重力

	// グラフ本体
	std::vector<std::vector<Edge>> m_graph;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	NavigationGraphBase(float jumpImpulse, float mass, float horizontalVelocity);

	virtual ~NavigationGraphBase() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------
	
	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// デバッグ描画
	virtual void DebugDraw(Renderer& renderer) = 0;

	virtual void DebugDraw(const std::vector<Edge>& edges, Renderer& renderer, int color = 0x00FF00) = 0;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// 派生クラスのみが読み取り可能な変数
protected:

	float GetJumpImpluse() const { return m_jumpImpulse; } 
	float GetEnemyMass() const { return m_enemyMass; }
	float GetHorizontalVelocity() const { return m_horizontalVelocity; }

	float GetMaxHeight() const { return m_maxHeight; }
	float GetGravity() const { return m_gravity; }

	std::vector<std::vector<Edge>>& GetGraph() { return m_graph; }


	// 外部からも読み取り可能な変数
public:
	// グラフを取得する関数
	const std::vector<std::vector<Edge>>& GetGraph() const { return m_graph; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

protected:
	void ResetGraph()
	{
		m_graph.clear();
	}

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// グラフを再構築する関数
	virtual void InitializeGraph() = 0;

	// グラフを更新する関数
	virtual void UpdateGraph() = 0;

	// 候補点同士
	virtual void BuildConnection(size_t first, size_t second) = 0;
};
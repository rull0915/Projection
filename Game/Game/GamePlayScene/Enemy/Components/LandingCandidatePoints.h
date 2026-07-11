//====================================================//
// ファイル名   : LandingCandidatePoints.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要 : 着地候補点コンポーネント
//       敵AIに使用する着地候補点を示すコンポーネントです
//
// 更新履歴 :
// 2026/06/14 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(LandingCandidatePoints)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Collider/3D/ColliderBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class LandingCandidatePoints : public WorldComponentBase
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 着地候補点
	std::vector<DirectX::SimpleMath::Vector3> m_candidatePoints;

	// 中心点
	DirectX::SimpleMath::Vector3 m_centerPoint;

	// 自身と対応するコライダー
	ColliderBase* m_ownCollider;

	// バージョン
	uint32_t m_latestVersion;

	// 変更済みフラグ
	bool m_isChanged;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	LandingCandidatePoints(IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_candidatePoints(0)
		, m_centerPoint{}
		, m_ownCollider{ nullptr }
		, m_latestVersion{ 0 }
		, m_isChanged{ true }
	{
	}

	~LandingCandidatePoints() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;

	void Start() override;

	void Update(const GameTimer& gameTimer) override;

	ColliderBase* GetOwnCollider() const { return m_ownCollider; }

	// 候補点をすべて取得する関数
	const std::vector<DirectX::SimpleMath::Vector3>& GetPoints() const { return m_candidatePoints; }

	// 中心座標を返す関数
	const DirectX::SimpleMath::Vector3& GetCenterPoint() const { return m_centerPoint; }

	// フラグリセット
	void ResetChangedFlag() { m_isChanged = false; }

	// フラグを返す
	bool IsChanged() const { return m_isChanged; }

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<LandingCandidatePoints>();
	}

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 候補点の座標を作成する関数
	void UpdateCandidatePoints();

	// ボックスコライダーの場合
	void UpdateCandidatePointsOnBox();

};

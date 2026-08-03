//====================================================//
// ファイル名   : PathMover.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : パスを使用し移動するコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(PathMover)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "CatmullRomComponent.h"
#include "Components/World/Transform/Transform.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class PathMover : public REngine::WorldComponentBase
{
public:
	enum class RotType
	{
		None,
		Forward,
		Rigit,
		Left
	};

private:
	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 移動する倍率
	float m_magnification;

	// 開始時に移動開始するかどうか
	bool m_playOnStart;

	// 移動中フラグ
	bool m_isPlaying;

	// 合計経過時間
	float m_sumTime;

	// トランスフォーム
	REngine::Transform* m_pTransform;

	// 補間コンポーネント
	CatmullRomComponent* m_pCatmull;

	// 回転の補正方法
	RotType m_rotateType;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PathMover(REngine::IComponentOwner* own);
	~PathMover() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;
	void LateUpdate(const REngine::GameTimer& gameTimer) override;

	// 移動開始関数
	void MoveStart();

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return REngine::TypeIDGenerator::GetID<PathMover>();
	}
};

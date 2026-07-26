//====================================================//
// ファイル名   : EasingTestComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要 : イージングのテストをするコンポーネント
//
// 更新履歴 :
// 2026/07/21 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(EasingTestComponent)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

#include "EasingConfig.h"
#include "Components/Interface/IDebugRenderable.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class EasingTestComponent : public REngine::WorldComponentBase, public REngine::IDebugRenderable
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// トランスフォームポインタ
	REngine::Transform* m_pTransform;

	// 開始点
	DirectX::SimpleMath::Vector3 m_start;

	// 終了点
	DirectX::SimpleMath::Vector3 m_end;

	// 再生倍率
	float m_playScale;

	// イージング情報
	EasingConfig m_easingConfig;

	// 経過時間の合計
	float m_sumTime;

	// ループフラグ
	bool m_loop;

	// 次のループまでの間隔
	float m_loopInterval;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	EasingTestComponent(REngine::IComponentOwner* own);
	~EasingTestComponent() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void OnEnable() override;

	void Update(const REngine::GameTimer& gameTimer) override;

	// デバッグ描画関数
	void DebugRender(REngine::Renderer& renderer, const DirectX::SimpleMath::Color& color) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return REngine::TypeIDGenerator::GetID<EasingTestComponent>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};

//====================================================//
// ファイル名   : CatmullRomComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : Catmull–Romスプライン補間をするコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "Components/Interface/IDebugRenderable.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class CatmullRomComponent : public WorldComponentBase, public IDebugRenderable
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ループフラグ
	bool m_isLoop;

	// オフセット
	DirectX::SimpleMath::Vector3 m_offset;

	// 各点
	DirectX::SimpleMath::Vector3 m_p0;
	DirectX::SimpleMath::Vector3 m_p1;
	DirectX::SimpleMath::Vector3 m_p2;
	DirectX::SimpleMath::Vector3 m_p3;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	CatmullRomComponent(IComponentOwner* own);
	~CatmullRomComponent() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const GameTimer& gameTimer) override;
	void LateUpdate(const GameTimer& gameTimer) override;

	void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<CatmullRomComponent>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};

//====================================================//
// ファイル名   : MaterialTester.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/06
//
// 概要 : マテリアルをテストするコンポーネント
//
// 更新履歴 :
// 2026/08/06 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(MaterialTester)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Renderer/RendererBase.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/MaterialAsset.h"
#include "Components/World/Transform/Transform.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class MaterialTester : public REngine::RendererBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// トランスフォームコンポーネント
	REngine::Transform* m_pTransform;

	// マテリアル
	REngine::Handle<REngine::MaterialAsset> m_material;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	MaterialTester(REngine::IComponentOwner* own);
	~MaterialTester() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;
	void LateUpdate(const REngine::GameTimer& gameTimer) override;

	void Draw(REngine::Renderer& renderer) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return REngine::TypeIDGenerator::GetID<MaterialTester>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};

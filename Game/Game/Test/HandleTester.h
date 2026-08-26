//====================================================//
// ファイル名   : HandleTester.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/27
//
// 概要 : ハンドルのテストをするコンポーネント
//
// 更新履歴 :
// 2026/07/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(HandleTester)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Assets/Types/Texture.h"
#include "Assets/Objects/Handle.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class HandleTester : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// テクスチャハンドル
	REngine::Handle<REngine::Texture> m_textureHandle;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	HandleTester(REngine::IComponentOwner* own);
	~HandleTester() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------

	COMPONENT_TYPE(HandleTester, REngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;
	void LateUpdate(const REngine::GameTimer& gameTimer) override;
};

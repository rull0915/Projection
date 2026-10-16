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
#include "Components/Both/BothComponentBase.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Components/World/Transform/Transform.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Types/Shader/SamplerType.h"

//====================================================//
// 前方宣言
//====================================================//
class RHEngine::AssetManager;

//====================================================//
// クラス宣言
//====================================================//
class MaterialTester : public RHEngine::BothComponentBase, public RHEngine::IAssetDependent
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// トランスフォームコンポーネント
	RHEngine::Transform* m_pTransform;

	// マテリアル
	RHEngine::Handle<RHEngine::MaterialAsset> m_material;

	// AssetManager
	RHEngine::AssetManager* m_assetManager;

	// テクスチャハンドル
	RHEngine::Handle<RHEngine::Texture> m_texture;

	// サンプラータイプ
	RHEngine::SamplerType m_samplerType;

	// 経過時間
	float m_sumTime;

	// 乗算カラー
	DirectX::SimpleMath::Color m_mulColor;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	MaterialTester(RHEngine::IComponentOwner* own);
	~MaterialTester() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(MaterialTester, RHEngine::BothComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	void OnValidate() override;

	void ReceiveAssetManager(RHEngine::AssetManager& am) override
	{
		m_assetManager = &am;
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------
	
	// バッファの設定を行う関数
	void SetBuffer();
};

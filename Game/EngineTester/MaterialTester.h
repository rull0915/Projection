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
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Types/Shader/SamplerType.h"

//====================================================//
// 前方宣言
//====================================================//
class REngine::AssetManager;

//====================================================//
// クラス宣言
//====================================================//
class MaterialTester : public REngine::RendererBase, public REngine::IAssetDependent
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// トランスフォームコンポーネント
	REngine::Transform* m_pTransform;

	// マテリアル
	REngine::Handle<REngine::MaterialAsset> m_material;

	// AssetManager
	REngine::AssetManager* m_assetManager;

	// テクスチャハンドル
	REngine::Handle<REngine::Texture> m_texture;

	// サンプラータイプ
	REngine::SamplerType m_samplerType;

	// 経過時間
	float m_sumTime;

	// 乗算カラー
	DirectX::SimpleMath::Color m_mulColor;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	MaterialTester(REngine::IComponentOwner* own);
	~MaterialTester() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(MaterialTester, REngine::RendererBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;
	void LateUpdate(const REngine::GameTimer& gameTimer) override;

	void Draw(REngine::Renderer& renderer) override;

	void OnValidate() override;

	void ReceiveAssetManager(REngine::AssetManager& am) override
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

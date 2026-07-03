//====================================================//
// ファイル名   : SkyboxComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : スカイボックスコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Renderer/RendererBase.h"
#include <GeometricPrimitive.h>

//====================================================//
// 前方宣言
//====================================================//
class SkyboxEffect;

//====================================================//
// クラス宣言
//====================================================//
class SkyboxComponent : public RendererBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	std::unique_ptr<DirectX::GeometricPrimitive> m_sky;
	std::unique_ptr<SkyboxEffect> m_effect;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyInputLayout;

	std::string m_keyName;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	SkyboxComponent(IComponentOwner* own);
	~SkyboxComponent() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;

	void Start() override;

	void Draw(Renderer& renderer) override;

	void Load(const std::string& key);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<SkyboxComponent>();
	}
};

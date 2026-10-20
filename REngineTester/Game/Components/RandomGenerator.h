//====================================================//
// ファイル名   : RandomGenerator.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/19
//
// 概要 : ランダムにPrefabを生成するコンポーネント
//
// 更新履歴 :
// 2026/10/19 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(RandomGenerator)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Assets/Types/Prefab.h"

//====================================================//
// クラス宣言
//====================================================//
class RandomGenerator : public RHEngine::WorldComponentBase
{
public:
	enum RandomType
	{
		Position	= 0b001,
		Rotation	= 0b010,
		Scale		= 0b100,
		PosRot		= Position | Rotation,
		PosSca		= Position | Scale,
		RotSca		= Rotation | Scale,
		All			= Position | Rotation | Scale
	};

private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 生成対象のPrefab
	RHEngine::Handle<RHEngine::Prefab> m_prefab;

	// 生成範囲
	DirectX::SimpleMath::Vector3 m_min;
	DirectX::SimpleMath::Vector3 m_max;

	// 生成方法
	RandomType m_generateType;

	// 生成個数
	int m_generateCount;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	RandomGenerator(RHEngine::IComponentOwner* own);
	~RandomGenerator() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(RandomGenerator, RHEngine::WorldComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};

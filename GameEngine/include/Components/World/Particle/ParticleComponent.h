//====================================================//
// ファイル名   : ParticleComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/23
//
// 概要 : パーティクルを管理するコンポーネント
//
// 更新履歴 :
// 2026/10/23 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(ParticleComponent)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"

namespace RHEngine
{
	struct Particle
	{
		DirectX::SimpleMath::Vector3 position;	// 位置
		DirectX::SimpleMath::Vector3 velocity;	// 速度

		float life;		// 生存時間
		float maxLife;	// 寿命

		DirectX::SimpleMath::Vector2 size;	// 大きさ
		float rotation;						// 回転

		DirectX::SimpleMath::Color color;	// 色
	};

	struct ParticleSystem : public PropertyObject
	{
		uint32_t maxParticles = 1000;

		float emissionRate = 10.0f;
		float lifetime = 2.0f;

		float startSpeed = 1.0f;
		float startSize = 1.0f;

		DirectX::SimpleMath::Color startColor;
	};

	class ParticleComponent : public WorldComponentBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------



	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ParticleComponent(IComponentOwner* own);
		~ParticleComponent() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(ParticleComponent, WorldComponentBase);

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Start() override;

		void Update(const GameTimer& gameTimer) override;

	};
}

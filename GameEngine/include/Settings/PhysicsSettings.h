//====================================================//
// ファイル名   : PhysicsSettings.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/01
//
// 概要 : 物理関係の設定項目
//
// 更新履歴 :
// 2026/06/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <SimpleMath.h>
#include <array>

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class PhysicsSettings
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------
	static constexpr int LAYER_MAX = 64;

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 重力の大きさ
	float gravityScale;

	// 重力の向き
	DirectX::SimpleMath::Vector3 gravityDirection;
	DirectX::SimpleMath::Vector2 gravityDirection2D;

	// 各レイヤーが衝突するか
	std::array<std::array<bool, LAYER_MAX>, LAYER_MAX> hitLayer;

private:
	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PhysicsSettings()
		: gravityScale{ 30.0f }
		, gravityDirection{ 0, -1.0f, 0 }
		, gravityDirection2D{ 0, -1.0f }
		, hitLayer{ true }
	{
		for (auto& line : hitLayer)
		{
			for (auto& cell : line)
			{
				cell = true;
			}
		}
	}
	~PhysicsSettings() = default;

public:
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	static PhysicsSettings& Instance()
	{
		static PhysicsSettings instance;
		return instance;
	}

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------
	inline bool CanHit(int layerA, int layerB) const
	{
		if (layerA < 0 || layerA >= LAYER_MAX) return false;
		if (layerB < 0 || layerB >= LAYER_MAX) return false;

		return hitLayer[layerA][layerB];
	}
	inline float GetGravityScale() const { return gravityScale; }
	inline const DirectX::SimpleMath::Vector3& GetGravityDirection() const { return gravityDirection; }
	inline const DirectX::SimpleMath::Vector2& GetGravityDirection2D() const { return gravityDirection2D; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	inline void LayerSetting(int layerA, int layerB, bool condition)
	{
		if (layerA < 0 || layerA >= LAYER_MAX) return;
		if (layerB < 0 || layerB >= LAYER_MAX) return;

		hitLayer[layerA][layerB] = condition;
		hitLayer[layerB][layerA] = condition;
	}
	inline void SetGravityScale(float value) { gravityScale = value; }
	inline void SetGravityDirection(const DirectX::SimpleMath::Vector3& direction)
	{
		if (direction.LengthSquared() > FLT_EPSILON)
		{
			gravityDirection = direction;
			gravityDirection.Normalize();
		}
	}

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};

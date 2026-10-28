/**
 * @file    PhysicsSettings.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/01
 *
 * @brief   物理関係の設定項目
 */

#pragma once

#include <SimpleMath.h>
#include <array>
#include <cfloat>

namespace RHEngine
{
	/**
	 * @brief 物理関係の設定項目
	 */
	class PhysicsSettings
	{
	private:

		/** @brief レイヤーの最大値 */
		static constexpr int LAYER_MAX = 64;

		/** @brief グラビティのスケール */
		float gravityScale;

		/** @brief 重力の向き */
		DirectX::SimpleMath::Vector3 gravityDirection;

		/** @brief 2Dでの重力の向き */
		DirectX::SimpleMath::Vector2 gravityDirection2D;

		/** @brief 各レイヤーの衝突可否 */
		std::array<std::array<bool, LAYER_MAX>, LAYER_MAX> hitLayer;

	private:

		/// @name ライフサイクル
		/// @{

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

		/// @}

	public:
		
		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static PhysicsSettings& Instance()
		{
			static PhysicsSettings instance;
			return instance;
		}

		/**
		 * @brief 衝突するな組み合わせかどうか
		 * @param layerA レイヤー番号
		 * @param layerB もう一方のレイヤー番号
		 * @return 衝突するかどうか
		 */
		inline bool CanHit(int layerA, int layerB) const
		{
			if (layerA < 0 || layerA >= LAYER_MAX) return false;
			if (layerB < 0 || layerB >= LAYER_MAX) return false;

			return hitLayer[layerA][layerB];
		}

		/**
		 * @brief 組み合わせが衝突するかどうかをセットする
		 * @param layerA レイヤー番号
		 * @param layerB もう一方のレイヤー番号
		 * @param condition 衝突するかどうか
		 */
		inline void LayerSetting(int layerA, int layerB, bool condition)
		{
			if (layerA < 0 || layerA >= LAYER_MAX) return;
			if (layerB < 0 || layerB >= LAYER_MAX) return;

			hitLayer[layerA][layerB] = condition;
			hitLayer[layerB][layerA] = condition;
		}

		/// @name ゲッター
		/// @{

		/** @brief 重力倍率 */
		inline float GetGravityScale() const { return gravityScale; }

		/** @brief 重力ベクトル */
		inline const DirectX::SimpleMath::Vector3& GetGravityDirection() const { return gravityDirection; }

		/** @brief 2Dでの重力ベクトル */
		inline const DirectX::SimpleMath::Vector2& GetGravityDirection2D() const { return gravityDirection2D; }

		/// @}

		/// @name ライフサイクル
		/// @{

		/** @brief 重力倍率 */
		inline void SetGravityScale(float value) { gravityScale = value; }

		/** @brief 重力ベクトル */
		inline void SetGravityDirection(const DirectX::SimpleMath::Vector3& direction)
		{
			if (direction.LengthSquared() > FLT_EPSILON)
			{
				gravityDirection = direction;
				gravityDirection.Normalize();
			}
		}

		/** @brief 2Dでの重力ベクトル */
		inline void SetGravityDirection2D(const DirectX::SimpleMath::Vector3& direction)
		{
			if (direction.LengthSquared() > FLT_EPSILON)
			{
				gravityDirection = direction;
				gravityDirection.Normalize();
			}
		}
	};
}	// namespace RHEngine

/**
 * @file    WorldSetting2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/01
 *
 * @brief   2次元世界の設定
 */

#pragma once

#include <SimpleMath.h>

namespace RHEngine
{
	/**
	 * @brief 2D世界の設定
	 */
	class WorldSetting2D
	{
	private:

		/** @brief X軸ベクトルの向き */
		DirectX::SimpleMath::Vector3 m_xAxis;

		/** @brief Y軸ベクトルの向き */
		DirectX::SimpleMath::Vector3 m_yAxis;

		/** @brief 変更フラグ */
		bool m_isDirty;

	private:

		/// @name ライフサイクル
		/// @{

		WorldSetting2D()
			: m_xAxis{ DirectX::SimpleMath::Vector3::UnitX }
			, m_yAxis{ DirectX::SimpleMath::Vector3::UnitY }
			, m_isDirty{ true }
		{}

		~WorldSetting2D() = default;

		/// @}

	public:

		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static WorldSetting2D& Instance()
		{
			static WorldSetting2D instance;
			return instance;
		}

		/**
		 * @brief 3次元上の点を2D空間へ変換する
		 * @param point 対象の3次元座標
		 * @return 変換後の2次元座標
		 */
		DirectX::SimpleMath::Vector2 World3DToLocal2D(const DirectX::SimpleMath::Vector3& point)
		{
			return
			{
				point.Dot(m_xAxis),
				point.Dot(m_yAxis)
			};
		}

		/**
		 * @brief 2次元座標の点を3次元ワールド座標へ変換する
		 * @param point 対象の2次元座標
		 * @return 変換後の3次元座標
		 */
		DirectX::SimpleMath::Vector3 Local2DToWorld3D(const DirectX::SimpleMath::Vector2& point)
		{
			return
			{
				point.x * m_xAxis.x + point.y * m_yAxis.x,
				point.x * m_xAxis.y + point.y * m_yAxis.y,
				point.x * m_xAxis.z + point.y * m_yAxis.z
			};
		}

		/// @name ゲッター
		/// @{

		/** @brief X軸ベクトルの向き */
		const DirectX::SimpleMath::Vector3& GetXAxis() const { return m_xAxis; }

		/** @brief Y軸ベクトルの向き */
		const DirectX::SimpleMath::Vector3& GetYAxis() const { return m_yAxis; }

		/** @brief 2D世界平面の法線 */
		const DirectX::SimpleMath::Vector3 GetNormal() const { return m_xAxis.Cross(m_yAxis); }

		/** @brief 軸が変更されたかどうか */
		bool IsDirty() const { return m_isDirty; }

		/// @}

		/// @name セッター
		/// @{

		/**
		 * @brief 各軸を設定する
		 * @param xAxis X軸ベクトル
		 * @param yAxis Y軸ベクトル
		 */
		void SetAxis(DirectX::SimpleMath::Vector3 xAxis, DirectX::SimpleMath::Vector3 yAxis)
		{
			// x軸を正規化
			xAxis.Normalize();

			// 3次元直交座標系になるようy軸を変更
			yAxis -= xAxis * xAxis.Dot(yAxis);

			// 同方向だった時
			if (yAxis.LengthSquared() < 0.00001f)
			{
				yAxis =
					std::abs(xAxis.y) < 0.99f
					? DirectX::SimpleMath::Vector3::Up
					: DirectX::SimpleMath::Vector3::Right;

				yAxis -= xAxis * yAxis.Dot(xAxis);
			}

			yAxis.Normalize();

			m_xAxis = xAxis;
			m_yAxis = yAxis;

			m_isDirty = true;
		}

		void ResetDirty() { m_isDirty = false; }
	};
}	// namespace RHEngine

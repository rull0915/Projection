//====================================================//
// ファイル名   : WorldSetting2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/01
//
// 概要 : 2次元世界の設定
//
// 更新履歴 :
// 2026/06/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <SimpleMath.h>

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class WorldSetting2D
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 軸とするベクトル
		DirectX::SimpleMath::Vector3 m_xAxis, m_yAxis;

		// 変更フラグ
		bool m_isDirty;

	private:
		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		WorldSetting2D()
			: m_xAxis{ DirectX::SimpleMath::Vector3::UnitX }
			, m_yAxis{ DirectX::SimpleMath::Vector3::UnitY }
			, m_isDirty{ true }
		{}

		~WorldSetting2D() = default;

	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// インスタンスを取得する関数
		static WorldSetting2D& Instance()
		{
			static WorldSetting2D instance;
			return instance;
		}

		// 3次元の点を2次元ローカル座標系に変換する関数
		DirectX::SimpleMath::Vector2 World3DToLocal2D(const DirectX::SimpleMath::Vector3& point)
		{
			return
			{
				point.Dot(m_xAxis),
				point.Dot(m_yAxis)
			};
		}
		// ローカル2次元の点を3次元ワールド座標系に変換する関数
		DirectX::SimpleMath::Vector3 Local2DToWorld3D(const DirectX::SimpleMath::Vector2& point)
		{
			return
			{
				point.x * m_xAxis.x + point.y * m_yAxis.x,
				point.x * m_xAxis.y + point.y * m_yAxis.y,
				point.x * m_xAxis.z + point.y * m_yAxis.z
			};
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Vector3& GetXAxis() const { return m_xAxis; }
		const DirectX::SimpleMath::Vector3& GetYAxis() const { return m_yAxis; }
		const DirectX::SimpleMath::Vector3 GetNormal() const { return m_xAxis.Cross(m_yAxis); }

		bool IsDirty() const { return m_isDirty; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// 各軸を設定する関数
		void SetAxis(DirectX::SimpleMath::Vector3 xAxis, DirectX::SimpleMath::Vector3 yAxis)
		{
			// x軸を設定
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
}	// namespace REngine

//====================================================//
// ファイル名   : Ray.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/30
//
// 概要 : レイクラス
//
// 更新履歴 :
// 2026/05/30 新規作成
//====================================================//

#pragma once

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Ray
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		DirectX::SimpleMath::Vector3 m_start;
		DirectX::SimpleMath::Vector3 m_direction;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Ray(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 direction)
			: m_start{ start }
			, m_direction{ direction }
		{
			m_direction.Normalize();
		}
		~Ray() = default;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Vector3& GetStart() const
		{
			return m_start;
		}
		const DirectX::SimpleMath::Vector3& GetDirection() const
		{
			return m_direction;
		}

		const DirectX::SimpleMath::Vector3 GetMin(float maxDistance) const
		{
			return DirectX::SimpleMath::Vector3{
				m_direction.x > 0 ? m_start.x : m_start.x + m_direction.x * maxDistance,
				m_direction.y > 0 ? m_start.y : m_start.y + m_direction.y * maxDistance,
				m_direction.z > 0 ? m_start.z : m_start.z + m_direction.z * maxDistance
			};
		}
		const DirectX::SimpleMath::Vector3 GetMax(float maxDistance) const
		{
			return DirectX::SimpleMath::Vector3{
				m_direction.x < 0 ? m_start.x : m_start.x + m_direction.x * maxDistance,
				m_direction.y < 0 ? m_start.y : m_start.y + m_direction.y * maxDistance,
				m_direction.z < 0 ? m_start.z : m_start.z + m_direction.z * maxDistance
			};
		}
	};
}	// namespace REngine

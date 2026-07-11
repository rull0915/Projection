//====================================================//
// ファイル名  : SlideTransition.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要       : フェードシーン遷移
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "SlideTransition.h"

#include "Renderer/Renderer.h"
#include "System/WindowManager.h"

#include "GameLib/GameMath/GameMath.h"
#include "Math/Easing.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace Transition
{
	void Slide::Initialize()
	{
		// 0~2PIに補正
		while (m_slideAngle < 0) m_slideAngle += 2 * PI_F;
		while (m_slideAngle > 2 * PI_F) m_slideAngle -= 2 * PI_F;

		// 角度から始点がどの端か求める
		End end = Slide::End::LeftUp;

		if (m_slideAngle >= 0 && m_slideAngle < PI_F / 2) end = End::LeftUp;
		if (m_slideAngle >= PI_F / 2 && m_slideAngle < PI_F) end = End::RightUp;
		if (m_slideAngle >= PI_F && m_slideAngle < PI_F * (3.f / 2)) end = End::RightDown;
		if (m_slideAngle >= PI_F * (3.f / 2) && m_slideAngle < PI_F * 2) end = End::LeftDown;

		// 角度からベクトルを算出
		DirectX::SimpleMath::Vector2 stVec = DirectX::SimpleMath::Vector2{ cosf(m_slideAngle), sinf(m_slideAngle) } * 100;
		DirectX::SimpleMath::Vector2 vrVec = DirectX::SimpleMath::Vector2{ -sinf(m_slideAngle), cosf(m_slideAngle) } * 100;

		// 各点の座標
		DirectX::SimpleMath::Vector2 RU = DirectX::SimpleMath::Vector2{ WindowManager::Instance().GetWidthF(), 0 };
		DirectX::SimpleMath::Vector2 RD = DirectX::SimpleMath::Vector2{ WindowManager::Instance().GetWidthF(), WindowManager::Instance().GetHeightF() };
		DirectX::SimpleMath::Vector2 LU = DirectX::SimpleMath::Vector2{ 0, 0 };
		DirectX::SimpleMath::Vector2 LD = DirectX::SimpleMath::Vector2{ 0, WindowManager::Instance().GetHeightF() };

		// 始点によって分岐
		switch (end)
		{
			// 右上始まりの場合
		case Slide::End::RightUp:

			// 右上からvrVec方向の直線と、右下からstVec方向の直線の交点を求める
			m_points[0] = MyMath::GetIntersection(MyMath::Line{ RU, RU + vrVec }, MyMath::Line{ RD, RD + stVec });

			// 右上からvrVec方向の直線と、左上からstVec方向の直線の交点を求める
			m_points[1] = MyMath::GetIntersection(MyMath::Line{ RU, RU + vrVec }, MyMath::Line{ LU, LU + stVec });

			// 左下からvrVec方向の直線と、右下からstVec方向の直線の交点を求める
			m_points[2] = MyMath::GetIntersection(MyMath::Line{ LD, LD + vrVec }, MyMath::Line{ RD, RD + stVec });

			// 左下からvrVec方向の直線と、左上からstVec方向の直線の交点を求める
			m_points[3] = MyMath::GetIntersection(MyMath::Line{ LD, LD + vrVec }, MyMath::Line{ LU, LU + stVec });

			break;
		case Slide::End::RightDown:

			m_points[0] = MyMath::GetIntersection(MyMath::Line{ RD, RD + vrVec }, MyMath::Line{ LD, LD + stVec });
			m_points[1] = MyMath::GetIntersection(MyMath::Line{ RD, RD + vrVec }, MyMath::Line{ RU, RU + stVec });
			m_points[2] = MyMath::GetIntersection(MyMath::Line{ LU, LU + vrVec }, MyMath::Line{ LD, LD + stVec });
			m_points[3] = MyMath::GetIntersection(MyMath::Line{ LU, LU + vrVec }, MyMath::Line{ RU, RU + stVec });

			break;
		case Slide::End::LeftUp:

			m_points[0] = MyMath::GetIntersection(MyMath::Line{ LU, LU + vrVec }, MyMath::Line{ RU, RU + stVec });
			m_points[1] = MyMath::GetIntersection(MyMath::Line{ LU, LU + vrVec }, MyMath::Line{ LD, LD + stVec });
			m_points[2] = MyMath::GetIntersection(MyMath::Line{ RD, RD + vrVec }, MyMath::Line{ RU, RU + stVec });
			m_points[3] = MyMath::GetIntersection(MyMath::Line{ RD, RD + vrVec }, MyMath::Line{ LD, LD + stVec });

			break;
		case Slide::End::LeftDown:

			m_points[0] = MyMath::GetIntersection(MyMath::Line{ LD, LD + vrVec }, MyMath::Line{ LU, LU + stVec });
			m_points[1] = MyMath::GetIntersection(MyMath::Line{ LD, LD + vrVec }, MyMath::Line{ RD, RD + stVec });
			m_points[2] = MyMath::GetIntersection(MyMath::Line{ RU, RU + vrVec }, MyMath::Line{ LU, LU + stVec });
			m_points[3] = MyMath::GetIntersection(MyMath::Line{ RU, RU + vrVec }, MyMath::Line{ RD, RD + stVec });

			break;
		default:
			break;
		}
	}

	bool Slide::InUpdate(const GameTimer& gameTimer)
	{
		// 経過時間の加算
		m_elapsedTime += gameTimer.GetElapsedTime();

		// 補正
		if (m_elapsedTime >= m_transSec) m_elapsedTime = m_transSec;

		// 遷移時間を超えたかどうか
		return (m_elapsedTime >= m_transSec);
	}

	bool Slide::OutUpdate(const GameTimer& gameTimer)
	{
		// 経過時間の加算
		m_elapsedTime += gameTimer.GetElapsedTime();

		// 補正
		if (m_elapsedTime >= m_transSec) m_elapsedTime = m_transSec;

		// 遷移時間を超えたかどうか
		return (m_elapsedTime >= m_transSec);
	}

	void Slide::InRender(Renderer& renderer)
	{
		// 4点を決める
		DirectX::SimpleMath::Vector2 endLeft;
		DirectX::SimpleMath::Vector2 endRight;

		endLeft = m_points[0] + (DirectX::SimpleMath::Vector2(m_points[2] - m_points[0])) * Easing::EaseInQuad(1 - m_elapsedTime / m_transSec);
		endRight = m_points[1] + (DirectX::SimpleMath::Vector2(m_points[3] - m_points[1])) * Easing::EaseInQuad(1 - m_elapsedTime / m_transSec);

		renderer.Draw().UI().Rect(
			m_points[0],
			m_points[1],
			endRight,
			endLeft,
			m_slideColor, true
		);
	}

	void Slide::OutRender(Renderer& renderer)
	{
		// 4点を決める
		DirectX::SimpleMath::Vector2 endLeft;
		DirectX::SimpleMath::Vector2 endRight;

		endLeft = m_points[0] + (DirectX::SimpleMath::Vector2(m_points[2] - m_points[0])) * Easing::EaseOutQuad(m_elapsedTime / m_transSec);
		endRight = m_points[1] + (DirectX::SimpleMath::Vector2(m_points[3] - m_points[1])) * Easing::EaseOutQuad(m_elapsedTime / m_transSec);

		renderer.Draw().UI().Rect(
			m_points[0],
			m_points[1],
			endRight,
			endLeft,
			m_slideColor, true
		);
	}
}

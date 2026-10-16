//====================================================//
// ファイル名   : EasingConfig.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要 : イージングコンフィグ
//
// 更新履歴 :
// 2026/07/21 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <algorithm>
#include "Common/Property/PropertyObject.h"
#include "Math/Easing.h"

/// <summary>
/// イージングの種類を宣言した列挙型
/// </summary>
enum class EasingKind
{
	Linear,
	Quad,
	Cubic,
	Quart,
	Quint,
	Sine,
	Circ,
	Back,
	Bounce,
	Elastic,
};

/// <summary>
/// イージングのタイプを宣言した列挙型
/// </summary>
enum class EasingType
{
	In,
	Out,
	InOut,
};

// イージングをまとめたクラス
class EasingConfig : public RHEngine::PropertyObject
{
private:
	EasingKind m_kind;
	EasingType m_type;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EasingConfig()
		: m_kind{ EasingKind::Linear }
		, m_type{ EasingType::In }
	{
		ADD_PROPERTY(EasingConfig, m_kind);
		ADD_PROPERTY(EasingConfig, m_type);
	}

	/// <summary>
	/// 割合に応じたイージング後の値を返す関数
	/// </summary>
	float Get(float ratio)
	{
		// 0~1にクランプ
		float t = std::clamp(ratio, 0.0f, 1.0f);

		switch (m_type)
		{
		case EasingType::In:

			switch (m_kind)
			{
			case EasingKind::Linear:
				return RHEngine::Easing::Linear(t);
			case EasingKind::Quad:
				return RHEngine::Easing::EaseInQuad(t);
			case EasingKind::Cubic:
				return RHEngine::Easing::EaseInCubic(t);
			case EasingKind::Quart:
				return RHEngine::Easing::EaseInQuart(t);
			case EasingKind::Quint:
				return RHEngine::Easing::EaseInQuint(t);
			case EasingKind::Sine:
				return RHEngine::Easing::EaseInSine(t);
			case EasingKind::Circ:
				return RHEngine::Easing::EaseInCirc(t);
			case EasingKind::Back:
				return RHEngine::Easing::EaseInBack(t);
			case EasingKind::Bounce:
				return RHEngine::Easing::EaseInBounce(t);
			case EasingKind::Elastic:
				return RHEngine::Easing::EaseInElastic(t);
			default:
				return t;
			}

			break;
		case EasingType::Out:

			switch (m_kind)
			{
			case EasingKind::Linear:
				return RHEngine::Easing::Linear(t);
			case EasingKind::Quad:
				return RHEngine::Easing::EaseOutQuad(t);
			case EasingKind::Cubic:
				return RHEngine::Easing::EaseOutCubic(t);
			case EasingKind::Quart:
				return RHEngine::Easing::EaseOutQuart(t);
			case EasingKind::Quint:
				return RHEngine::Easing::EaseOutQuint(t);
			case EasingKind::Sine:
				return RHEngine::Easing::EaseOutSine(t);
			case EasingKind::Circ:
				return RHEngine::Easing::EaseOutCirc(t);
			case EasingKind::Back:
				return RHEngine::Easing::EaseOutBack(t);
			case EasingKind::Bounce:
				return RHEngine::Easing::EaseOutBounce(t);
			case EasingKind::Elastic:
				return RHEngine::Easing::EaseOutElastic(t);
			default:
				return t;
			}

			break;
		case EasingType::InOut:

			switch (m_kind)
			{
			case EasingKind::Linear:
				return RHEngine::Easing::Linear(t);
			case EasingKind::Quad:
				return RHEngine::Easing::EaseInOutQuad(t);
			case EasingKind::Cubic:
				return RHEngine::Easing::EaseInOutCubic(t);
			case EasingKind::Quart:
				return RHEngine::Easing::EaseInOutQuart(t);
			case EasingKind::Quint:
				return RHEngine::Easing::EaseInOutQuint(t);
			case EasingKind::Sine:
				return RHEngine::Easing::EaseInOutSine(t);
			case EasingKind::Circ:
				return RHEngine::Easing::EaseInOutCirc(t);
			case EasingKind::Back:
				return RHEngine::Easing::EaseInOutBack(t);
			case EasingKind::Bounce:
				return RHEngine::Easing::EaseInOutBounce(t);
			case EasingKind::Elastic:
				return RHEngine::Easing::EaseInOutElastic(t);
			default:
				return t;
			}

			break;
		default:
			break;
		}

		return 0;
	}
};

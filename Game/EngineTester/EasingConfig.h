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
class EasingConfig : public REngine::PropertyObject
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
		ADD_PROPERTY(m_kind);
		ADD_PROPERTY(m_type);
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
				return REngine::Easing::Linear(t);
			case EasingKind::Quad:
				return REngine::Easing::EaseInQuad(t);
			case EasingKind::Cubic:
				return REngine::Easing::EaseInCubic(t);
			case EasingKind::Quart:
				return REngine::Easing::EaseInQuart(t);
			case EasingKind::Quint:
				return REngine::Easing::EaseInQuint(t);
			case EasingKind::Sine:
				return REngine::Easing::EaseInSine(t);
			case EasingKind::Circ:
				return REngine::Easing::EaseInCirc(t);
			case EasingKind::Back:
				return REngine::Easing::EaseInBack(t);
			case EasingKind::Bounce:
				return REngine::Easing::EaseInBounce(t);
			case EasingKind::Elastic:
				return REngine::Easing::EaseInElastic(t);
			default:
				return t;
			}

			break;
		case EasingType::Out:

			switch (m_kind)
			{
			case EasingKind::Linear:
				return REngine::Easing::Linear(t);
			case EasingKind::Quad:
				return REngine::Easing::EaseOutQuad(t);
			case EasingKind::Cubic:
				return REngine::Easing::EaseOutCubic(t);
			case EasingKind::Quart:
				return REngine::Easing::EaseOutQuart(t);
			case EasingKind::Quint:
				return REngine::Easing::EaseOutQuint(t);
			case EasingKind::Sine:
				return REngine::Easing::EaseOutSine(t);
			case EasingKind::Circ:
				return REngine::Easing::EaseOutCirc(t);
			case EasingKind::Back:
				return REngine::Easing::EaseOutBack(t);
			case EasingKind::Bounce:
				return REngine::Easing::EaseOutBounce(t);
			case EasingKind::Elastic:
				return REngine::Easing::EaseOutElastic(t);
			default:
				return t;
			}

			break;
		case EasingType::InOut:

			switch (m_kind)
			{
			case EasingKind::Linear:
				return REngine::Easing::Linear(t);
			case EasingKind::Quad:
				return REngine::Easing::EaseInOutQuad(t);
			case EasingKind::Cubic:
				return REngine::Easing::EaseInOutCubic(t);
			case EasingKind::Quart:
				return REngine::Easing::EaseInOutQuart(t);
			case EasingKind::Quint:
				return REngine::Easing::EaseInOutQuint(t);
			case EasingKind::Sine:
				return REngine::Easing::EaseInOutSine(t);
			case EasingKind::Circ:
				return REngine::Easing::EaseInOutCirc(t);
			case EasingKind::Back:
				return REngine::Easing::EaseInOutBack(t);
			case EasingKind::Bounce:
				return REngine::Easing::EaseInOutBounce(t);
			case EasingKind::Elastic:
				return REngine::Easing::EaseInOutElastic(t);
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

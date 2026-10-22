//====================================================//
// ファイル名  : CanvasScaler.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/22
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CanvasScaler.h"

#include "System/WindowManager.h"

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	CanvasScaler::CanvasScaler(IComponentOwner* own)
		: UIComponentBase(own)
		, m_pRect{ nullptr }
		, m_mode{ CanvasMode::None }
		, m_scaleFactor{ 1.0f }
		, m_referenceResolution{ 1280.0f, 720.0f }
		, m_screenMatchMode{ ScreenMatchMode::MatchWidthOrHeight }
		, m_matchWidthOrHeight{ 0.0f }
	{
		ADD_PROPERTY(CanvasScaler, m_mode);

		// 列挙型をレジスタに登録
		EnumRegistry::Instance().Register<ScreenMatchMode>();
	}

	// 最初のUpdate関数の直線に一度呼ばれます
	void CanvasScaler::Start()
	{}

	// 毎フレーム呼ばれます
	void CanvasScaler::Update(const GameTimer& gameTimer)
	{}

	std::vector<Property> CanvasScaler::GetProperties()
	{
		// 既存のプロパティを取得
		std::vector<Property> properties = PropertyObject::GetProperties();

		// プロパティを追加するラムダ式
		auto addProperty = [&properties](
				const std::string& name,
				PropertyType type,
				std::type_index typeindex,
				void* value
			)
			{
				properties.push_back({ name, type, value, typeindex });
			};

		// モードで分岐
		switch (m_mode)
		{
			// ロード時用に全プロパティを追加
		case RHEngine::CanvasScaler::CanvasMode::None:

			addProperty("m_scaleFactor", PropertyType::Float, std::type_index(typeid(float)), &m_scaleFactor);
			addProperty("m_referenceResolution", PropertyType::Vector2, std::type_index(typeid(DirectX::SimpleMath::Vector2)), &m_referenceResolution);
			addProperty("m_screenMatchMode", PropertyType::Enum, std::type_index(typeid(ScreenMatchMode)), &m_screenMatchMode);
			addProperty("m_matchWidthOrHeight", PropertyType::Float, std::type_index(typeid(float)), &m_matchWidthOrHeight);

			break;
		case RHEngine::CanvasScaler::CanvasMode::ConstantPixelSize: 

			// プロパティを追加
			addProperty("m_scaleFactor", PropertyType::Float, std::type_index(typeid(float)), &m_scaleFactor);
			break;

		case RHEngine::CanvasScaler::CanvasMode::ScaleWithScreenSize: {

			// プロパティを追加
			addProperty("m_referenceResolution", PropertyType::Vector2, std::type_index(typeid(DirectX::SimpleMath::Vector2)), &m_referenceResolution);
			addProperty("m_screenMatchMode", PropertyType::Enum, std::type_index(typeid(ScreenMatchMode)), &m_screenMatchMode);
			if (m_screenMatchMode == ScreenMatchMode::MatchWidthOrHeight)
			{
				addProperty("m_matchWidthOrHeight", PropertyType::Float, std::type_index(typeid(float)), &m_matchWidthOrHeight);
			}

			break;
		}
		default:
			break;
		}
		return properties;
	}

	void CanvasScaler::CorrectRectTransform()
	{
		// RectTransformがなかった場合取得
		if (!m_pRect) m_pRect = GetComponent<RectTransform>();

		// 取得もできなかった場合処理しない
		if (!m_pRect) return;

		// ルートじゃない場合対応しない
		if (m_pRect->GetParent()) return;

		// スケールの計算
		float scale = CalcScale();

		// WindowSizeを取得
		DirectX::SimpleMath::Vector2 windowSize = WindowManager::Instance().GetSize();

		// RectTransformに反映
		m_pRect->SetScale({ scale, scale });
		m_pRect->SetSize(windowSize / scale);
	}

	void CanvasScaler::ResetRectTransform()
	{
		// RectTransformがなかった場合取得
		if (!m_pRect) m_pRect = GetComponent<RectTransform>();

		// 取得もできなかった場合処理しない
		if (!m_pRect) return;

		// ルートじゃない場合対応しない
		if (m_pRect->GetParent()) return;

		DirectX::SimpleMath::Vector2 windowSize = WindowManager::Instance().GetSize();

		// RectTransformに反映
		m_pRect->SetScale({ 1.0f, 1.0f });
		m_pRect->SetSize(windowSize);
	}

	float CanvasScaler::CalcScale()
	{
		// WindowSizeを取得
		DirectX::SimpleMath::Vector2 windowSize = WindowManager::Instance().GetSize();

		// モードで処理を変更
		switch (m_mode)
		{
			// ピクセルサイズ固定の場合
		case RHEngine::CanvasScaler::CanvasMode::ConstantPixelSize:

			// 倍率はそのまま
			return m_scaleFactor;

			// 基準解像度使用の場合
		case RHEngine::CanvasScaler::CanvasMode::ScaleWithScreenSize: {

			// 横幅の倍率を取得
			const float widthScale =
				windowSize.x / m_referenceResolution.x;

			// 高さの倍率を取得
			const float heightScale =
				windowSize.y / m_referenceResolution.y;

			// モードで分岐
			switch (m_screenMatchMode)
			{
			case RHEngine::CanvasScaler::ScreenMatchMode::MatchWidthOrHeight:

				return std::expf((1.0f - m_matchWidthOrHeight) * std::logf(widthScale) + m_matchWidthOrHeight * std::logf(heightScale));

			case RHEngine::CanvasScaler::ScreenMatchMode::Expand:

				return std::min(widthScale, heightScale);

			case RHEngine::CanvasScaler::ScreenMatchMode::Shrink:

				return std::max(widthScale, heightScale);

			default:
				break;
			}

			break;
		}
		default:
			break;
		}

		return 1.0f;
	}
}

//====================================================//
// ファイル名   : LightComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要 : 
//
// 更新履歴 :
// 2026/10/11 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(LightComponent)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"

namespace RHEngine
{
	enum class LightType
	{
		None,
		Point,
		Spot,
		Directional,
	};

	//====================================================//
	// クラス宣言
	//====================================================//
	class LightComponent : public WorldComponentBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ライトの種類
		LightType m_lightType;

		// ------ 共通設定 ------ //
		
		// 色
		DirectX::SimpleMath::Color m_color;

		// 強さ
		float m_intensity;

		// ------ ポイントライト設定 ------ //

		// 半径
		float m_radius;
		
		// ------ スポットライト設定 ------ //

		// 届く距離
		float m_length;

		// 光の減衰が始まる角度
		float m_innerAngle;

		// 最大の角度
		float m_outerAngle;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		LightComponent(IComponentOwner* own);
		~LightComponent();

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(LightComponent, WorldComponentBase);

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		std::vector<Property> GetProperties() override;

		void OnValidate() override;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ライトタイプ
		LightType GetLightType() const { return m_lightType; }

		// 色
		DirectX::SimpleMath::Color GetColor() const { return m_color; }

		// 強さ
		float GetIntensity() const { return m_intensity; }

		// 半径
		float GetRadius() const { return m_radius; }

		// 届く距離
		float GetLength() const { return m_length; }

		// 光の減衰が始まる角度
		float GetInnerAngle() const { return m_innerAngle; }

		// 最大の角度
		float GetOuterAngle() const { return m_outerAngle; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// ライトタイプ
		void SetLightType(LightType type) { m_lightType = type; }

		// 色
		void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }

		// 強さ
		void SetIntensity(float intensity) { m_intensity = intensity; }

		// 半径
		void SetRadius(float radius) { m_radius = radius; }

		// 届く距離
		void SetLength(float length) { m_length = length; }

		// 光の減衰が始まる角度
		void SetInnerAngle(float innerAngle) { m_innerAngle = innerAngle; }

		// 最大の角度
		void SetOuterAngle(float outerAngle) { m_outerAngle = outerAngle; }
	};
}

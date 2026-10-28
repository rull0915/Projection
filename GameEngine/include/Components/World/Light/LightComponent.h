/**
 * @file    LightComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/11
 *
 * @brief   ライトコンポーネント
 */

#pragma once

#include <vector>
#include <SimpleMath.h>

#include "Components/World/WorldComponentBase.h"

namespace RHEngine
{
	class IComponentOwner;
	struct Property;

	/**
	 * @brief ライトのタイプ
	 */
	enum class LightType
	{
		None,
		Point,
		Spot,
		Directional,
	};

	class LightComponent : public WorldComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

	private:

		/** @brief ライトの種類 */
		LightType m_lightType;

		/// @name 共通設定
		/// @{

		/** @brief ライトの色 */
		DirectX::SimpleMath::Color m_color;

		/** @brief ライトの強さ */
		float m_intensity;

		/// @}

		/// @name ポイントライト
		/// @{

		/** @brief 照らす半径 */
		float m_radius;

		/// @}

		/// @name スポットライト
		/// @{

		/** @brief 光の届く距離 */
		float m_length;

		/** @brief 減衰が始まる角度 */
		float m_innerAngle;

		/** @brief 光が届く最大の角度 */
		float m_outerAngle;

		/// @}

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		LightComponent(IComponentOwner* own);

		~LightComponent();

		/// @}

		/**
		 * @brief プロパティリストを取得する
		 * @details ライトタイプに応じてシリアライズする変数を変える
		 * @return 生成したプロパティリスト
		 */
		std::vector<Property> GetProperties() override;

		/**
		 * @brief GUI上で値が変更された際に制限を適用する
		 */
		void OnValidate() override;

		/// @name ゲッター
		/// @{

		/** @brief ライトのタイプ */
		LightType GetLightType() const { return m_lightType; }

		/** @brief ライトの色 */
		DirectX::SimpleMath::Color GetColor() const { return m_color; }

		/** @brief ライトの強さ */
		float GetIntensity() const { return m_intensity; }

		/** @brief ポイントライト: 半径 */
		float GetRadius() const { return m_radius; }

		/** @brief スポットライト: 一刈が届く距離 */
		float GetLength() const { return m_length; }

		/** @brief スポットライト: 減衰が始まる角度 */
		float GetInnerAngle() const { return m_innerAngle; }

		/** @brief スポットライト: 光が届く最大の角度 */
		float GetOuterAngle() const { return m_outerAngle; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief ライトのタイプ */
		void SetLightType(LightType type) { m_lightType = type; }

		/** @brief ライトの色 */
		void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }

		/** @brief ライトの強さ */
		void SetIntensity(float intensity) { m_intensity = intensity; }

		/** @brief ポイントライト: ライトの半径 */
		void SetRadius(float radius) { m_radius = radius; }

		/** @brief スポットライト: 光が届く距離 */
		void SetLength(float length) { m_length = length; }

		/** @brief スポットライト: 光の減衰が始まる角度 */
		void SetInnerAngle(float innerAngle) { m_innerAngle = innerAngle; }

		/** @brief スポットライト: 光が届く最大の角度 */
		void SetOuterAngle(float outerAngle) { m_outerAngle = outerAngle; }

		/// @}
	};
}	// namespace RHEngine

/**
 * @file    CatmullRomComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   Catmull–Romスプライン補間をするコンポーネント
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "Components/Interface/IDebugRenderable.h"

/**
 * @brief Catmull-Romスプライン補間
 */
class CatmullRomComponent : public RHEngine::WorldComponentBase, public RHEngine::IDebugRenderable
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	// ループフラグ
	bool m_isLoop;

	// オフセット
	DirectX::SimpleMath::Vector3 m_offset;

	// 各点
	DirectX::SimpleMath::Vector3 m_p0;
	DirectX::SimpleMath::Vector3 m_p1;
	DirectX::SimpleMath::Vector3 m_p2;
	DirectX::SimpleMath::Vector3 m_p3;

public:

	CatmullRomComponent(RHEngine::IComponentOwner* own);
	~CatmullRomComponent() = default;

	void DebugRender(RHEngine::Renderer& renderer, const DirectX::SimpleMath::Color& color);

	// 値を取得する関数
	DirectX::SimpleMath::Vector3 GetValue(float ratio);

	// 接線ベクトルを返す関数
	DirectX::SimpleMath::Vector3 GetTangent(float ratio);
};

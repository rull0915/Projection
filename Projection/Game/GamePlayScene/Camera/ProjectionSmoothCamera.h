/**
 * @file    ProjectionSmoothCamera.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/29
 *
 * @brief   プロジェクション行列を滑らかに切り替えるカメラ
 */
#pragma once

#include "Components/World/Camera/CameraBase.h"

/**
 * @brief プロジェクション行列をPerspectiveとOrthographicの間で滑らかに切り替える
 */
class ProjectionSmoothCamera : public RHEngine::CameraBase
{
public:
	DECLARE_COMPONENT_TYPE(RHEngine::CameraBase);

private:

	/** @brief 現在の射影モード */
	RHEngine::ProjectionType m_type;

	/// @name 共通設定
	/// @{

	/** @brief ニアクリップ */
	float m_nearZ;

	/** @brief ファークリップ */
	float m_farZ;

	/// @}

	/// @name Perspective
	/// @{
	
	/** @brief 画角 */
	float m_fov;

	/// @}

	/// @name Orthographic
	/// @{

	/** @brief サイズ */
	float m_size;

	/** 
	 * @brief ニアクリップ
	 * @details オーソグラフィックの際にカメラよりも手前の物を描画するために負のニアクリップを設定する
	 */
	float m_negativeNearClip;

	/// @}

	/** @brief 変化開始前の射影行列 */
	DirectX::SimpleMath::Matrix m_oldProjecition;

	/** @brief 変化後の射影行列 */
	DirectX::SimpleMath::Matrix m_targetProjection;

	/** @brief 変化中かどうか */
	bool m_isChanging;

	/** @brief 変化に何秒かけるか */
	float m_changeTime;

	/** @brief 変化し始めてからの時間 (秒) */
	float m_nowTime;

public:

	ProjectionSmoothCamera(RHEngine::IComponentOwner* owner);
	~ProjectionSmoothCamera() = default;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	void ChangeProjectionMode(float changeTime = 1.0f);

	RHEngine::ProjectionType GetProjectionType() const { return m_type; }

	void UpdateView() override;
	void UpdateProj() override;

	bool IsChanging() const { return m_isChanging; }
};

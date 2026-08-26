//====================================================//
// ファイル名   : ProjectionSmoothCamera.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要 : プロジェクション行列を滑らかに切り替えるカメラ
//
// 更新履歴 :
// 2026/05/29 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(ProjectionSmoothCamera)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Camera/CameraBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ProjectionSmoothCamera : public REngine::CameraBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// projection行列のパラメータ
	REngine::ProjectionType m_type;

	// 共通
	float m_nearZ;  // ニアクリップ
	float m_farZ;   // ファークリップ
	float m_aspect; // アスペクト比

	// Perspective
	float m_fov;    // 画角

	// Orthographic
	float m_size;

	// 変化前のProjection
	DirectX::SimpleMath::Matrix m_oldProjecition;

	// 変化先のProjection
	DirectX::SimpleMath::Matrix m_targetProjection;

	// 変化中フラグ
	bool m_isChanging;

	// 何秒で変化し終わるか
	float m_changeTime;

	// 変化し始めて何秒か
	float m_nowTime;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ProjectionSmoothCamera(REngine::IComponentOwner* owner);
	~ProjectionSmoothCamera() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(ProjectionSmoothCamera, REngine::CameraBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Update(const REngine::GameTimer& gameTimer) override;

	void ChangeProjectionMode(float changeTime = 1.0f);

	REngine::ProjectionType GetProjectionType() const { return m_type; }

	void UpdateView() override;
	void UpdateProj() override;

	bool IsChanging() const { return m_isChanging; }
};

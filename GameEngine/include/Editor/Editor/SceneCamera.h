//====================================================//
// ファイル名   : SceneCamera.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : シーンビューのカメラコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

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
class SceneCamera : public CameraBase
{
private:

	// 動くかどうか
	bool m_movable;

	// projection行列のパラメータ

	float m_nearZ;  // ニアクリップ
	float m_farZ;   // ファークリップ
	float m_aspect; // アスペクト比

	float m_fov;    // 画角

	// 注視点の位置
	DirectX::SimpleMath::Vector3 m_targetPoint;

	// クォータニオン
	DirectX::SimpleMath::Quaternion m_rot;

	// 半径
	float m_radius;

	// 回転
	float m_yaw = 0;
	float m_pitch = 0;

	// マウスの移動量に対するカメラ移動の割合
	float m_moveRatio;

	// マウスの移動量に対するカメラの回転の割合
	float m_rotateRatio;

	// ホイールの移動量に対する拡大の割合
	float m_scaleRatio;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	SceneCamera(IComponentOwner* own);
	~SceneCamera() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const GameTimer& gameTimer) override;
	void LateUpdate(const GameTimer& gameTimer) override;

	void UpdateView() override;
	void UpdateProj() override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<SceneCamera>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetMovable(bool f) { m_movable = f; }

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};

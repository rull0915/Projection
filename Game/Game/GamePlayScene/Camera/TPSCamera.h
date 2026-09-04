//====================================================//
// ファイル名   : TPSCamera.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/07
//
// 概要 : カメラをTPS用に動かすコンポーネント
//
// 更新履歴 :
// 2026/06/07 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(TPSCamera)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class TPSCamera : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ターゲットの名前
	std::string m_targetName;

	// 感度
	float m_sensitivity;

	float m_distance;

	REngine::Transform* m_pOwnTransform;
	REngine::Transform* m_pTargetTransform;

	DirectX::SimpleMath::Vector2 m_angle;

	// 位置
	DirectX::SimpleMath::Vector3 m_position;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	TPSCamera(REngine::IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_targetName{ "" }
		, m_pOwnTransform{ nullptr }
		, m_pTargetTransform{ nullptr }
		, m_angle{ 0.0f, 0.0f }
		, m_sensitivity{ 0.5f }
		, m_distance{ 5.0f }
		, m_position{ 0, 0, 0 }
	{
		ADD_PROPERTY(TPSCamera, m_targetName);
		ADD_PROPERTY(TPSCamera, m_sensitivity);
		ADD_PROPERTY(TPSCamera, m_distance);
	}

	~TPSCamera() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(TPSCamera, REngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;

	void Start() override;

	void OnEnable() override;
	void OnDisable() override;

	void Update(const REngine::GameTimer& gameTimer) override;

	void SetTarget(const std::string& name);

	// 理想位置
	DirectX::SimpleMath::Vector3 GetIdealPosition() const { return m_position; }

	// 回転
	DirectX::SimpleMath::Vector2 GetRotation() const { return m_angle; }
};

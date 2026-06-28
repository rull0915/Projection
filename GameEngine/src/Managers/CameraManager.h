//====================================================//
// ファイル名   : CameraManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : カメラ管理クラス
//
// 更新履歴 :
// 2026/04/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Camera/CameraBase.h"
#include "GameObject/GameObject.h"

//====================================================//
// 前方宣言
//====================================================//
class Scene;

//====================================================//
// クラス宣言
//====================================================//
class CameraManager
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// メインカメラがないときに使用するカメラ
	GameObject* m_spareCamera;

	// 使用しているメインカメラ
	CameraBase* m_mainCamera;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
public:
	CameraManager();
	~CameraManager();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Initialize(Scene* pScene);

	void Update();

	// メインカメラを設定する関数
	void SetMainCamera(CameraBase* camera)
	{
		m_mainCamera = camera;
	}
	void UnSetMainCamera(CameraBase* camera)
	{
		if (camera == m_mainCamera) m_mainCamera = nullptr;
	}

	// メインカメラを取得する関数
	CameraBase* GetMainCamera()
	{
		return (!m_mainCamera || !m_mainCamera->IsActive()) ? m_spareCamera->GetComponent<CameraBase>() : m_mainCamera;
	}

	// メインカメラの行列の取得関数
	const DirectX::SimpleMath::Matrix& GetView() { return GetMainCamera()->GetView(); }
	const DirectX::SimpleMath::Matrix& GetProj() { return GetMainCamera()->GetProj(); }
	const DirectX::SimpleMath::Matrix& GetInverseView() { return GetMainCamera()->GetInverseView(); }
	const DirectX::SimpleMath::Matrix& GetInverseProj() { return GetMainCamera()->GetInverseProj(); }

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};
//====================================================//
// ファイル名  : CameraManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要        :  カメラ管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CameraManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

CameraManager::CameraManager()
	: m_mainCamera{ nullptr }
{
	// 予備カメラの作成
	m_spareCamera.AddComponent<StandardCamera>();
	m_spareCamera.GetComponent<Transform>()->SetLocalPosition({ 0, 0, 10 });
}

CameraManager::~CameraManager()
{
}

void CameraManager::Update()
{
	BaseCamera* mainCamera = GetMainCamera();

	if (mainCamera->IsNeedUpdateProj())
	{
		mainCamera->UpdateProj();
	}

	mainCamera->UpdateView();
}

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

#include "Components/World/Camera/Derived/StandardCamera.h"
#include "Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

CameraManager::CameraManager()
	: m_mainCamera{ nullptr }
	, m_spareCamera{ nullptr }
{

}

CameraManager::~CameraManager()
{
}

void CameraManager::Initialize(Scene* pScene)
{
	// 予備カメラの作成
	m_spareCamera = pScene->Generate({ 0, 0, 10 });
	m_spareCamera->AddComponent<StandardCamera>();
	m_spareCamera->SetInvincible(true);
}

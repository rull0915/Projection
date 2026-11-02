/**
 * @file    CameraManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/17
 */

#include "pch.h"
#include "CameraManager.h"

namespace RHEngine
{
	CameraManager::CameraManager()
	{
	}

	CameraManager::~CameraManager()
	{}

	CameraBase* CameraManager::GetMainCamera()
	{
		auto& cameras = GetObjects();

		// 追加逆順に調べる
		for (auto it = cameras.rbegin(); it != cameras.rend(); ++it)
		{
			// レンダーターゲットが設定されていなければ
			if (!(*it)->GetRenderTarget())
			{
				// メインカメラとして返す
				return (*it);
			}
		}

		return nullptr;
	}
}	// namespace RHEngine

//====================================================//
// ファイル名  : ObjectFinder.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/07
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/ObjectFinder.h"

#include "Scene/Scene.h"
#include "Managers/ObjectManager.h"
#include "Managers/UI/UIManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

GameObject* ObjectFinder::FindWithNameInWorld(const std::string& name) const
{
	// ObjectManagerを取得
	ObjectManager* objManager = m_pScene->GetObjectManager();

	// 全オブジェクトを調べる
	for (auto& object : objManager->GetAllObject())
	{
		// 名前を比較
		if (object->GetName() == name)
		{
			return object.get();
		}
	}
	// 予約リストも調べる
	for (auto& object : objManager->GetAllReserves())
	{
		// 名前を比較
		if (object->GetName() == name)
		{
			return object.get();
		}
	}

	// 見つからなかった場合null
	return nullptr;
}

GameObject* ObjectFinder::FindWithTagInWorld(const std::string& tag) const
{
	// ObjectManagerを取得
	ObjectManager* objManager = m_pScene->GetObjectManager();

	// 全オブジェクトを調べる
	for (auto& object : objManager->GetAllObject())
	{
		// タグを比較
		if (object->GetTag() == tag)
		{
			return object.get();
		}
	}
	// 予約リストも調べる
	for (auto& object : objManager->GetAllReserves())
	{
		// 名前を比較
		if (object->GetTag() == tag)
		{
			return object.get();
		}
	}

	// 見つからなかった場合null
	return nullptr;
}

GameObject* ObjectFinder::FindWithNameInUI(const std::string& name) const
{
	// UIManagerを取得
	UIManager* uiManager = m_pScene->GetUIManager();

	// 全Canvasを調べる
	for (auto& canvas : uiManager->GetAllCanvas())
	{
		// 全オブジェクトを調べる
		for (auto& object : canvas->GetAllObjects())
		{
			// 名前を比較
			if (object->GetName() == name)
			{
				return object.get();
			}
		}
		// 見つからなかった場合予約リストも調べる
		for (auto& object : canvas->GetAllReserves())
		{
			// 名前を比較
			if (object->GetName() == name)
			{
				return object.get();
			}
		}
	}
	// 予約Canvasも調べる
	for (auto& canvas : uiManager->GetAllReserves())
	{
		// 全オブジェクトを調べる
		for (auto& object : canvas->GetAllObjects())
		{
			// 名前を比較
			if (object->GetName() == name)
			{
				return object.get();
			}
		}
		// 見つからなかった場合予約リストも調べる
		for (auto& object : canvas->GetAllReserves())
		{
			// 名前を比較
			if (object->GetName() == name)
			{
				return object.get();
			}
		}
	}

	return nullptr;
}

GameObject* ObjectFinder::FindWithTagInUI(const std::string& tag) const
{
	// UIManagerを取得
	UIManager* uiManager = m_pScene->GetUIManager();

	// 全Canvasを調べる
	for (auto& canvas : uiManager->GetAllCanvas())
	{
		// 全オブジェクトを調べる
		for (auto& object : canvas->GetAllObjects())
		{
			// 名前を比較
			if (object->GetTag() == tag)
			{
				return object.get();
			}
		}
		// 見つからなかった場合予約リストも調べる
		for (auto& object : canvas->GetAllReserves())
		{
			// 名前を比較
			if (object->GetTag() == tag)
			{
				return object.get();
			}
		}
	}
	// 予約Canvasも調べる
	for (auto& canvas : uiManager->GetAllReserves())
	{
		// 全オブジェクトを調べる
		for (auto& object : canvas->GetAllObjects())
		{
			// 名前を比較
			if (object->GetTag() == tag)
			{
				return object.get();
			}
		}
		// 見つからなかった場合予約リストも調べる
		for (auto& object : canvas->GetAllReserves())
		{
			// 名前を比較
			if (object->GetTag() == tag)
			{
				return object.get();
			}
		}
	}

	return nullptr;
}

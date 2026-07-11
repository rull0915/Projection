//====================================================//
// ファイル名   : ObjectSaver.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : コンポーネントをjsonファイルに保存するクラス
//
// 更新履歴 :
// 2026/06/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>
#include <functional>

#include <nlohmann/json.hpp>
#include "Components/ComponentBase.h"

class GameObject;
class Canvas;

class ObjectManager;
class UIManager;

class Scene;

//====================================================//
// クラス宣言
//====================================================//
class ObjectSaver
{
public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ObjectSaver() = default;
	~ObjectSaver() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// Propertyの保存
	static nlohmann::json SaveProperty(const PropertyObject& obj);

	// GameObjectの保存
	static nlohmann::json SaveObject(const GameObject* obj);

	// キャンバスの保存
	static nlohmann::json SaveCanvas(const Canvas* canvas);

	// ObjectManagerの保存
	static nlohmann::json SaveObjectManager(const ObjectManager* objManager);

	// UIManagerの保存
	static nlohmann::json SaveUIManager(const UIManager* uiManager);

	// Sceneの保存
	static nlohmann::json SaveScene(const Scene* scene);

	// 保存関数
	static void SaveObjectToFile(const std::wstring& filePath, GameObject* obj);
	static void SaveSceneToFile(const std::wstring& filePath, Scene* scene);
};

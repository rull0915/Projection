//====================================================//
// ファイル名   : ObjectLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : コンポーネントをjsonファイルから読みとるクラス
//
// 更新履歴 :
// 2026/06/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <nlohmann/json.hpp>
#include "Common/PropertyObject.h"

class GameObject;
class Canvas;
class Scene;
class ObjectManager;
class UIManager;

//====================================================//
// クラス宣言
//====================================================//
class ObjectLoader
{
public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ObjectLoader() = default;
	~ObjectLoader() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// プロパティのロード
	static void LoadProperty(const nlohmann::json& json, PropertyObject& obj);

	// Worldオブジェクトのロード
	static void LoadObject(const nlohmann::json& json, GameObject* obj, Scene* pScene);

	// UIオブジェクトのロード
	static void LoadUIObject(const nlohmann::json& json, GameObject* obj, Canvas* canvas);

	// Canvasのロード
	static void LoadCanvas(const nlohmann::json& json, Canvas* canvas);

	// UIManagerのロード
	static void LoadUIManager(const nlohmann::json& json, UIManager* manager);

	// ObjectManagerのロード
	static void LoadObjectManager(const nlohmann::json& json, Scene* pScene);

	// シーンのロード関数
	static void LoadScene(const nlohmann::json& json, Scene* pScene);

	// ロード関数
	static void LoadFromFile(const std::wstring& filePath, GameObject* obj);

	static void LoadSceneFromFile(const std::wstring& filePath, Scene* scene);
};

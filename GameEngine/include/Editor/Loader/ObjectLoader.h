//====================================================//
// ファイル名   : ObjectLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : オブジェクトをjsonファイルから読みとるクラス
//
// 更新履歴 :
// 2026/06/29 新規作成
// 2026/07/28 静的クラスから通常のクラスに変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <nlohmann/json.hpp>
#include "Common/Property/PropertyObject.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
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
	private:

		// AssetManager
		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ObjectLoader(AssetManager& assetManager)
			: m_assetManager{ assetManager }
		{
		}
		~ObjectLoader() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// プロパティのロード
		void LoadProperty(const nlohmann::json& json, PropertyObject& obj, Scene* pScene);

		// Worldオブジェクトのロード
		void LoadObject(const nlohmann::json& json, GameObject* obj, Scene* pScene);

		// ObjectManagerのロード
		void LoadObjectManager(const nlohmann::json& json, Scene* pScene);

		// シーンのロード関数
		void LoadScene(const nlohmann::json& json, Scene* pScene);

		// ロード関数
		void LoadPropertyFromFile(const std::wstring& filePath, PropertyObject* obj);
		void LoadFromFile(const std::wstring& filePath, GameObject* obj);

		void LoadSceneFromFile(const std::wstring& filePath, Scene* scene);
	};
} // namespace REngine

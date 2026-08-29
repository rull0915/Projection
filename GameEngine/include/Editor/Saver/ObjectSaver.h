//====================================================//
// ファイル名   : ObjectSaver.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : コンポーネントをjsonファイルに保存するクラス
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
#include "Components/ComponentBase.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
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
	private:

		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ObjectSaver(AssetManager& assetManager)
			: m_assetManager{ assetManager }
		{
		}
		~ObjectSaver() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// Propertyの保存
		nlohmann::json SaveProperty(const PropertyObject& obj);

		// GameObjectの保存
		nlohmann::json SaveObject(const GameObject* obj);

		// ObjectManagerの保存
		nlohmann::json SaveObjectManager(const ObjectManager* objManager);

		// Sceneの保存
		nlohmann::json SaveScene(const Scene* scene);

		// 保存関数
		void SaveObjectToFile(const std::wstring& filePath, GameObject* obj);
		void SaveSceneToFile(const std::wstring& filePath, Scene* scene);
		void SavePropertyToFile(const std::wstring& filePath, PropertyObject* scene);
	};
} // namespace REngine

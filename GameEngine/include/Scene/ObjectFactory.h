//====================================================//
// ファイル名   : ObjectFactory.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要 : オブジェクトの生成を担うクラス
//
// 更新履歴 :
// 2026/07/12 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameObject/GameObject.h"
#include "Assets/Types/Prefab.h"
#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	class Scene;
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ObjectFactory
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// シーンポインタ
		Scene* m_pScene;

		// アセットマネージャー
		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ObjectFactory(Scene* pScene, AssetManager& assetManager);
		~ObjectFactory() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// オブジェクトを生成する関数
		GameObject* Generate(DirectX::SimpleMath::Vector3 position = { 0, 0, 0 });

		// プレハブからオブジェクトを生成する関数
		GameObject* Instantiate(Prefab* prefab) const;

		GameObject* Instantiate(Handle<Prefab> prefab) const;

		// UIオブジェクトを生成する関数
		GameObject* GenerateUI();
	};
}	// namespace RHEngine

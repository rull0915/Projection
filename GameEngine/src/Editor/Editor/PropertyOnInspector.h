//====================================================//
// ファイル名   : PropertyOnInspector.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/17
//
// 概要 : プロパティをインスペクターに表示するクラス
//
// 更新履歴 :
// 2026/07/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Common/Property/PropertyObject.h"

namespace REngine
{
	class AssetManager;
	class Scene;

	//====================================================//
	// クラス宣言
	//====================================================//
	class PropertyOnInspector
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// アセット管理クラス
		AssetManager& m_assetManager;

		// 編集中クォータニオンのキャッシュ
		DirectX::SimpleMath::Vector3 m_quaternionCache;
		// 編集中フラグ
		bool m_quaternionEditing;		

		// シーンポインタ
		Scene* m_pScene;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		PropertyOnInspector(Scene* pScene, AssetManager& am)
			: m_assetManager{ am }
			, m_quaternionCache{ DirectX::SimpleMath::Vector3::Zero }
			, m_quaternionEditing{ false }
			, m_pScene{ pScene }
		{
		}

		~PropertyOnInspector() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
		
		// プロパティオブジェクトを表示する関数
		bool DrawPropertyObject(PropertyObject* object);

		// プロパティを一つ表示する関数
		bool DrawProperty(Property* property);
	};
}

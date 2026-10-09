//====================================================//
// ファイル名   : SelectedOnGUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/29
//
// 概要 : GUI上で選択されているオブジェクトを持つクラス
//
// 更新履歴 :
// 2026/07/29 新規作成
//====================================================//

#pragma once

#ifdef ENGINE_GUI

//====================================================//
// インクルードファイル
//====================================================//
#include <optional>
#include "Common/Property/PropertyObject.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class SelectedOnGUI
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 選択中のオブジェクト
		PropertyObject* m_propertyObject;

		// 選択されたUUID
		UUID m_selectedAssetID;

		// AssetManager
		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SelectedOnGUI(AssetManager& assetManager)
			: m_propertyObject{ nullptr } 
			, m_selectedAssetID{ UUID_NONE }
			, m_assetManager{ assetManager }
		{};
		~SelectedOnGUI() = default;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		PropertyObject* GetSelected()
		{
			// Handleが設定されていてpropertyObjectがない場合
			if (!m_propertyObject && m_selectedAssetID != UUID_NONE)
			{
				// ロード完了チェック
				AssetBase* asset = m_assetManager.GetFromUnTypeHandle(m_assetManager.GetHandle(m_selectedAssetID));

				// 読み込まれていれば
				if (asset)
				{
					// 選択
					m_propertyObject = asset;
				}
			}

			return m_propertyObject;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetSelected(PropertyObject* obj) 
		{
			m_propertyObject = obj; 

			m_selectedAssetID = UUID_NONE;
		}

		void SetSelectedAssetID(UUID uuid) 
		{
			m_selectedAssetID = uuid;

			m_propertyObject = nullptr;
		}
	};
}

#endif // ENGINE_GUI

/**
 * @file    SelectedOnGUI.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/29
 *
 * @brief   GUI上で選択されているオブジェクトを持つクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include "Common/UUID.h"
#include "Common/Property/PropertyObject.h"
#include "Assets/Managers/AssetManager.h"
#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	/**
	 * @brief GUI上で選択されているオブジェクトを管理する
	 */
	class SelectedOnGUI
	{
	private:

		// 選択中のオブジェクト
		PropertyObject* m_pSelectedObject;

		// 選択されたUUID
		UUID m_selectedAssetID;

		// AssetManager
		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SelectedOnGUI(AssetManager& assetManager)
			: m_pSelectedObject{ nullptr } 
			, m_selectedAssetID{ UUID_NONE }
			, m_assetManager{ assetManager }
		{};
		~SelectedOnGUI() = default;

		/**
		 * @brief 選択中のオブジェクトの取得
		 * @return 選択中のオブジェクト
		 * @TODO 処理が多いためcppに定義を移行すること
		 */
		PropertyObject* GetSelected()
		{
			// Handleが設定されていてpropertyObjectがない場合
			if (!m_pSelectedObject && m_selectedAssetID != UUID_NONE)
			{
				// ロード完了チェック
				AssetBase* asset = m_assetManager.GetFromUnTypeHandle(m_assetManager.GetHandle(m_selectedAssetID));

				// 読み込まれていれば
				if (asset)
				{
					// 選択
					m_pSelectedObject = asset;

					m_selectedAssetID = UUID_NONE;
				}
			}

			return m_pSelectedObject;
		}

		/**
		 * @brief 選択オブジェクトを変更する
		 * @param obj 新たに選択されたオブジェクト
		 */
		void SetSelected(PropertyObject* pSelected) 
		{
			m_pSelectedObject = pSelected; 

			m_selectedAssetID = UUID_NONE;
		}

		/**
		 * @brief アセット選択時
		 * @param uuid 選択されたアセットのUUID
		 */
		void SetSelectedAssetID(UUID uuid) 
		{
			m_selectedAssetID = uuid;

			m_pSelectedObject = nullptr;
		}
	};
}	// namespace RHEngine

#endif // ENGINE_GUI

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

		// 選択されたハンドル
		std::optional<UnTypeHandle> m_selectedHandle;

		// AssetManager
		AssetManager& m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SelectedOnGUI(AssetManager& assetManager)
			: m_propertyObject{ nullptr } 
			, m_assetManager{ assetManager }
		{};
		~SelectedOnGUI() = default;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		PropertyObject* GetSelected()
		{
			// Handleが設定されていてpropertyObjectがない場合
			if (!m_propertyObject && m_selectedHandle != std::nullopt)
			{
				// ロード完了チェック
				AssetBase* asset = m_assetManager.GetFromUnTypeHandle(m_selectedHandle.value());

				// 読み込まれていれば
				if (asset)
				{
					// 選択
					m_propertyObject = asset;
				}
			}

			return m_propertyObject;
		}

		UnTypeHandle* GetSelectedHandle()
		{
			if (m_selectedHandle.has_value()) return &m_selectedHandle.value();

			return nullptr;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetSelected(PropertyObject* obj) 
		{
			m_propertyObject = obj; 

			m_selectedHandle.reset();
		}

		void SetSelectedHandle(UnTypeHandle handle) 
		{
			m_selectedHandle = handle;

			m_propertyObject = nullptr;
		}
	};
}

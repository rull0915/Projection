/**
 * @file    AssetBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/21
 *
 * @brief   アセット基底クラス
 */

#pragma once

#include <string>

#include "Common/UUID.h"
#include "LoadStatus.h"
#include "Common/Property/PropertyObject.h"

namespace RHEngine
{
	class AssetManager;

	/**
	 * @brief アセットの基底
	 */
	class AssetBase : public PropertyObject
	{
	private:

		/** @brief 読み込み状態 */
		LoadStatus m_status = LoadStatus::Unloaded;

		/** @brief 対応するファイルパス */
		std::wstring m_path = L"";

		/** @brief 対応するUUID */
		UUID m_uuid = 0;

		/** @brief GUIから変更不可のフラグ */
		bool m_readOnly = false;

		/** @brief アセット名 */
		std::string m_name;

	public:
		/// @name ライフサイクル
		/// @{

		AssetBase() = default;

		virtual ~AssetBase() = default;

		/// @}

		/** 
		 * @brief GUI上で値が変更されたときに呼ばれる 
		 */
		virtual void OnValidate() {}

		/**
		 * @brief ロード時にアセットマネージャーが渡される
		 * @param  アセットマネージャーポインタ
		 */
		virtual void ReceiveAssetManager(AssetManager*) {};

		/// @name セッター
		/// @{

		/** @brief 読み込み状態 */
		void SetStatus(LoadStatus status) { m_status = status; }

		/** @brief ファイルパス */
		void SetPath(const std::wstring& path) { m_path = path; }

		/** @brief UUID */
		void SetUUID(UUID uuid) { m_uuid = uuid; }

		/** @brief アセット名 */
		void SetName(const std::string& name) { m_name = name; }

		/** @brief 変更不可フラグ */
		void SetReadOnly(bool flag) { m_readOnly = flag; }

		/// @}

		/// @name ゲッター
		/// @{

		/** @brief 読み込み状態 */
		LoadStatus GetStatus() const { return m_status; }

		/** @brief UUID */
		UUID GetUUID() const { return m_uuid; }

		/** @brief 変更不可フラグ */
		bool IsReadOnly() const { return m_readOnly; }

		/** @brief アセット名 */
		const std::string& GetName() const { return m_name; }

		/// @}
	};
}	// namespace RHEngine

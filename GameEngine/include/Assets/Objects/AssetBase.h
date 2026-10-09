//====================================================//
// ファイル名   : AssetBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要 : アセット基底クラス
//
// 更新履歴 :
// 2026/07/21 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>

#include "Common/UUID.h"
#include "LoadStatus.h"
#include "Common/Property/PropertyObject.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetBase : public PropertyObject
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 読み込み状態
		LoadStatus m_status = LoadStatus::Unloaded;

		// 参照しているファイルへのパス
		std::wstring m_path = L"";

		// 自身のUUID
		UUID m_uuid = 0;

		// GUI変更不可フラグ
		bool m_readOnly = false;

		// アセット名
		std::string m_name;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetBase() = default;
		virtual ~AssetBase() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// GUI出の値変更時に呼ばれる関数
		virtual void OnValidate() {}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// 読み込み状態
		void SetStatus(LoadStatus status) { m_status = status; }

		// パス
		void SetPath(const std::wstring& path) { m_path = path; }

		// UUID
		void SetUUID(UUID uuid) { m_uuid = uuid; }

		// 名前
		void SetName(const std::string& name) { m_name = name; }

		// 変更不可フラグ
		void SetReadOnly(bool flag) { m_readOnly = flag; }

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

	public:
		// 読み込み状態
		LoadStatus GetStatus() const { return m_status; }

		// UUID
		UUID GetUUID() const { return m_uuid; }

		// 変更不可フラグ
		bool IsReadOnly() const { return m_readOnly; }

		// 名前
		const std::string& GetName() const { return m_name; }
	};
}	// namespace REngine

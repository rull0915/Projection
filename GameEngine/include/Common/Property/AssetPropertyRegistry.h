//====================================================//
// ファイル名   : AssetPropertyRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : プロパティとしてのアセットを管理するクラス
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>
#include <typeindex>
#include <functional>

#include "Assets/Objects/Handle.h"
#include "Assets/Managers/IAssetResolver.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetPropertyRegistry
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 値の変更を反映させる関数マップ
		std::unordered_map<std::type_index, std::function<void(void*, const UnTypeHandle&)>> m_assignMap;

		// ハンドルからUUIDを取得する関数マップ
		std::unordered_map<std::type_index, std::function<UUID(void*, const IAssetResolver&)>> m_getUUIDMap;

	private:
		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetPropertyRegistry() = default;
		~AssetPropertyRegistry() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
	public:
		// シングルトン化
		static AssetPropertyRegistry& Instance()
		{
			static AssetPropertyRegistry instance;
			return instance;
		}

		template<class T>
		void Register()
		{
			// TypeIndexを取得
			auto idx = std::type_index(typeid(T));

			// 既にあればなにもしない
			if (m_assignMap.contains(idx)) return;

			// マップに追加
			m_assignMap[idx] =
				[](void* property, const UnTypeHandle& handle)
				{
					*static_cast<Handle<T>*>(property) = handle.As<T>();
				};

			m_getUUIDMap[idx] =
				[](void* property, const IAssetResolver& resolver)
				{
					return resolver.GetUUID(static_cast<Handle<T>*>(property)->GetUnTypeHandle());
				};
		}
		
		// 値を反映させる関数
		void Assign(std::type_index idx, void* ptr, const UnTypeHandle& handle) const { m_assignMap.at(idx)(ptr, handle); }

		// UUIDを取得する関数
		UUID GetUUID(std::type_index idx, void* ptr, const IAssetResolver& resolver) const { return m_getUUIDMap.at(idx)(ptr, resolver); }
	};
}	//namespace REngine

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

		// 列挙子名から実際の変数の値をセットする関数マップ
		std::unordered_map<std::type_index, std::function<bool(void*, const UnTypeHandle&)>> m_assignMap;

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetPropertyRegistry() = default;
		~AssetPropertyRegistry() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

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
		}
	};
}	//namespace REngine

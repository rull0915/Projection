//====================================================//
// ファイル名   : ClassNameGetter.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : コンポーネント名を取得する関数
//
// 更新履歴 :
// 2026/06/29 新規作成
//====================================================//

#pragma once
#include <type_traits>
#include <typeinfo>
#include <string>

//====================================================//
// クラス宣言
//====================================================//


namespace ClassNameGetter
{
	// クラスのみを受け取るテンプレート
	template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
	std::string Get()
	{
		std::string name = typeid(T).name();
		name.erase(name.begin(), name.begin() + 6);
	}

	// クラスのみを受け取るテンプレート
	template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
	std::string Get(const T& t)
	{
		std::string name = typeid(t).name();
		name.erase(name.begin(), name.begin() + 6);

		return name;
	}
};

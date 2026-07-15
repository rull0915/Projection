//====================================================//
// ファイル名   : TypeIdGenerator.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/25
//
// 概要 : 型ごとに異なる整数値を取得できるクラス
//
// 更新履歴 :
// 2026/06/25 新規作成
//====================================================//

#pragma once

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class TypeIDGenerator {
	private:
		// 呼ばれるたびに数値を1増やして返す関数
		static unsigned int GetNextValue()
		{
			// 数値
			static unsigned int value = 0;
			return ++value;
		}
	public:
		// IDを取得する関数 
		// templateの型ごとに関数が生成される仕組みを利用
		template <typename T>
		static unsigned int GetID()
		{
			// 一度だけ値を取得する
			static unsigned int id = GetNextValue();
			return id;
		}
	};
}	// namespace REngine

/**
 * @file    FactoryRegister.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/29
 *
 * @brief   エディターデフォルトのコンポーネントをファクトリへ登録する
 */
#pragma once

namespace RHEngine
{
	class FactoryRegister
	{
	public:

		/**
		 * @brief 各コンポーネントを登録する
		 */
		static void RegistFactories();
	};
}	// namespace RHEngine

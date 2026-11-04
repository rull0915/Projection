/**
 * @file    WorldComponentBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/28
 *
 * @brief   World空間のコンポーネントの基底クラス
 */

#pragma once

#include "Components/ComponentBase.h"
#include "Components/ComponentInfo.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief World空間のコンポーネントの基底
	 */
	class WorldComponentBase : public ComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(ComponentBase);

		/**
		 * @brief 自身の所属空間をWorldに指定
		 */
		static constexpr ComponentSpace SPACE = ComponentSpace::World;

	public:

		/// @name ライフサイクル
		/// @{
		
		/**
		 * @param pOwner 所有者のポインタ
		 */
		WorldComponentBase(IComponentOwner* pOwner)
			: ComponentBase(pOwner)
		{}

		virtual ~WorldComponentBase() = default;

		/// @}
	};
} // namespace RHEngine

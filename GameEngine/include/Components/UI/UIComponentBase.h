/**
 * @file    UIComponentBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/11
 *
 * @brief   UI用コンポーネント基底クラス
 */

#pragma once

#include "Components/ComponentBase.h"
#include "Components/ComponentInfo.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief UI空間コンポーネントの基底
	 */
	class UIComponentBase : public ComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(ComponentBase);

		static constexpr ComponentSpace SPACE = ComponentSpace::UI;

	public:

		/// @name ライフサイクル
		/// @{
		
		/**
		 * @param owner 所有者
		 */
		UIComponentBase(IComponentOwner* pOwner)
			: ComponentBase(pOwner)
		{}

		virtual ~UIComponentBase() = default;

		/// @}
	};
} // namespace RHEngine

/**
 * @file    BothComponentBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   World,UIどちらにも付けられるコンポーネント
 */

#pragma once

#include "Components/ComponentBase.h"
#include "Components/ComponentInfo.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief World空間、UI空間どちらにも存在可能なコンポーネントの基底
	 */
	class BothComponentBase : public ComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(ComponentBase);

		/** @brief 所属空間をbothに指定 */
		static constexpr ComponentSpace SPACE = ComponentSpace::Both;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		BothComponentBase(IComponentOwner* pOwner)
			: ComponentBase(pOwner)
		{}

		virtual ~BothComponentBase() = default;

		/// @}
	};
} // namespace RHEngine

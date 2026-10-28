/**
 * @file    ReferenceRegistry.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/29
 *
 * @brief   Refを使った参照関係の管理をするクラス
 */

#pragma once

#include <unordered_map>

#include "Common/ObjectReference.h"
#include "Common/Property/PropertyObject.h"

namespace RHEngine
{
	class ReferenceRegistry
	{
	private:

		/** @brief 参照クラスと参照先の対応表 */
		std::unordered_map<RefBase*, PropertyObject*> m_refs;

	public:

		/// @name ライフサイクル
		/// @{

		ReferenceRegistry() = default;

		~ReferenceRegistry()
		{
			// 全無効化
			for (auto& ref : m_refs)
			{
				ref.first->Invalidate();
			}
		}

		/// @}

		/**
		 * @brief 対応表を追加する
		 * @param pRef 参照クラス
		 * @param pObject 参照先のオブジェクト
		 */
		void AddPair(RefBase* pRef, PropertyObject* pObject)
		{
			// リストに追加
			m_refs[pRef] = pObject;

			// 削除時コールバックを設定
			pRef->SetDestroyCallBack([this](RefBase* pBase) { this->RemoveRef(pBase); });
		}

		/**
		 * @brief 参照クラスを対応表から削除する
		 */
		void RemoveRef(RefBase* pRef)
		{
			m_refs.erase(pRef);
		}

		/**
		 * @brief 参照先を対応表から削除する
		 */
		void RemoveObj(PropertyObject* pObject)
		{
			// 検索
			std::erase_if(m_refs, [pObject](const auto& pair)
				{
					// 削除対象なら
					if (pair.second == pObject)
					{
						// 無効化
						pair.first->Invalidate();

						return true;
					}
					return false;
				});
		}
	};
}	// namespace RHEngine

/**
 * @file    ObjectFinder.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/07
 *
 * @brief   オブジェクトの探索をするクラス
 */

#pragma once

#include <string>

#include "GameObject/GameObject.h"
#include "Common/UUID.h"

namespace RHEngine
{
	class Scene;

	class ObjectFinder
	{
	private:

		/** @brief シーンポインタ */
		Scene* m_pScene;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene シーンポインタ
		 */
		ObjectFinder(Scene* pScene)
			: m_pScene{ pScene }
		{}

		~ObjectFinder() = default;

		/// @}
		 
		/** @brief オブジェクトを名前検索する関数 */
		GameObject* FindWithName(const std::string& name) const;
		
		/** @brief オブジェクトをタグ検索する関数 */
		GameObject* FindWithTag(const std::string& tag) const;

		/** @brief オブジェクトをUUID検索する関数 */
		GameObject* FindWithUUID(UUID uuid) const;
	};
}	// namespace RHEngine

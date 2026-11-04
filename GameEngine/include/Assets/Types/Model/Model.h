/**
 * @file    Model.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/07
 *
 * @brief   モデルアセット
 */

#pragma once

#include <vector>

#include "Mesh.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"

namespace RHEngine
{
	/**
	 * @brief モデルアセット
	 */
	class Model : public AssetBase
	{
	private:

		/** @brief 自身が持つメッシュリスト */
		std::vector<Handle<Mesh>> m_meshes;
		
		/** @brief 自身が持つマテリアルリスト */
		std::vector<Handle<MaterialAsset>> m_materials;

	public:

		/// @name ライフサイクル
		/// @{
		
		Model() = default;

		~Model() = default;

		/// @}
	};
}	// namespace RHEngine

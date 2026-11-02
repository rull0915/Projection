/**
 * @file    MeshFilter.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/08
 *
 * @brief   オブジェクトが使うメッシュを保持するコンポーネント
 */

#pragma once

#define IS_COMPONENT(MeshFilter)

#include "Components/World/WorldComponentBase.h"
#include "Assets/Types/Model/Mesh.h"
#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief オブジェクトが使用するメッシュを保持するコンポーネント
	 * @details 将来的にMeshRendererのみではなく、MeshCollider等にも使用する可能性があったため分離。
	 */
	class MeshFilter : public RHEngine::WorldComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

	private:

		/** @brief メッシュアセット */
		Handle<Mesh> m_mesh;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		MeshFilter(RHEngine::IComponentOwner* own)
			: WorldComponentBase(own)
			, m_mesh{ ERROR_HANDLE<Mesh> }
		{
			ADD_PROPERTY(MeshFilter, m_mesh);
		}

		~MeshFilter() = default;

		/// @}

		/// @name ゲッター
		/// @{

		/** @brief メッシュアセット */
		Handle<Mesh> GetMesh() const { return m_mesh; }
		
		/// @}

		/// @name セッター
		/// @{

		/** @brief メッシュアセット */
		void SetMesh(Handle<Mesh> mesh) { m_mesh = mesh; }

		/// @}
	};
}	// namespace RHEngine

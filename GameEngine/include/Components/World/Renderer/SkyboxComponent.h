/**
 * @file    SkyboxComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   スカイボックスを描画するコンポーネント
 */

#pragma once

#include "Components/World/Renderer/RendererBase.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Types/Model/Mesh.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class SkyboxEffect;
	class AssetManager;

	/**
	 * @brief スカイボックス描画コンポーネント
	 */
	class SkyboxComponent : public RendererBase
	{
	public:
		DECLARE_COMPONENT_TYPE(RendererBase);

	private:

		/** @brief 描画するメッシュ */
		Mesh* m_pMesh;

		/** @brief 描画に使用するマテリアル */
		Handle<MaterialAsset> m_material;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		SkyboxComponent(IComponentOwner* own);

		~SkyboxComponent() = default;

		/// @}

		/**
		 * @brief スカイボックスの描画を行う
		 */
		void Draw(Renderer& renderer) override;
	};
} // namespace RHEngine

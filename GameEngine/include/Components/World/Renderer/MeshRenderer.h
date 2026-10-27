/**
 * @file    MeshRenderer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/03
 *
 * @brief   メッシュの描画を行うコンポーネント
 */

#pragma once

#include <vector>

#include "RendererBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class ShadowRenderer;
	class AssetManager;

	/**
	 * @brief メッシュの描画を行う
	 * @details 影を生成する唯一の描画コンポーネントであるためShadowRendererも実装する
	 */
	class MeshRenderer : public RendererBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(RendererBase);

	private:
		
		/**
		 * @brief マテリアルの配列
		 * @details メッシュが持つSubMesh1つずつに割り当てられる
		 *			SubMeshのn番目をmaterialsのn番目で描画する
		 */
		std::vector<Handle<MaterialAsset>> m_materials;

		/** @brief アセットマネージャーのポインタ */
		AssetManager* m_pAssetManager;

	public:

		/// @name ライフサイクル
		/// @{
		 
		/**
		 * @param own 所有者のポインタ
		 */
		MeshRenderer(IComponentOwner* own);

		~MeshRenderer() = default;

		/// @}

		/**
		 * @brief メッシュの描画を行う
		 */
		void Draw(Renderer& renderer) override;

		/**
		 * @brief シャドウマップへの描画を行う
		 */
		void DrawToShadowMap(ShadowRenderer& shadowRenderer);

		/**
		 * @brief アセットマネージャーを受け取る
		 */
		void ReceiveAssetManager(AssetManager& assetManager) override
		{
			m_pAssetManager = &assetManager;
		}

		/// @name ゲッター
		/// @{
		
		/** @brief マテリアルリスト */
		std::vector<Handle<MaterialAsset>>& GetMaterials() { return m_materials; }

		/** @brief マテリアルリスト (const) */
		const std::vector<Handle<MaterialAsset>>& GetMaterials() const { return m_materials; }

		/// @}
	};
} // namespace RHEngine

/**
 * @file    SpriteRenderer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/10
 *
 * @brief   ワールド空間に画像を描画する
 */

#pragma once

#include "RendererBase.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"
#include "Assets/Types/Model/Mesh.h"
#include "Components/Interface/IAssetDependent.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class AssetManager;

	/**
	 * @brief 描画を行うコンポーネントの基底
	 */
	class SpriteRenderer : public RendererBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(RendererBase);
		
		/**
		 * @brief 描画モード
		 */
		enum class RenderMode
		{
			World,
			Billboard,
			ConstrainedBillboard
		};

	private:

		/** @brief アセットマネージャーポインタ */
		AssetManager* m_pAssetManager;

		/** @brief 使うテクスチャハンドル */
		Handle<Texture> m_texture;

		/** @brief 乗算色 */
		DirectX::SimpleMath::Color m_color;

		/** @brief 描画する範囲の最小値 */
		DirectX::SimpleMath::Vector2 m_minUvRect;

		/** @brief 描画する範囲の最大値 */
		DirectX::SimpleMath::Vector2 m_maxUvRect;

		/** @brief 描画のモード */
		RenderMode m_mode;

		/** @brief 使うメッシュ */
		Mesh* m_pMesh;

		/** @brief ConstrainedBillboard時に使用する固定軸ベクトル */
		DirectX::SimpleMath::Vector3 m_fixedAxis;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		SpriteRenderer(IComponentOwner* pOwner);

		~SpriteRenderer() = default;

		/// @}

		void OnValidate() override;

		/**
		 * @brief アセットマネージャーを受け取る
		 */
		void ReceiveAssetManager(AssetManager& assetManager) override
		{
			m_pAssetManager = &assetManager;
		}

		/**
		 * @brief 描画を行う
		 */
		virtual void Draw(Renderer& renderer);
	};
} // namespace RHEngine

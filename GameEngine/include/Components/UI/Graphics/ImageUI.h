/**
 * @file    ImageUI.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/23
 *
 * @brief   画像描画コンポーネント
 */

#pragma once

#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"
#include "Assets/Types/Material/MaterialAsset.h"

namespace RHEngine
{
	class IComponentOwner;
	class AssetManager;
	class Renderer;

	/**
	 * @brief 画像描画コンポーネント
	 */
	class ImageUI : public UIGraphicBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(UIGraphicBase);

	private:

		/** @brief 描画するテクスチャアセットのハンドル */
		Handle<Texture> m_textureHandle;

		/** @brief 描画時に使用するマテリアルアセットのハンドル */
		Handle<MaterialAsset> m_materialHandle;

		/** @brief Rayと衝突するかどうか */
		bool m_raycastTarget;

		/** @brief アセットマネージャーのポインタ */
		AssetManager* m_pAssetManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param owner 所有者のポインタ
		 */
		ImageUI(IComponentOwner* pOwner)
			: UIGraphicBase(pOwner)
			, m_textureHandle{}
			, m_materialHandle{}
			, m_raycastTarget{ true }
			, m_pAssetManager{ nullptr }
		{
			ADD_PROPERTY(ImageUI, m_textureHandle);
			ADD_PROPERTY(ImageUI, m_materialHandle);
			ADD_PROPERTY(ImageUI, m_raycastTarget);
		}

		~ImageUI() = default;

		/// @}

		/**
		 * @brief 画像の描画を行う
		 */
		void Draw(Renderer& renderer) override;

		/**
		 * @brief アセットマネージャーを受け取る
		 */
		void ReceiveAssetManager(AssetManager& assetManager) override
		{
			m_pAssetManager = &assetManager;
		}

		/// @name ゲッター
		//

		/** @brief Rayに衝突するかどうか */
		bool IsRaycastTarget() const { return m_raycastTarget; }

		/// @}

		/// @name セッター
		/// @{
		
		/** @brief テクスチャハンドル */
		void SetTexture(Handle<Texture> handle) { m_textureHandle = handle; }

		/** @brief Rayに衝突するかどうか */
		void SetRaycastTarget(bool f) { m_raycastTarget = f; }

		/// @}
	};
} // namespace RHEngine

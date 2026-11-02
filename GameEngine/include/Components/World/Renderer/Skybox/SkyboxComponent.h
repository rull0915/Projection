/**
 * @file    SkyboxComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   スカイボックスを描画するコンポーネント
 * @TODO 自分のMaterialをシェーダーアセットを使用した描画形式に変更
 */

#pragma once

#include <memory>
#include <GeometricPrimitive.h>
#include <wrl/client.h>
#include <d3d11.h>

#include "Components/World/Renderer/RendererBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class SkyboxEffect;
	class AssetManager;

	/**
	 * @brief スカイボックス描画コンポーネント
	 */
	class SkyboxComponent : public RendererBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(RendererBase);

	private:

		/** @brief スカイボックスを張り付ける形状 */
		std::unique_ptr<DirectX::GeometricPrimitive> m_sky;

		/** @brief 描画時に使用するエフェクト */
		std::unique_ptr<SkyboxEffect> m_effect;

		/** @brief 描画時に使用するInputLayout */
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyInputLayout;

		/** @brief スカイボックスのテクスチャアセット */
		Handle<Texture> m_textureHandle;

		/** @brief アセットマネージャーのポインタ */
		AssetManager* m_pAssetManager;

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

		/**
		 * @brief テクスチャアセットをセットする
		 * @param handle 変更後のテクスチャアセットハンドル
		 */
		void SetTexture(Handle<Texture> handle);

		/**
		 * @brief アセットマネージャーを受け取る
		 * @param assetManager 
		 */
		void ReceiveAssetManager(AssetManager& assetManager) override
		{
			m_pAssetManager = &assetManager;
		}

		/**
		 * @brief GUI上での値変更時テクスチャを反映させる
		 */
		void OnValidate() override
		{
			SetTexture(m_textureHandle);
		}
	};
} // namespace RHEngine

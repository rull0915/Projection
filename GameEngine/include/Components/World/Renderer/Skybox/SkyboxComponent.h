//====================================================//
// ファイル名   : SkyboxComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : スカイボックスコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <GeometricPrimitive.h>
#include "Components/World/Renderer/RendererBase.h"

#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class SkyboxEffect;

	//====================================================//
	// クラス宣言
	//====================================================//
	class SkyboxComponent : public RendererBase, public IAssetDependent
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		std::unique_ptr<DirectX::GeometricPrimitive> m_sky;
		std::unique_ptr<SkyboxEffect> m_effect;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyInputLayout;

		// テクスチャハンドル
		Handle<Texture> m_textureHandle;

		// AssetManager
		AssetManager* m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SkyboxComponent(IComponentOwner* own);
		~SkyboxComponent() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Start() override;

		void Draw(Renderer& renderer) override;

		void SetTexture(Handle<Texture> handle);

		void ReceiveAssetManager(AssetManager& manager) override
		{
			m_assetManager = &manager;
		}

		void OnValidate() override
		{
			SetTexture(m_textureHandle);
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<SkyboxComponent>();
		}
	};
} // namespace REngine

//====================================================//
// ファイル名   : ImageUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要 : 画像描画コンポーネント
//
// 更新履歴 :
// 2026/05/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Texture.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ImageUI : public UIGraphicBase, public IAssetDependent
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// テクスチャハンドル
		Handle<Texture> m_textureHandle;

		// Rayと衝突するかどうか
		bool m_raycastTarget;

		// AssetManager
		AssetManager* m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ImageUI(IComponentOwner* owner)
			: UIGraphicBase(owner)
			, m_textureHandle{}
			, m_raycastTarget{ true }
		{
			ADD_PROPERTY(m_textureHandle);
		}

		~ImageUI() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Draw(Renderer& renderer) override;

		void ReceiveAssetManager(AssetManager& manager) override
		{
			m_assetManager = &manager;
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<ImageUI>();
		}

		bool IsRaycastTarget() const { return m_raycastTarget; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetTexture(Handle<Texture> handle)
		{
			m_textureHandle = handle;
		}
		void SetRaycastTarget(bool f) { m_raycastTarget = f; }
	};
} // namespace REngine

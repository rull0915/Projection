//====================================================//
// ファイル名   : MeshRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/03
//
// 概要 :
//
// 更新履歴 :
// 2026/05/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "RendererBase.h"
#include "Components/Interface/IAssetDependent.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/MaterialAsset.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class MeshRenderer : public RendererBase, public IAssetDependent
	{
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// マテリアルの配列
		std::vector<Handle<MaterialAsset>> m_materials;
		Handle<MaterialAsset> m_material;

		// AssetManager
		AssetManager* m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		MeshRenderer(IComponentOwner* own)
			: RendererBase(own)
			, m_materials{}
			, m_assetManager{ nullptr }
		{
			//ADD_PROPERTY(MeshRenderer, m_materials);
			ADD_PROPERTY(MeshRenderer, m_materials);
		};
		~MeshRenderer() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(MeshRenderer, RendererBase)

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 描画関数
		void Draw(Renderer& renderer) override;

		// AssetManagerを受け取る関数
		void ReceiveAssetManager(AssetManager& a) override
		{
			m_assetManager = &a;
		}

	};
} // namespace REngine

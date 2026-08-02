//====================================================//
// ファイル名   : ModelComponent.h
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
#include "Assets/Types/Model.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ModelComponent : public RendererBase, public IAssetDependent
	{
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// モデルハンドル
		Handle<Model> m_modelHandle;

		// AssetManager
		AssetManager* m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ModelComponent(IComponentOwner* own)
			: RendererBase(own)
			, m_modelHandle{}
		{
			ADD_PROPERTY(m_modelHandle);
		};
		~ModelComponent() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Start() override;

		// 描画関数
		void Draw(Renderer& renderer) override;

		// AssetManagerを受け取る関数
		void ReceiveAssetManager(AssetManager& a) override
		{
			m_assetManager = &a;
		}

		// モデルをセットする関数
		void SetModel(Handle<Model> handle)
		{
			m_modelHandle = handle;
		}

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<ModelComponent>();
		}
	};
} // namespace REngine

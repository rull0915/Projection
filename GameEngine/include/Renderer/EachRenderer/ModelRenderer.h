//====================================================//
// ファイル名   : ModelRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/01
//
// 概要 : モデルを描画するクラス
//
// 更新履歴 :
// 2026/05/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Model.h"
#include "Renderer/Command/DrawCommandContainer.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ModelRenderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 描画の状態
		DrawCommandContainer& m_commandContainer;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ModelRenderer(DrawCommandContainer& container)
			: m_commandContainer{ container }
		{}
		~ModelRenderer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
		void DrawModel(DirectX::Model* model, DirectX::SimpleMath::Matrix world)
		{
			// コマンドの生成
			auto& command = m_commandContainer.AddModel();

			command.pModel = model;
			command.world = world;
		}
	};
}	// namespace REngine

//====================================================//
// ファイル名   : DrawCommandExecutor.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要 : 描画コマンドを実行するクラス
//
// 更新履歴 :
// 2026/07/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <VertexTypes.h>

#include "DrawCommandContainer.h"
#include "Renderer/RenderInfoManager.h"

namespace REngine
{
	class AssetManager;
	class SamplerList;

	//====================================================//
	// クラス宣言
	//====================================================//
	class DrawCommandExecutor
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		//----- 各描画システム -----//

		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;

		// スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

		// 描画情報マネージャー
		RenderInfoManager& m_infoManager;

		//----- デバイス -----//

		// デバイス
		ID3D11Device* m_pDevice;

		// デバイスコンテキスト
		ID3D11DeviceContext* m_pContext;

		// コモンステート
		DirectX::CommonStates* m_pStates;

		//----- その他 -----//

		// アセットマネージャー
		AssetManager& m_assetManager;

		// デフォルトマテリアル
		Handle<MaterialAsset> m_defaultMaterial;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		DrawCommandExecutor(AssetManager& assetManager, RenderInfoManager& infoManager);
		~DrawCommandExecutor();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize();

		// 描画実行
		void DrawCommandExecute(DrawCommandContainer& container);

	private:

		// プリミティブコマンド
		void DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands);

		// スプライト フォント
		void DrawSpriteAndFontCommandExecute(const std::vector<DrawCommandContainer::DrawUICommand>& commands);

		// モデル
		void DrawModelCommandExecute(const std::vector<DrawModelCommand>& commands);
	};
}	// namespace REngine

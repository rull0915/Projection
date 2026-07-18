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
#include <SpriteFont.h>
#include <Model.h>
#include <Effects.h>
#include <VertexTypes.h>

#include "DrawCommandContainer.h"

namespace REngine
{
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
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

		// ベーシックエフェクト
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

		// スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		
		// インプットレイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		//----- 行列 -----//

		// ビュー行列
		DirectX::SimpleMath::Matrix m_view;

		// プロジェクション行列
		DirectX::SimpleMath::Matrix m_projection;

		//----- デバイス -----//

		// デバイスコンテキスト
		ID3D11DeviceContext* m_pContext;

		// コモンステート
		DirectX::CommonStates* m_pStates;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		DrawCommandExecutor();
		~DrawCommandExecutor() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize();

		// 描画実行
		void DrawCommandExecute(DrawCommandContainer& container, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	private:
		
		// プリミティブコマンド
		void DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands);

		// スプライト フォント
		void DrawSpriteAndFontCommandExecute(const std::vector<DrawSpriteCommand>& spriteCommands, const std::vector<DrawTextCommand>& textCommands);

		// モデル
		void DrawModelCommandExecute(const std::vector<DrawModelCommand>& commands);

		// プリミティブの描画を開始する関数
		void PreparePrimitiveRendering();
	};
}	// namespace REngine

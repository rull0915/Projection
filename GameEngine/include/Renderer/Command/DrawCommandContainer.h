//====================================================//
// ファイル名   : DrawCommandContainer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要 : 描画コマンドをため込んでおくクラス
//
// 更新履歴 :
// 2026/07/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include "DrawCommand.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class DrawCommandContainer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// プリミティブ描画コマンド配列
		std::vector<DrawPrimitiveCommand> m_drawPrimitiveCommands;

		// モデル描画コマンド配列
		std::vector<DrawModelCommand> m_drawModelCommands;

		// スプライト描画コマンド配列
		std::vector<DrawSpriteCommand> m_drawSpriteCommands;

		// テキスト描画コマンド配列
		std::vector<DrawTextCommand> m_drawTextCommands;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		DrawCommandContainer() = default;
		~DrawCommandContainer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 配列の初期化関数
		void Clear()
		{
			m_drawPrimitiveCommands.clear();
			m_drawModelCommands.clear();
			m_drawSpriteCommands.clear();
			m_drawTextCommands.clear();
		};

		// プリミティブ描画コマンドの登録関数
		DrawPrimitiveCommand& AddPrimitive()
		{
			return m_drawPrimitiveCommands.emplace_back();
		}

		// モデル描画コマンドの登録関数
		DrawModelCommand& AddModel()
		{
			return m_drawModelCommands.emplace_back();
		}

		// スプライト描画コマンドの登録関数
		DrawSpriteCommand& AddSprite()
		{
			return m_drawSpriteCommands.emplace_back();
		}

		// テキスト描画コマンドの登録関数
		DrawTextCommand& AddText()
		{
			return m_drawTextCommands.emplace_back();
		}

		// 最新のプリミティブ描画コマンドを取得する関数
		DrawPrimitiveCommand* GetLatestPrimitiveCommand()
		{
			// なければnullをかえす
			if (m_drawPrimitiveCommands.empty()) return nullptr;

			// あれば最後尾を返す
			return &m_drawPrimitiveCommands.back();
		}

		//============= 全コマンドの取得関数 =============//

		// プリミティブ描画コマンド配列
		const std::vector<DrawPrimitiveCommand>& GetDrawPrimitiveCommands() const { return m_drawPrimitiveCommands; };

		// モデル描画コマンド配列
		const std::vector<DrawModelCommand>& GetDrawModelCommands() const { return m_drawModelCommands; }

		// スプライト描画コマンド配列
		const std::vector<DrawSpriteCommand>& GetDrawSpriteCommands() const { return m_drawSpriteCommands; }

		// テキスト描画コマンド配列
		const std::vector<DrawTextCommand>& GetDrawTextCommands() const { return m_drawTextCommands; }

	};
}	// namespace REngine

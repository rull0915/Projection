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
#include <variant>
#include "DrawCommand.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class DrawCommandContainer
	{
	public:
		// テキストコマンドとスプライトコマンドを同時に扱うクラス
		using DrawUICommand = std::variant<DrawSpriteCommand, DrawTextCommand>;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// プリミティブ描画コマンド配列
		std::vector<DrawPrimitiveCommand> m_drawPrimitiveCommands;

		// モデル描画コマンド配列
		std::vector<DrawModelCommand> m_drawModelCommands;

		// UI描画コマンド配列
		std::vector<DrawUICommand> m_drawUICommands;

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
			m_drawUICommands.clear();
		};

		// プリミティブ描画コマンドの登録関数
		DrawPrimitiveCommand& AddPrimitive()
		{
			auto& cmd = m_drawPrimitiveCommands.emplace_back();
			cmd.lines.reserve(1024);
			cmd.triangles.reserve(512);

			return cmd;
		}

		// モデル描画コマンドの登録関数
		DrawModelCommand& AddModel()
		{
			return m_drawModelCommands.emplace_back();
		}

		// スプライト描画コマンドの登録関数
		DrawSpriteCommand& AddSprite()
		{
			return std::get<DrawSpriteCommand>(m_drawUICommands.emplace_back(DrawSpriteCommand()));
		}

		// テキスト描画コマンドの登録関数
		DrawTextCommand& AddText()
		{
			return std::get<DrawTextCommand>(m_drawUICommands.emplace_back(DrawTextCommand()));
		}

		// 最新のプリミティブ描画コマンドを取得する関数
		DrawPrimitiveCommand* GetLatestPrimitiveCommand()
		{
			// なければnullをかえす
			if (m_drawPrimitiveCommands.empty()) return nullptr;

			// どちらかの配列が最大値を超えるなら
			auto& latest = m_drawPrimitiveCommands.back();
			if (latest.lines.size() > 750 || latest.triangles.size() > 500)
			{
				// 追加
				auto& newCommand = AddPrimitive();

				// ワールドを引き継ぐ
				newCommand.world = latest.world;

				return &newCommand;
			}

			return &latest;
		}

		//============= 全コマンドの取得関数 =============//

		// プリミティブ描画コマンド配列
		const std::vector<DrawPrimitiveCommand>& GetDrawPrimitiveCommands() const { return m_drawPrimitiveCommands; };

		// モデル描画コマンド配列
		const std::vector<DrawModelCommand>& GetDrawModelCommands() const { return m_drawModelCommands; }

		// UI描画コマンド配列
		const std::vector<DrawUICommand>& GetDrawUICommands() const { return m_drawUICommands; }

	};
}	// namespace REngine

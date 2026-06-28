//====================================================//
// ファイル名   : SceneTransition.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2025/12/18
//
// 概要 : シーン遷移の基底クラス
//
// 更新履歴 :
// 2025/12/18 新規作成
//====================================================//

#pragma once

class Renderer;

// 遷移モード
namespace Transition
{
	enum class Mode
	{
		None,
		In,
		Out
	};

	// シーン遷移基底クラス
	class Base
	{
		// ----- 関数宣言 ----- //
	public:
		Base() = default;
		virtual ~Base() = default;

		// 純粋仮想関数
		virtual void Initialize() = 0;

		// 更新関数
		// 遷移終了時にtrue
		virtual bool InUpdate(const GameTimer& gameTimer) = 0;	// Inの場合	
		virtual bool OutUpdate(const GameTimer& gameTimer) = 0;	// Outの場合	

		// 描画関数
		virtual void InRender(Renderer& renderer) = 0;	// Inの場合
		virtual void OutRender(Renderer& renderer) = 0;	// Outの場合	
	};
}

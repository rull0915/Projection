//====================================================//
// ファイル名   : UIRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/09
//
// 概要 : UIの描画を担当するクラス
//
// 更新履歴 :
// 2026/05/09 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Renderer/Command/DrawCommandContainer.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class UIRenderer
	{
	private:

		DrawCommandContainer& m_container;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		UIRenderer(DrawCommandContainer& container);
		~UIRenderer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
	public:

		//-----------------------------------------------------
		// 描画関数
		//-----------------------------------------------------

		// ------ Triangle ------ //

		void DrawTriangle	// 短縮版
		(
			DirectX::SimpleMath::Vector2 p1,
			DirectX::SimpleMath::Vector2 p2,
			DirectX::SimpleMath::Vector2 p3,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		// -------- Rect -------- //

		void DrawRect
		(
			DirectX::SimpleMath::Vector2 p1,
			DirectX::SimpleMath::Vector2 p2,
			DirectX::SimpleMath::Vector2 p3,
			DirectX::SimpleMath::Vector2 p4,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		// -------- Line -------- //

		void DrawLine
		(
			DirectX::SimpleMath::Vector2 start,
			DirectX::SimpleMath::Vector2 end,
			DirectX::SimpleMath::Color color
		);

		// -------- Circle -------- //

		void DrawCircle
		(
			DirectX::SimpleMath::Vector2 centerPos,
			float radius,
			uint16_t division,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);

		// --------- Box ---------- //
		void DrawBox
		(
			DirectX::SimpleMath::Vector2 min,
			DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);
	};
}	// namespace REngine

//====================================================//
// ファイル名   : PrimitiveRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : プリミティブの描画を担当するクラス
//
// 更新履歴 :
// 2026/04/29 新規作成
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
	class PrimitiveRenderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// コマンドコンテナ
		DrawCommandContainer& m_container;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		PrimitiveRenderer(DrawCommandContainer& container);
		~PrimitiveRenderer() = default;

	public:

		//-----------------------------------------------------
		// 描画関数
		//-----------------------------------------------------

		// Triangle
		void DrawTriangle	
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector3& p3,
			DirectX::SimpleMath::Color color, bool fillFlag
		);
		void DrawTriangle	
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector2& uv1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector2& uv2,
			const DirectX::SimpleMath::Vector3& p3,
			const DirectX::SimpleMath::Vector2& uv3,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		// Rect
		void DrawRect
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector3& p3,
			const DirectX::SimpleMath::Vector3& p4,
			DirectX::SimpleMath::Color color, bool fillFlag
		);
		void DrawRect
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector2& uv1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector2& uv2,
			const DirectX::SimpleMath::Vector3& p3,
			const DirectX::SimpleMath::Vector2& uv3,
			const DirectX::SimpleMath::Vector3& p4,
			const DirectX::SimpleMath::Vector2& uv4,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		// Line
		void DrawLine
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector3& p2,
			DirectX::SimpleMath::Color color
		);

		// Circle
		void DrawCircle
		(
			const DirectX::SimpleMath::Vector3& centerPos,
			const DirectX::SimpleMath::Vector3& normal,
			float radius,
			uint16_t division,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);

		// Arc
		void DrawArc(
			const DirectX::SimpleMath::Vector3& center,
			const DirectX::SimpleMath::Vector3& vStart, // 開始方向（正規化されている前提）
			const DirectX::SimpleMath::Vector3& vEnd,   // 終了方向（正規化されている前提）
			uint16_t segments,
			float radius,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);
	};
}	// namespace REngine

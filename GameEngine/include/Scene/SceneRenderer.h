//====================================================//
// ファイル名   : SceneRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要 : シーンの描画担当クラス
//
// 更新履歴 :
// 2026/07/12 新規作成
// 2026/10/15 パスごとの描画関数に分割
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "System/Render/RenderContext.h"

namespace REngine
{
	class Renderer;
	class ShadowRenderer;
	class Scene;

	//====================================================//
	// クラス宣言
	//====================================================//

	/// <summary>
	/// シーンの描画担当
	/// </summary>
	class SceneRenderer
	{
		// デバッグ描画色
	public:
		static constexpr DirectX::SimpleMath::Color DEBUG_COLOR = { 0, 1, 0, 1 };

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// シーンポインタ
		Scene* m_pScene;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SceneRenderer(Scene* pScene);
		~SceneRenderer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ShadowPass描画
		void DrawShadowPass(const ShadowRenderContext& ctx, ShadowRenderer& renderer);

		// WorldPass描画
		void DrawWorldPass(const RenderContext& ctx, Renderer& renderer);

		// UIPass描画
		void DrawUIPass(const RenderContext& ctx, Renderer& renderer);
	};
}	// namespace REngine

//====================================================//
// ファイル名   : Renderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : 描画を統括するクラス
//
// 更新履歴 :
// 2026/04/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

// DirectXTK
#include "GraphicsSystem.h"
#include "RenderProxy.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Renderer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 描画仲介クラス
		RenderProxy m_renderProxy;

		// 描画システム
		GraphicsSystem m_graphicSystem;

		// 初期化フラグ
		bool m_isInitialized;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Renderer();
		~Renderer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化
		void Initialize();

		// 描画関数
		RenderProxy& Draw()
		{
			// 初期化チェック
			assert(m_isInitialized);

			// 仲介クラスを返す
			return m_renderProxy;
		}

		// 描画終了
		void End();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Matrix& GetWorld() const { return m_graphicSystem.GetWorld(); };
		const DirectX::SimpleMath::Matrix& GetView() const { return m_graphicSystem.GetView(); };
		const DirectX::SimpleMath::Matrix& GetProjection() const { return m_graphicSystem.GetProjection(); };

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_graphicSystem.SetWorld(world); }

		void SetVPMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_graphicSystem.SetVPMatrix(view, proj); }
	};
}	// namespace REngine

//====================================================//
// ファイル名   : ImageWindow.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/21
//
// 概要 : 画像を表示するウィンドウを描画するクラス
//
// 更新履歴 :
// 2026/10/21 新規作成
//====================================================//

#pragma once

#ifdef ENGINE_GUI

//====================================================//
// インクルードファイル
//====================================================//
#include "Editor/Editor/GuizmoManager.h"
#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ImageWindow
	{
	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------


		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ギズモ管理クラス
		GuizmoManager m_guizmoManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ImageWindow();
		~ImageWindow() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

	public:

		// シーンビューウィンドウの表示
		bool DrawSceneViewWindow(ID3D11ShaderResourceView* sceneView, SceneCamera* sceneViewCamera, Transform* target);

		// ゲームビューウィンドウの表示
		bool DrawGameViewWindow(ID3D11ShaderResourceView* gameView);

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	private:

		// 画像の表示
		DirectX::SimpleMath::Vector2 DrawImage(ID3D11ShaderResourceView* img);

		// シーンビュー描画の開始
		void StartSceneView();

		// ゲームビュー描画の開始
		void StartGameView();

		// シーンビューの描画
		bool DrawSceneView(ID3D11ShaderResourceView* sceneView, SceneCamera* sceneViewCamera, Transform* target);
	};
}

#endif

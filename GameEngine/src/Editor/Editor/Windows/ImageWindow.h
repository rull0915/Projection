/**
 * @file    ImageWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 *
 * @brief   画像を表示するウィンドウを描画するクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <d3d11.h>
#include <SimpleMath.h>

#include "Editor/Editor/GuizmoManager.h"
#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	class SceneCamera;

	/**
	 * @brief 画像を表示するウィンドウ
	 */
	class ImageWindow
	{
	private:

		/** @brief ギズモ管理クラス */
		GuizmoManager m_guizmoManager;

	public:

		/// @name ライフサイクル
		/// @{

		ImageWindow();

		~ImageWindow() = default;

		/// @}

		/**
		 * @brief シーンビューウィンドウを表示する
		 * @param pSceneView シーンビューのテクスチャ
		 * @param pSceneViewCamera シーンビュー用カメラ
		 * @param pTarget 選択されているトランスフォーム
		 * @return カメラを動かすことが出来るかどうか
		 */
		bool DrawSceneViewWindow(ID3D11ShaderResourceView* pSceneView, SceneCamera* pSceneViewCamera, Transform* pTarget);

		/**
		 * @brief ゲームビューウィンドウを表示する
		 * @param gameView ゲームビューのテクスチャ
		 * @return クリックされたかどうか
		 */
		bool DrawGameViewWindow(ID3D11ShaderResourceView* gameView);

	private:

		/**
		 * @brief ウィンドウへ画像を表示する
		 * @param img 表示する画像
		 * @return 表示した画像のサイズ
		 */
		DirectX::SimpleMath::Vector2 DrawImage(ID3D11ShaderResourceView* img);

		/**
		 * @brief シーンビューの描画を開始する
		 */
		void StartSceneView();

		/**
		 * @brief ゲームビューの描画を開始する
		 */		
		void StartGameView();

		/**
		 * @brief シーンビューを描画する
		 * @param pSceneView シーンビューテクスチャ
		 * @param pSceneViewCamera シーンビュー用カメラ
		 * @param target 選択しているTransform
		 * @return カメラを動かせるかどうか
		 */
		bool DrawSceneView(ID3D11ShaderResourceView* pSceneView, SceneCamera* pSceneViewCamera, Transform* target);
	};
}	// namespace RHEngine

#endif

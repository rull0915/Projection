/**
 * @file    ImageWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 */

#include "pch.h"

#ifdef ENGINE_GUI

#include "ImageWindow.h"

#include "ThirdParty/imgui/imgui.h"
#include "System/WindowManager.h"
#include "Input/KeyInput.h"

namespace RHEngine
{
	ImageWindow::ImageWindow()
	{}

	bool ImageWindow::DrawSceneViewWindow(ID3D11ShaderResourceView* pSceneView, SceneCamera* pSceneViewCamera, Transform* pTarget)
	{
		StartSceneView();

		bool canSceneCameraMove = DrawSceneView(pSceneView, pSceneViewCamera, pTarget);

		ImGui::End();

		return canSceneCameraMove;
	}

	bool ImageWindow::DrawGameViewWindow(ID3D11ShaderResourceView* pGameView)
	{
		StartGameView();

		// ウィンドウがクリックされたかどうか
		bool clicked = ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

		DrawImage(pGameView);

		ImGui::End();

		return clicked;
	}

	DirectX::SimpleMath::Vector2 ImageWindow::DrawImage(ID3D11ShaderResourceView* pImg)
	{
		// 有効サイズ
		ImVec2 avail = ImGui::GetContentRegionAvail();

		// アスペクト比
		float aspect = WindowManager::Instance().GetAspect();

		// 小さい方に合わせる
		float correction = avail.x / aspect;

		if (correction > avail.y)
		{
			avail.x = avail.y * aspect;
		}
		else if (correction < avail.y)
		{
			avail.y = avail.x / aspect;
		}

		ImGui::Image(
			(ImTextureID)pImg,
			avail);

		// 描画したサイズを返す
		return { avail.x, avail.y };
	}

	void ImageWindow::StartSceneView()
	{
		ImGui::Begin("Scene", nullptr);
	}

	void ImageWindow::StartGameView()
	{
		ImGui::Begin("GameView", nullptr);
	}

	bool ImageWindow::DrawSceneView(ID3D11ShaderResourceView* pSceneView, SceneCamera* pSceneViewCamera, Transform* pTarget)
	{
		// Image の表示位置を取得
		ImVec2 imagePos = ImGui::GetCursorScreenPos();

		// 描画
		DirectX::SimpleMath::Vector2 imageSize = DrawImage(pSceneView);

		// SceneViewウィンドウ上にマウスカーソルがあるか
		bool hovered = ImGui::IsWindowHovered();

		// SceneViewがフォーカスされているか
		bool focused = ImGui::IsWindowFocused();

		// SceneViewが入力を受け付ける状態か
		bool canControlCamera = hovered && focused;

		if (canControlCamera)
		{
			// キー入力を取得し描画するギズモを変える
			if (Input::Key::GetDown(Input::Key::Code::W)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_TRANSLATION);
			if (Input::Key::GetDown(Input::Key::Code::E)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_ROTATION);
			if (Input::Key::GetDown(Input::Key::Code::R)) m_guizmoManager.SetDrawFlag(GuizmoManager::DRAW_SCALE);
		}

		if (pTarget)
		{
			m_guizmoManager.DrawTransformGuizmo(pSceneViewCamera, pTarget, { imagePos.x, imagePos.y }, imageSize);
		}

		return canControlCamera;
	}
}	// namespace RHEngine

#endif	//ENGINE_GUI

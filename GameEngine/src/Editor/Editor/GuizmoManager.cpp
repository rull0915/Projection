//====================================================//
// ファイル名  : GuizmoManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/17
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GuizmoManager.h"

#include "imgui/ImGuizmo.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void REngine::GuizmoManager::DrawTransformGuizmo(CameraBase* camera, Transform* transform, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 size)
{
	// ワールド行列を取得
	auto world = transform->GetWorldMatrix();

	// 描画領域
	ImGuizmo::SetDrawlist();

	// ビュー
	const DirectX::SimpleMath::Matrix& view = camera->GetView();

	// プロジェクション
	const DirectX::SimpleMath::Matrix& projection = camera->GetProj();

	ImGuizmo::SetRect(
		pos.x,
		pos.y,
		size.x,
		size.y
	);

	ImGuizmo::OPERATION ope =
		(m_drawFlag & DRAW_TRANSLATION ? ImGuizmo::TRANSLATE : static_cast<ImGuizmo::OPERATION>(0)) |
		(m_drawFlag & DRAW_ROTATION ? ImGuizmo::ROTATE : static_cast<ImGuizmo::OPERATION>(0)) |
		(m_drawFlag & DRAW_SCALE ? ImGuizmo::SCALE : static_cast<ImGuizmo::OPERATION>(0));

	// 表示
	ImGuizmo::Manipulate(
		&view._11,
		&projection._11,
		ope,
		ImGuizmo::LOCAL,
		&world._11
	);

	// 変更された場合
	if (ImGuizmo::IsUsing())
	{
		transform->SetWorldMatrix(world);
	}
}

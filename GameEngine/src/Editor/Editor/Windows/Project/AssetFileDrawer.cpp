//====================================================//
// ファイル名  : AssetFileDrawer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/08
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "AssetFileDrawer.h"

#include "ThirdParty/imgui/imgui.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Managers/AssetManager.h"
#include "Editor/Editor/HandlePayload.h"

//====================================================//
// 関数の実体宣言
//====================================================//

REngine::AssetFileDrawer::Result REngine::AssetFileDrawer::DrawAssetFile(const std::filesystem::path& path, UUID selected)
{
	Result r{};

	// ファイル名を取得
	std::string fileName = path.filename().string();

	// 初期状態のフラグ
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	// UUIDを取得
	UUID id = m_assetManager.GetDataBase().GetUUID(path);

	// 選択されていれば
	if (selected == id)
	{
		// 選択状態に
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	// SubAssetを持っていなければ▼を表示しない
	if (!m_assetManager.HaveSubAsset(path))
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	// 拡張可能なツリーを展開
	bool open = ImGui::TreeNodeEx(fileName.c_str(), flags);

	// クリックされたら選択
	if (ImGui::IsItemClicked())
	{
		// 選択
		r.select = id;

		// 読み込み
		m_assetManager.LoadFromUUID(id);
	}

	// 左ボタンがダブルクリックされていれば
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		// シーン以外なら
		if (path.extension() != L".scene")
			// 選択状態にする
			r.doubleClick = id;

		//// シーンなら
		//else
			//// Todo: ロードする
	}

	// ドラッグ可能に
	if (ImGui::BeginDragDropSource())
	{
		// UUIDからハンドルを取得
		UnTypeHandle handle = m_assetManager.LoadFromUUID(id);

		// 無効ハンドルであればドラッグ不可
		if (handle != ERROR_UNTYPE_HANDLE)
		{
			// 受け渡し構造体を生成
			HandlePayload payload = { m_assetManager.GetTypeManager().GetAssetClass(path), handle };

			// データを設定
			ImGui::SetDragDropPayload("ASSET", &payload, sizeof(payload));

			// ドラッグ中に表示される内容
			ImGui::Text(path.stem().string().c_str());
		}

		// ドラッグの終了
		ImGui::EndDragDropSource();
	}

	if (open)
	{
		// Auxを取得
		auto aux = m_assetManager.GetDataBase().GetAux(m_assetManager.GetDataBase().GetUUID(path));

		// サブアセットの分ループ
		for (auto& subAsset : aux.subAssets)
		{
			// 選択されたら
			if (ImGui::Selectable(subAsset.name.c_str(), subAsset.uuid == selected))
			{
				r.select = subAsset.uuid;
			}

			// 左ボタンがダブルクリックされていれば
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				// 選択状態にする
				r.doubleClick = subAsset.uuid;
			}
		
			// 仮想パスを作成
			std::filesystem::path subPath = path.string() + "#" + subAsset.name;

			// ドラッグ可能に
			if (ImGui::BeginDragDropSource())
			{
				// パスからUUIDを取得
				UnTypeHandle handle = m_assetManager.LoadFromUUID(m_assetManager.GetDataBase().GetUUID(subPath));

				// 無効ハンドルであればドラッグ不可
				if (handle != ERROR_UNTYPE_HANDLE)
				{
					// 受け渡し構造体を生成
					HandlePayload payload = { m_assetManager.GetTypeManager().GetAssetClassFromType(subAsset.assetType), handle };

					// データを設定
					ImGui::SetDragDropPayload("ASSET", &payload, sizeof(payload));

					// ドラッグ中に表示される内容
					ImGui::Text(subAsset.name.c_str());
				}

				// ドラッグの終了
				ImGui::EndDragDropSource();
			}
		}

		// ツリーを終了
		ImGui::TreePop();
	}

	return r;
}

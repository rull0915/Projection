//====================================================//
// ファイル名   : DrawMeshCommandPublisher.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/01
//
// 概要 : モデルを描画のコマンドを発行するクラス
//
// 更新履歴 :
// 2026/05/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Renderer/Command/DrawCommandContainer.h"
#include "Renderer/GraphicsSystem.h"
#include "Assets/Types/Model/Mesh.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class DrawMeshCommandPublisher
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 描画の状態
		DrawCommandContainer& m_commandContainer;

		// システム
		GraphicsSystem& m_graphicSystem;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		DrawMeshCommandPublisher(DrawCommandContainer& container, GraphicsSystem& graphicSystem)
			: m_commandContainer{ container }
			, m_graphicSystem{ graphicSystem }
		{}
		~DrawMeshCommandPublisher() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
		void DrawMesh(Mesh* mesh, size_t subMeshIndex, DirectX::SimpleMath::Matrix world)
		{
			// コマンドの生成
			auto& command = m_commandContainer.AddModel();

			command.pMesh = mesh;
			command.subMeshIndex = subMeshIndex;
			command.world = world;
			command.material = m_graphicSystem.GetMaterial();
		}
	};
}	// namespace REngine

//====================================================//
// ファイル名   : MeshFilter.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/08
//
// 概要 : オブジェクトが使うメッシュを保持するコンポーネント
//
// 更新履歴 :
// 2026/10/08 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(MeshFilter)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Assets/Types/Model/Mesh.h"
#include "Assets/Objects/Handle.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class MeshFilter : public REngine::WorldComponentBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// メッシュアセット
		Handle<Mesh> m_mesh;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		MeshFilter(REngine::IComponentOwner* own)
			: WorldComponentBase(own)
			, m_mesh{ ERROR_HANDLE<Mesh> }
		{
			ADD_PROPERTY(MeshFilter, m_mesh);
		}
		~MeshFilter() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(MeshFilter, REngine::WorldComponentBase);

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// メッシュのゲッター
		Handle<Mesh> GetMesh() const { return m_mesh; }
		
		// メッシュのセッター
		void SetMesh(Handle<Mesh> mesh) { m_mesh = mesh; }
	};
}

//====================================================//
// ファイル名   : RendererManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/03
//
// 概要 : 描画コンポーネントを管理するクラス
//
// 更新履歴 :
// 2026/05/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <unordered_set>
#include "Components/World/Renderer/RendererBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class RendererManager
	{
	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------


	   //-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 登録予約中のRenderer
		std::vector<RendererBase*> m_addReserves;
		std::unordered_set<RendererBase*> m_removeReserves;

		// 登録されているRendererBase
		std::vector<RendererBase*> m_renderers;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		RendererManager();
		~RendererManager();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 描画処理
		void DrawAll(Renderer& renderer);

		// 登録予約
		void AddRenderer(RendererBase* r) { m_addReserves.push_back(r); }
		void RemoveRenderer(RendererBase* r) { m_removeReserves.insert(r); }

		// 予約反映
		void ReflectReserves()
		{
			AddReserved();
			RemoveReserved();
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// 予約済みポインタの追加
		void AddReserved()
		{
			for (auto p : m_addReserves)
			{
				m_renderers.push_back(p);
			}

			m_addReserves.clear();
		}

		// 予約済みポインタの削除
		void RemoveReserved()
		{
			// 削除リストが空なら何もしない
			if (m_removeReserves.empty()) return;

			// 削除リストに含まれているかを調べるラムダ式
			auto shouldRemove = [this](RendererBase* base)
				{
					return m_removeReserves.contains(base);
				};

			// 削除
			std::erase_if(m_renderers, shouldRemove);
			std::erase_if(m_addReserves, shouldRemove);

			// 削除リストをクリア
			m_removeReserves.clear();
		}
	};
}	// namespace REngine

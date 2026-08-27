//====================================================//
// ファイル名   : UIManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/24
//
// 概要 : UI管理クラス
// 　　   Canvasを保持する
//
// 更新履歴 :
// 2026/05/24 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <unordered_set>

#include "Components/UI/Canvas.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class Renderer;
	class Scene;

	//====================================================//
	// クラス宣言
	//====================================================//
	class UIManager
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 予約中のキャンバス
		std::vector<Canvas*> m_addReserves;
		std::unordered_set<Canvas*> m_removeReserves;

		// 登録されているキャンバス
		std::vector<Canvas*> m_canvases;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		UIManager(Scene* pScene);
		~UIManager();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void CheckEvent();

		void Draw(Renderer& renderer);

		// マウスとの衝突を調べる関数
		void CheckHitRay(DirectX::SimpleMath::Vector2 position);

		// デバッグ描画関数
		void DebugDraw(Renderer& renderer, DirectX::SimpleMath::Color color);

		// リセット
		void Reset()
		{
			m_addReserves.clear();
			m_canvases.clear();
		}

		//-----------------------------------------------------
		// 予約関連
		//-----------------------------------------------------

		// 登録予約
		void AddCanvas(Canvas* c) { m_addReserves.push_back(c); }
		void RemoveCanvas(Canvas* c) { m_removeReserves.insert(c); }

		// 予約済みポインタの追加
		void AddReserved()
		{
			for (auto p : m_addReserves)
			{
				m_canvases.push_back(p);
			}

			m_addReserves.clear();
		}

		// 予約済みポインタの削除
		void RemoveReserved()
		{
			// 削除リストが空なら何もしない
			if (m_removeReserves.empty()) return;

			// 削除リストに含まれているかを調べるラムダ式
			auto shouldRemove = [this](Canvas* base)
				{
					return m_removeReserves.contains(base);
				};

			// 削除
			std::erase_if(m_canvases, shouldRemove);
			std::erase_if(m_addReserves, shouldRemove);

			// 削除リストをクリア
			m_removeReserves.clear();
		}

		// 予約反映
		void ReflectReserves()
		{
			AddReserved();
			RemoveReserved();
		}

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// キャンバスの並び順をソートする関数
		void SortCanvas();
	};
}	// namespace REngine

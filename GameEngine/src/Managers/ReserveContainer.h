//====================================================//
// ファイル名   : ReserveContainer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/02
//
// 概要 : オブジェクトの追加/削除を遅延させるコンテナクラス
//
// 更新履歴 :
// 2026/10/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <unordered_set>

//====================================================//
// クラス宣言
//====================================================//
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
	template<typename T>
	class ReserveContainer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 本リスト
		std::vector<T*> m_objectList;

		// 追加予約リスト
		std::vector<T*> m_addReserves;

		// 削除予約リスト
		std::unordered_set<T*> m_removeReserves;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ReserveContainer()
			: m_objectList{}
			, m_addReserves{}
			, m_removeReserves{}
		{
		}

		virtual ~ReserveContainer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// オブジェクトの追加
		void AddObject(T* c) { m_addReserves.push_back(c); }

		// オブジェクトの削除
		void RemoveObject(T* c) { m_removeReserves.insert(c); }

		// 予約済みポインタの追加
		virtual void AddReserved()
		{
			for (auto p : m_addReserves)
			{
				m_objectList.push_back(p);
			}

			m_addReserves.clear();
		}

		// 予約済みポインタの削除
		virtual void RemoveReserved()
		{
			// 削除リストが空なら何もしない
			if (m_removeReserves.empty()) return;

			// 削除リストに含まれているかを調べるラムダ式
			auto shouldRemove = [this](T* base)
				{
					return m_removeReserves.contains(base);
				};

			// 削除
			std::erase_if(m_objectList, shouldRemove);		
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

		// リセット
		void Reset()
		{
			m_addReserves.clear();
			m_objectList.clear();
			m_removeReserves.clear();
		}

	protected:

		// 予約リストを取得する関数
		std::vector<T*>& GetAddReserves()
		{
			return m_addReserves;
		}
		std::unordered_set<T*>& GetRemoveReserves()
		{
			return m_removeReserves;
		}

		// オブジェクトリストを取得する関数
		std::vector<T*>& GetObjects()
		{
			return m_objectList;
		}
		const std::vector<T*>& GetConstObjects() const
		{
			return m_objectList;
		}
	};
}	// namespace REngine

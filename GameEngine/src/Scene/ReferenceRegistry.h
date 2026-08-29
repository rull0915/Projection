//====================================================//
// ファイル名   : ReferenceRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/29
//
// 概要 : Refを使った参照関係の管理をするクラス
//
// 更新履歴 :
// 2026/08/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>
#include "Common/ObjectReference.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ReferenceRegistry
	{
	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------


		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 対応表
		std::unordered_map<RefBase*, PropertyObject*> m_refs;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ReferenceRegistry() = default;
		~ReferenceRegistry()
		{
			// 全無効化
			for (auto& ref : m_refs)
			{
				ref.first->Invalidate();
			}
		}

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 追加
		void AddPair(RefBase* ref, PropertyObject* obj)
		{
			// リストに追加
			m_refs[ref] = obj;

			// 削除時コールバックを設定
			ref->SetDestroyCallBack([this](RefBase* base) { this->RemoveRef(base); });
		}

		// 解除関数
		void RemoveRef(RefBase* ref)
		{
			m_refs.erase(ref);
		}
		void RemoveObj(PropertyObject* obj)
		{
			// 検索
			std::erase_if(m_refs, [obj](const auto& pair)
				{
					// 削除対象なら
					if (pair.second == obj)
					{
						// 無効化
						pair.first->Invalidate();

						return true;
					}
					return false;
				});
		}
	};
}

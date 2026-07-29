//====================================================//
// ファイル名   : AssetRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : Assetの所有者 生成や破棄を管理します
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <memory>

#include "Assets/Objects/Handle.h"
#include "Assets/Objects/AssetBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetRegistry
	{
	private:

		// スロット構造体
		struct Slot
		{
			// Asset本体
			std::unique_ptr<AssetBase> asset;

			// 世代
			uint32_t generation = 1;

			// 現在有効かどうか
			bool isValid = false;

			// UUID 
			UUID uuid = 0;
		};

	private:
		// Slotの配列
		std::vector<Slot> m_slots;

		// 現在使われていないスロットをまとめた配列(配列に含まれるインデックスが現在未使用)
		std::vector<uint32_t> m_freeIndexList;

	public:
		// アセットを登録する関数
		UnTypeHandle Register(UUID uuid)
		{
			// インデックス
			uint32_t index = 0;

			// 未使用リストが空なら新たに作る
			if (m_freeIndexList.empty())
			{
				// インデックスを末尾に
				index = static_cast<uint32_t>(m_slots.size());

				// 空のスロットを生成
				m_slots.push_back({});
			}
			// 未使用インデックスが存在するなら
			else
			{
				// 最新のインデックスを使用する
				index = m_freeIndexList.back();

				// リストから削除する
				m_freeIndexList.pop_back();
			}

			// ロード前なのでnullptrにしておく
			m_slots[index].asset = nullptr;

			// 有効フラグを立てる
			m_slots[index].isValid = true;

			// UUIDを設定する
			m_slots[index].uuid = uuid;

			// ハンドルを生成し返す
			return UnTypeHandle{ index, m_slots[index].generation };
		}

		// アセットを破棄する関数
		void Release(uint32_t index)
		{
			// 破棄するスロットを取得
			auto& slot = m_slots[index];

			// アセット本体を解放
			slot.asset.reset();

			// 無効化
			slot.isValid = false;

			// 世代を進めることで古いハンドルを無効化
			slot.generation++;

			// 未使用リストに追加
			m_freeIndexList.push_back(index);
		}

		// アセットを置き換える関数 (既存のハンドルを無効化しない)
		void Replace(uint32_t index, std::unique_ptr<AssetBase> newAsset)
		{
			m_slots[index].asset = std::move(newAsset);
			m_slots[index].asset->SetUUID(m_slots[index].uuid);
		}

		// ハンドルからアセットを取得する関数
		template<typename T>
		T* Get(Handle<T> handle)
		{
			// 無効なインデックスを指していた場合nullptrを返す
			if (handle.index >= m_slots.size()) return nullptr;

			// スロットを取得
			auto& slot = m_slots[handle.index];

			// 異なる世代のハンドルだった場合nullptrを返す
			if (handle.generation != slot.generation) return nullptr;

			// アセットを返す
			return static_cast<T*>(slot.asset.get());
		}

		// タイプ識別なしのハンドルからアセットを取得する関数
		AssetBase* GetFromUnTypeHandle(UnTypeHandle handle) const
		{
			// 無効なインデックスを指していた場合nullptrを返す
			if (handle.index >= m_slots.size()) return nullptr;

			// スロットを取得
			auto& slot = m_slots[handle.index];

			// 異なる世代のハンドルだった場合nullptrを返す
			if (handle.generation != slot.generation) return nullptr;

			// アセットを返す
			return slot.asset.get();
		}
	};
}	// namespace REngine

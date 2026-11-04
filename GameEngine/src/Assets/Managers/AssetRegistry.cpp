/**
 * @file    AssetRegistry.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 */

#include "pch.h"
#include "Assets/Managers/AssetRegistry.h"

namespace RHEngine
{
	AssetRegistry::AssetRegistry()
		: m_slots{}
		, m_freeIndexList{}
		, m_uuidToIndex{}
		, m_mutex{}
	{}

	AssetRegistry::~AssetRegistry()
	{}

	UnTypeHandle AssetRegistry::Register(UUID uuid)
	{
		// 非同期実行されるためロック
		std::lock_guard<std::mutex> lock(m_mutex);

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

		// 変換マップに追加
		m_uuidToIndex[uuid] = index;

		// ハンドルを生成し返す
		return UnTypeHandle{ index, m_slots[index].generation };
	}

	void AssetRegistry::Release(uint32_t index)
	{
		// 非同期実行されるためロック
		std::lock_guard<std::mutex> lock(m_mutex);

		// 無効インデックスならスキップ
		if (index >= m_slots.size() || !m_slots[index].isValid) return;

		// 破棄するスロットを取得
		auto& slot = m_slots[index];

		// マップから削除
		m_uuidToIndex.erase(slot.uuid);

		// アセット本体を解放
		slot.asset.reset();

		// 無効化
		slot.isValid = false;

		// 世代を進めることで古いハンドルを無効化
		slot.generation++;

		// 未使用リストに追加
		m_freeIndexList.push_back(index);
	}

	void AssetRegistry::Replace(uint32_t index, std::unique_ptr<AssetBase> newAsset)
	{
		// 非同期実行されるためロック
		std::lock_guard<std::mutex> lock(m_mutex);

		if (newAsset)
		{
			m_slots[index].type = &typeid(*newAsset);
			m_slots[index].asset = std::move(newAsset);
			m_slots[index].asset->SetUUID(m_slots[index].uuid);
		}
		else
		{
			m_slots[index].type = &typeid(void);
			m_slots[index].asset = nullptr;
		}
	}

	AssetBase* AssetRegistry::GetFromUnTypeHandle(UnTypeHandle handle) const
	{
		// 無効なインデックスを指していた場合nullptrを返す
		if (handle.index >= m_slots.size()) return nullptr;

		// スロットを取得
		auto& slot = m_slots[handle.index];

		// 無効スロットだった場合nullptrを返す
		if (!slot.isValid) return nullptr;

		// 異なる世代のハンドルだった場合nullptrを返す
		if (handle.generation != slot.generation) return nullptr;

		// アセットを返す
		return slot.asset.get();
	}

	UnTypeHandle AssetRegistry::GetHandle(UUID uuid) const
	{
		// UUIDで検索
		auto it = m_uuidToIndex.find(uuid);

		// 未登録UUIDならエラーハンドル
		if (it == m_uuidToIndex.end()) return ERROR_UNTYPE_HANDLE;

		// インデックスを取得
		uint32_t index = it->second;

		// ハンドルを構築して返す
		return UnTypeHandle{ index, m_slots[index].generation };
	}

	UUID AssetRegistry::GetUUID(const UnTypeHandle& handle) const
	{
		// 範囲外ハンドルならエラーID
		if (handle.index >= m_slots.size()) return UUID_NONE;

		// スロットを取得
		const auto& slot = m_slots[handle.index];

		// 無効スロットならエラーID
		if (!slot.isValid || slot.generation != handle.generation) return 0;

		// UUIDを取得し返す
		return slot.uuid;
	}

	void AssetRegistry::Reset()
	{
		m_slots.clear();

		m_freeIndexList.clear();

		m_uuidToIndex.clear();
	}
}	// namespace RHEngine

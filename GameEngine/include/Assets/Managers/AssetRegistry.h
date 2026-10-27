/**
 * @file    AssetRegistry.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   Assetの所有者 生成や破棄を管理します
 */

#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>
#include <typeinfo>
#include <unordered_map>

#include "Assets/Objects/Handle.h"
#include "Assets/Objects/AssetBase.h"
#include "IAssetResolver.h"

namespace RHEngine
{
	/**
	 * @brief アセットを所有し生成や破棄を管理する
	 */
	class AssetRegistry : public IAssetResolver
	{
	private:

		/**
		 * @brief アセットを保持するスロット
		 */
		struct Slot
		{
			std::unique_ptr<AssetBase> asset{};	///< アセット本体

			uint32_t generation = 1;	///< 世代番号

			bool isValid = false;		///< 有効なスロットかどうか

			UUID uuid = 0;	///< 対応するUUID

			const std::type_info* type = &typeid(void);	///< アセットのタイプ情報
		};

	private:

		/** @brief 管理中スロットのリスト */
		std::vector<Slot> m_slots;

		/** @brief 未使用スロットのリスト (配列に含まれるインデックスが現在未使用) */
		std::vector<uint32_t> m_freeIndexList;

		/** @brief UUIDからインデックスの変換マップ */
		std::unordered_map<UUID, uint32_t> m_uuidToIndex;

		/** @brief m_freeIndexListを保護するmutex */
		std::mutex m_mutex;

	public:
		/// @name ライフサイクル
		/// @{

		AssetRegistry();

		~AssetRegistry();

		/// @}

		/**
		 * @brief アセットの登録をする
		 * @param uuid 登録するUUID
		 * @return アセットに対応するハンドル
		 */
		UnTypeHandle Register(UUID uuid);

		/**
		 * @brief アセットを破棄する
		 * @param index 破棄するスロット番号
		 */
		void Release(uint32_t index);

		/**
		 * @brief アセットを置き換える
		 * @detalis 既存のアセットを無効化せず置き換える
		 * @param index 置き換えるインデックス
		 * @param newAsset 置き換え後のアセット
		 */
		void Replace(uint32_t index, std::unique_ptr<AssetBase> newAsset);

		/**
		 * @brief ハンドルからアセット本体を取得する
		 * @tparam T アセットの型
		 * @param handle 取得対象のハンドル
		 * @return 対応するアセット本体
		 */
		template<typename T>
		T* Get(Handle<T> handle) const;

		/**
		 * @brief ハンドルから本体を取得する (タイプ識別なし)
		 * @param handle 取得対象のハンドル
		 * @return アセット本体
		 */
		AssetBase* GetFromUnTypeHandle(UnTypeHandle handle) const;

		/**
		 * @brief UUIDからハンドルを取得する
		 * @param uuid 取得対象のUUID
		 * @return 対応するハンドル
		 */
		UnTypeHandle GetHandle(UUID uuid) const override;

		/**
		 * @brief ハンドルからUUIDを取得する
		 * @param handle 取得対象のハンドル
		 * @return 対応するUUID
		 */
		UUID GetUUID(const UnTypeHandle& handle) const override;

		/**
		 * @brief このクラスをリセットする
		 */
		void Reset();
	};

	template<typename T>
	inline T* AssetRegistry::Get(Handle<T> handle) const
	{
		// タイプ
		static const std::type_info* type = &typeid(T);

		// 無効なインデックスを指していた場合nullptrを返す
		if (handle.index >= m_slots.size()) return nullptr;

		// スロットを取得
		auto& slot = m_slots[handle.index];

		// タイプが異なる場合nullptrを返す
		if (slot.type != type) return nullptr;

		// 無効スロットだった場合nullptrを返す
		if (!slot.isValid) return nullptr;

		// 異なる世代のハンドルだった場合nullptrを返す
		if (handle.generation != slot.generation) return nullptr;

		// アセットを返す
		return static_cast<T*>(slot.asset.get());
	}
}	// namespace RHEngine

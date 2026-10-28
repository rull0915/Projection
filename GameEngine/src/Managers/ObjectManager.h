/**
 * @file    ObjectManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/07
 *
 * @brief   オブジェクト管理クラス
 */

#pragma once

#include <vector>
#include <memory>

#include "GameObject/GameObject.h"

namespace RHEngine
{
	class GameTimer;

	/**
	 * @brief オブジェクト管理クラス
	 */
	class ObjectManager
	{
	private:

		/** @brief 生成予約中のオブジェクトリスト */
		std::vector<std::unique_ptr<GameObject>> m_reservations;

		/** @brief 管理中のオブジェクトリスト */
		std::vector<std::unique_ptr<GameObject>> m_objects;

	public:
		
		/// @name ライフサイクル
		/// @{

		ObjectManager();

		~ObjectManager();

		/// @}

		/**
		 * @brief 更新関数
		 * @param gameTimer タイマー
		 * @param playing 再生中かどうか
		 */
		void Update(const GameTimer& gameTimer, bool playing);

		/**
		 * @brief 遅延更新関数
		 * @param gameTimer タイマー
		 * @param playing 再生中かどうか
		 */
		void LateUpdate(const GameTimer& gameTimer, bool playing);

		/**
		 * @brief 死亡コンポーネントを削除する
		 */
		void RemoveDeadComponent();

		/**
		 * @brief 終了時に全てのオブジェクトを解放する
		 */
		void Finalize();

		/**
		 * @brief 衝突判定で蓄積したキャッシュを適用する
		 */
		void AllReflectCache();

		/**
		 * @brief オブジェクトを登録対象に追加する
		 * @param object 
		 */
		void AddObject(GameObject* object)
		{
			// 予約リストに追加
			m_reservations.push_back(std::unique_ptr<GameObject>(object));
		}

		/**
		 * @brief オブジェクトを削除する
		 * @param object 
		 */
		void Destroy(GameObject* object)
		{
			// 削除フラグを立てる
			object->Destroy();
		}

		/**
		 * @brief 全オブジェクトを削除する
		 */
		void AllDestroy();

		/**
		 * @brief 死亡オブジェクトを全て削除する
		 */
		void RemoveDeadObject();

		/**
		 * @brief 全てのオブジェクトを取得する
		 */
		const std::vector<std::unique_ptr<GameObject>>& GetAllObject() const { return m_objects; }

		/**
		 * @brief 全ての追加予約リストを取得する
		 */
		const std::vector<std::unique_ptr<GameObject>>& GetAllReserves() const { return m_reservations; }

	private:

		/**
		 * @brief 追加予約を反映する
		 */
		void AddReservedObject();
	};
}	// namespace RHEngine

//====================================================//
// ファイル名   : ComponentContainer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要 : コンポーネントを保持するクラス
//
// 更新履歴 :
// 2026/05/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>
#include <vector>
#include <unordered_map>

#include "Common/TypeIdGenerator.h"
#include "Components/World/Transform/Transform.h"
#include "Components/UI/RectTransform/RectTransform.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class Scene;
	class IComponentOwner;

	//====================================================//
	// クラス宣言
	//====================================================//

	class ComponentContainer
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 自身を持つシーンのポインタ
		Scene* m_pScene;

		// トランスフォーム
		std::unique_ptr<Transform> m_pTransform;
		// UIトランスフォーム
		std::unique_ptr<RectTransform> m_pRectTransform;

		// その他コンポーネント
		std::vector<std::unique_ptr<ComponentBase>> m_addReserves;
		std::unordered_set<ComponentBase*> m_pDestroyReserves;	// 重複を防止するためunordered_setを使用
		std::vector<std::unique_ptr<ComponentBase>> m_pComponents;

		// コンポーネントリストのキャッシュ
		std::unordered_map<ComponentBase::TypeId, std::vector<ComponentBase*>> m_componentsCache;

		IComponentOwner* m_pOwner;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ComponentContainer(IComponentOwner* owner)
			: m_pTransform{ nullptr }
			, m_addReserves{}
			, m_pDestroyReserves{}
			, m_pComponents{}
			, m_pScene{ nullptr }
			, m_pOwner{ owner }
		{}
		~ComponentContainer() = default;

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------
		void SetScene(Scene* scene) { m_pScene = scene; }

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 全予約コンポーネントのAwakeを呼ぶ関数
		void AwakeComponets()
		{
			for (auto& component : m_addReserves)
			{
				// 更新処理
				component->Awake();

				// Activeなら
				if (component->IsActive())
				{
					component->OnEnable();
				}
			}
		}

		// 全コンポーネントを更新する関数
		void UpdateComponents(const GameTimer& gameTimer)
		{
			for (auto& component : m_pComponents)
			{
				if (!component->IsActive()) continue;

				// 初回更新の時
				if (!component->IsStarted())
				{
					// Start関数呼び出し
					component->Start();
					component->SetStart();
				}

				// 更新処理
				component->Update(gameTimer);
			}
		}

		// 遅延更新
		void LateUpdateComponents(const GameTimer& gameTimer)
		{
			for (auto& component : m_pComponents)
			{
				if (!component->IsActive()) continue;

				// 遅延更新処理
				component->LateUpdate(gameTimer);
			}
		}

		// 自身がどのタイプか調べる関数
		bool IsUI() const { return m_pRectTransform && !m_pTransform; }			// UIオブジェクト	
		bool IsWorld() const { return m_pTransform && !m_pRectTransform; }		// Worldオブジェクト

		// 有効なオブジェクトかどうか(どちらかのタイプである)
		bool IsValid() const { return IsUI() ^ IsWorld(); }

		// まだ未設定である
		bool IsNotSet() const { return !m_pTransform && !m_pRectTransform; }

		// コンポーネント空間と自身の空間が等しいか調べる
		bool IsCorrect(ComponentSpace space) const
		{
			// 無効オブジェクトなら
			if (!IsValid()) return false;

			// Both
			if (space == ComponentSpace::Both) return true;

			// UI判定
			if (space == ComponentSpace::UI && IsUI()) return true;

			// World判定
			if (space == ComponentSpace::World && IsWorld()) return true;

			// 正しくない
			return false;
		}

		// ===================== Component操作関数 ===================== //

		// ---------- Get ---------- //

		ComponentBase* Get(ComponentBase::TypeId id) const
		{
			// Transformは専用ポインタから早期リターン
			if (id == Transform::StaticTypeId())
			{
				return m_pTransform.get();
			}
			// RectTransformも同様
			else if (id == RectTransform::StaticTypeId())
			{
				return m_pRectTransform.get();
			}
			else 
			{
				// それ以外はキャッシュから取得
				auto it = m_componentsCache.find(id);

				// 指定されたidの配列があり1つ以上コンポーネントがあれば
				if (it != m_componentsCache.end() && it->second.size() > 0)
				{
					return it->second[0];
				}
			}

			return nullptr;
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* Get() const
		{
			return static_cast<T*>(Get(T::StaticTypeId()));
		}

		// ---------- Gets ---------- //

		const std::vector<ComponentBase*>& Gets(ComponentBase::TypeId id) const
		{
			// 存在しなかったとき用の空配列
			static const std::vector<ComponentBase*> empty{};

			// イテレータを取得
			auto it = m_componentsCache.find(id);

			return it != m_componentsCache.end() ? it->second : empty;
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		const std::vector<ComponentBase*>& Gets() const
		{
			return Gets(T::StaticTypeId());
		}

		// ---------- GetAll ---------- //

		std::vector<ComponentBase*> GetAll() const
		{
			return Gets(ComponentBase::StaticTypeId());
		}

		// ----------- Add ----------- //

		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* Add()
		{
			T* add = nullptr;

			// トランスフォームの場合
			if constexpr (std::is_same_v<T, Transform>)
			{
				// まだオブジェクトタイプが未設定なら
				if (IsNotSet())
				{
					// Transformを生成
					m_pTransform = std::make_unique<Transform>(m_pOwner);

					// 初期化
					m_pTransform.get()->UpdateCache();
				}
				// 既に確定していたら追加不可
				return m_pTransform.get();
			}

			// RectTransformの場合
			else if constexpr (std::is_same_v<T, RectTransform>)
			{
				// まだオブジェクトタイプが未設定なら
				if (IsNotSet())
				{
					// RectTransformを生成
					m_pRectTransform = std::make_unique<RectTransform>(m_pOwner);

					// 初期化
					m_pRectTransform.get()->UpdateCache();
				}
				// 既に確定していたら追加不可
				return m_pRectTransform.get();
			}

			else
			{
				// ユニークポインタを生成
				auto comp = std::make_unique<T>(m_pOwner);
				T* ptr = comp.get();

				// オブジェクトの所属空間と追加コンポーネントの所属空間が同じか調べる
				if (IsCorrect(comp->SPACE))
				{
					// 配列に追加
					m_addReserves.push_back(std::move(comp));

					add = ptr;
				}

				// 違っていたら追加不可
				else return nullptr;
			}
			
			// キャッシュに追加
			RegistComponentToCache(add);

			// 追加したコンポーネントを返す
			return add;
		}

		// ---------- Remove ---------- //

		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void Remove()
		{
			// リストを取得
			auto it = m_componentsCache.find(T::StaticTypeId());

			// あれば
			if (it != m_componentsCache.end() && it->second.size() > 0)
			{
				// 最初の要素を削除リストに追加
				m_pDestroyReserves.push_back(it->second[0]);
			}
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void Removes()
		{
			// リストを取得
			auto it = m_componentsCache.find(T::StaticTypeId());

			// あれば
			if (it != m_componentsCache.end())
			{
				// 全て削除リストに追加
				for (auto& comp : it->second)
				{
					m_pDestroyReserves.push_back(comp);
				}
			}
		}

		void Remove(ComponentBase::TypeId id)
		{
			// リストを取得
			auto it = m_componentsCache.find(id);

			// あれば
			if (it != m_componentsCache.end() && it->second.size() > 0)
			{
				// 最初の要素を削除リストに追加
				m_pDestroyReserves.insert(it->second[0]);
			}
		}

		void Removes(ComponentBase::TypeId id)
		{
			// リストを取得
			auto it = m_componentsCache.find(id);

			// あれば
			if (it != m_componentsCache.end())
			{
				// 全て削除リストに追加
				for (auto& comp : it->second)
				{
					m_pDestroyReserves.insert(comp);
				}
			}
		}

		void AllRemove()
		{
			for (auto& comp : m_pComponents)
			{
				// 削除リストに追加
				m_pDestroyReserves.insert(comp.get());
			}
		}

		// 予約済みのコンポーネントを登録する関数
		void RegisterComponents();

		// 削除予約済みのコンポーネントを削除する関数
		void RemoveRegistered();

	private:

		// シーンにコンポーネントを登録する関数
		void RegisterComponentToScene(ComponentBase* component);

		// シーンからコンポーネントを登録解除する関数
		void UnRegisterComponentToScene(ComponentBase* component);

		// キャッシュにコンポーネントを登録する関数
		void RegistComponentToCache(ComponentBase* component)
		{
			// キャッシュリストを格納する配列を用意
			std::vector<ComponentBase::TypeId> ids;

			// 対応するIDの一覧を取得
			component->CollectTypeIds(ids);

			// 全要素に自身を追加
			for (auto id : ids)
			{
				m_componentsCache[id].push_back(component);
			}
		}

		// キャッシュからコンポーネントを削除する関数
		void UnRegistComponentToCache(ComponentBase* component)
		{
			// キャッシュリストを格納する配列を用意
			std::vector<ComponentBase::TypeId> ids;

			// 対応するIDの一覧を取得
			component->CollectTypeIds(ids);

			// 全要素から自身を削除
			for (auto id : ids)
			{
				// リストのイテレータを取得
				auto listIt = m_componentsCache.find(id);

				// 存在しないなら次のリストへ
				if (listIt == m_componentsCache.end())
					continue;

				// イテレータから本体を取得
				auto& list = listIt->second;

				// 自身が格納されているイテレータを取得
				auto it = std::find(list.begin(), list.end(), component);

				// 見つかれば
				if (it != list.end())
				{
					// 削除
					list.erase(it);
				}
			}
		}
	};
} // namespace REngine

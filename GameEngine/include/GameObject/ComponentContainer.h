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

#include "System/TypeIdGenerator.h"
#include "Components/World/Transform/Transform.h"
#include "Components/UI/RectTransform/RectTransform.h"

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
	std::vector<std::unique_ptr<ComponentBase>> m_pReservesAdd;
	std::vector<ComponentBase*> m_pDestroyReserves;
	std::vector<std::unique_ptr<ComponentBase>> m_pComponents;

	IComponentOwner* m_pOwner;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ComponentContainer(IComponentOwner* owner)
		: m_pTransform{ nullptr }
		, m_pReservesAdd{}
		, m_pDestroyReserves{}
		, m_pComponents{}
		, m_pScene{ nullptr }
		, m_pOwner{ owner }
	{
	}
	~ComponentContainer() = default;

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------
	void SetScene(Scene* scene) { m_pScene = scene; }

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 全コンポーネントを更新する関数
	void UpdateComponets(const GameTimer& gameTimer)
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

		// UI判定
		if (space == ComponentSpace::UI && IsUI()) return true;

		// World判定
		if (space == ComponentSpace::World && IsWorld()) return true;

		// 正しくない
		return false;
	}

	// ===================== Component操作関数 ===================== //

	// ---------- Get ---------- //

	ComponentBase* Get(unsigned int id) const
	{
		// Transformは専用ポインタから早期リターン
		if (id == TypeIDGenerator::GetID<Transform>())
		{
			return m_pTransform.get();
		}
		// RectTransformも同様
		else if (id == TypeIDGenerator::GetID<RectTransform>())
		{
			return m_pRectTransform.get();
		}

		else {

			// それ以外は ID 比較でループ
			for (auto& comp : m_pComponents)
			{
				// IDで比較する
				if (comp->GetID() == id)
				{
					return comp.get();
				}
			}
			// 予約リストをチェック
			for (auto& comp : m_pReservesAdd)
			{
				// IDで比較する
				if (comp->GetID() == id)
				{
					return comp.get();
				}
			}

			return nullptr;
		}
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	T* Get() const
	{
		return static_cast<T*>(Get(TypeIDGenerator::GetID<T>()));
	}

	// ---------- Gets ---------- //

	std::vector<ComponentBase*> Gets(unsigned int id) const
	{
		std::vector<ComponentBase*> components;
		Gets(id, components);
		return components;
	}

	void Gets(unsigned int id, std::vector<ComponentBase*>& array) const
	{
		// 配列の初期化
		array.clear();

		// Transformは専用ポインタから即追加
		if (id == TypeIDGenerator::GetID<Transform>())
		{
			if (m_pTransform) array.push_back(m_pTransform.get());
		}
		// RectTransformも同様
		else if (id == TypeIDGenerator::GetID<RectTransform>())
		{
			if (m_pRectTransform) array.push_back(m_pRectTransform.get());
		}

		else
		{
			// それ以外は ID 比較でループ
			for (auto& comp : m_pComponents)
			{
				// IDで比較する
				if (comp->GetID() == id)
				{
					array.push_back(comp.get());
				}
			}
			// 予約リストをチェック
			for (auto& comp : m_pReservesAdd)
			{
				// IDで比較する
				if (comp->GetID() == id)
				{
					array.push_back(comp.get());
				}
			}
		}

		return;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	std::vector<T*> Gets() const
	{
		std::vector<T*> array;

		Gets<T>(array);

		return array;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void Gets(std::vector<T*>& array) const
	{
		// 配列の初期化
		array.clear();

		// Transformは専用ポインタから即追加
		if constexpr (std::is_same_v<T, Transform>)
		{
			if (m_pTransform) array.push_back(m_pTransform.get());
		}
		// RectTransformも同様に
		else if constexpr (std::is_same_v<T, RectTransform>)
		{
			if (m_pRectTransform) array.push_back(m_pRectTransform.get());
		}

		else
		{
			// それ以外は ID 比較でループ
			for (auto& comp : m_pComponents)
			{
				// IDで比較する
				if (comp->GetID() == TypeIDGenerator::GetID<T>())
				{
					array.push_back(static_cast<T*>(comp.get()));
				}
			}
			// 予約リストをチェック
			for (auto& comp : m_pReservesAdd)
			{
				// IDで比較する
				if (comp->GetID() == TypeIDGenerator::GetID<T>())
				{
					array.push_back(static_cast<T*>(comp.get()));
				}
			}
		}

		return;
	}

	// ---------- GetWithCategory ---------- //
	
	ComponentBase* GetWithCategory(ComponentCategory category) const
	{
		// それ以外は ID 比較でループ
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetCategory() == category)
			{
				return comp.get();
			}
		}
		// 予約リストをチェック
		for (auto& comp : m_pReservesAdd)
		{
			// IDで比較する
			if (comp->GetCategory() == category)
			{
				return comp.get();
			}
		}

		return nullptr;
	}
	
	// ---------- GetsWithCategory ---------- //

	void GetsWithCategory(ComponentCategory category, std::vector<ComponentBase*>& array) const
	{
		// 配列の初期化
		array.clear();

		// ID 比較でループ
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetCategory() == category)
			{
				array.push_back(comp.get());
			}
		}
		// 予約リストをチェック
		for (auto& comp : m_pReservesAdd)
		{
			// IDで比較する
			if (comp->GetCategory() == category)
			{
				array.push_back(comp.get());
			}
		}

		return;
	}

	std::vector<ComponentBase*> GetsWithCategory(ComponentCategory category) const
	{
		std::vector<ComponentBase*> components;
		GetsWithCategory(category, components);
		return components;
	}

	// ---------- GetAll ---------- //
	  
	std::vector<ComponentBase*> GetAll() const
	{
		std::vector<ComponentBase*> all;

		if (m_pTransform) all.push_back(m_pTransform.get());
		if (m_pRectTransform) all.push_back(m_pRectTransform.get());
		for (auto& comp : m_pComponents) all.push_back(comp.get());
		for (auto& comp : m_pReservesAdd) all.push_back(comp.get());

		return all;
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
			if (IsCorrect(comp->GetSpace()))
			{
				// 配列に追加
				m_pReservesAdd.push_back(std::move(comp));

				add = ptr;
			}
		}

		// 追加したコンポーネントを返す
		return add;
	}

	// ----------- Has ----------- //

	bool Has(unsigned int id) const
	{
		return Get(id) != nullptr;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	bool Has() const
	{
		return Has(TypeIDGenerator::GetID<T>());
	}

	// ---------- Remove ---------- //

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void Remove(T* component)
	{
		m_pDestroyReserves.push_back(component);
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void Remove()
	{
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetID() == TypeIDGenerator::GetID<T>)
			{
				Remove(comp.get());

				break;
			}
		}
	}

	void RemoveWithCategory(ComponentCategory category)
	{
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetCategory() == category)
			{
				Remove(comp.get());

				break;
			}
		}
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
	void Removes()
	{
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetID() == TypeIDGenerator::GetID<T>())
			{
				Remove(comp.get());
			}
		}
	}

	void Remove(unsigned int id)
	{
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetID() == id)
			{
				Remove(comp.get());

				return;
			}
		}
	}

	void Removes(unsigned int id)
	{
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetID() == id)
			{
				Remove(comp.get());
			}
		}
	}

	void RemovesWithCategory(ComponentCategory category)
	{
		for (auto& comp : m_pComponents)
		{
			// IDで比較する
			if (comp->GetCategory() == category)
			{
				Remove(comp.get());
			}
		}
	}

	void AllRemove()
	{
		for (auto& comp : m_pComponents)
		{
			Remove(comp.get());
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
};

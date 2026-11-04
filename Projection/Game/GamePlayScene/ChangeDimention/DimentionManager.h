/**
 * @file    DimentionManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/06
 *
 * @brief   次元変更管理クラス
 */
#pragma once

#include "ChangeColliderComponent.h"
#include "../Camera/ProjectionSmoothCamera.h"

#include "Components/World/WorldComponentBase.h"
#include "Components/Interface/IComponentListener.h"

/**
 * @brief 次元変更を管理する
 */
class DimentionManager : public RHEngine::WorldComponentBase, public RHEngine::IComponentListener
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:
	/**
	 * @brief 状態を表す列挙型
	 */
	enum State
	{
		World2D,	///< 2D世界
		World3D,	///< 3D世界
		ChangeTo2D,	///< 3Dから2Dへ変化中
		ChangeTo3D,	///< 2Dから3Dへ変化中
	};

private:

	// カメラポインタ
	ProjectionSmoothCamera* m_pCamera;

	// 管理しているコンポーネント
	std::vector<ChangeColliderComponent*> m_components;

	// 追加予約済みのコンポーネント
	std::vector<ChangeColliderComponent*> m_addReserves;
	// 削除予約済みのコンポーネント
	std::vector<ChangeColliderComponent*> m_removeReserves;

	// 現在の状態
	State m_nowState;

public:

	DimentionManager(RHEngine::IComponentOwner* owner);
	~DimentionManager() = default;

	// 初期化関数
	void Awake() override;

	// 更新関数
	void Update(const RHEngine::GameTimer& timer) override;

	// 次元を切り替える関数
	void ChangeDimention(float changeTime = 1.0f);

	bool GetIs2D() const { return (m_nowState == State::World2D); }

	// コンポーネントが追加されたときに通知される関数
	void OnComponentAdded(ComponentBase* component) override
	{
		// ChangeColliderの場合
		if (component->IsTypeOf(ChangeColliderComponent::StaticTypeId()))
		{
			m_addReserves.push_back(static_cast<ChangeColliderComponent*>(component));
		}

		// カメラの場合
		if (component->IsTypeOf(ProjectionSmoothCamera::StaticTypeId()))
		{
			m_pCamera = static_cast<ProjectionSmoothCamera*>(component);
		}
	}

	// コンポーネントを削除されたときに通知される関数
	void OnComponentRemoved(ComponentBase* component) override
	{
		// ChangeColliderの場合
		if (component->IsTypeOf(ChangeColliderComponent::StaticTypeId())) 
		{
			m_removeReserves.push_back(static_cast<ChangeColliderComponent*>(component));
		}

		// カメラの場合
		if (component->IsTypeOf(ProjectionSmoothCamera::StaticTypeId()))
		{
			if (m_pCamera == static_cast<ProjectionSmoothCamera*>(component)) m_pCamera = nullptr;
		}
	}

	// カメラをセットする関数
	void SetCamera(ProjectionSmoothCamera* camera)
	{
		m_pCamera = camera;
	}

	// 切り替え中か調べる関数
	bool IsChanging() const
	{
		return m_nowState == State::ChangeTo2D || m_nowState == State::ChangeTo3D;
	}

private:

	// 追加予約済みコンポーネントを追加する
	void AddReserved()
	{
		for (auto& component : m_addReserves)
		{
			m_components.push_back(component);
		}

		// リセット
		m_addReserves.clear();
	}

	// 削除予約済みのコンポーネントを削除する
	void RemoveReserved()
	{
		for (auto& component : m_removeReserves)
		{
			// 配列にあれば削除する
			m_components.erase(
				std::remove(m_components.begin(), m_components.end(), component),
				m_components.end()
			);
		}

		// リセット
		m_removeReserves.clear();
	}

	// 世界を2次元へ変更する関数
	void WorldTo2D();

	// 世界を3次元へ変更する関数
	void WorldTo3D();
};

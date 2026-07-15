//====================================================//
// ファイル名   : DimentionManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/06
//
// 概要 : 次元変更管理クラス
//
// 更新履歴 :
// 2026/06/06 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(DimentionManager)

//====================================================//
// インクルードファイル
//====================================================//
#include "ChangeColliderComponent.h"
#include "../Camera/ProjectionSmoothCamera.h"

#include "Components/World/WorldComponentBase.h"
#include "Components/Interface/IComponentListener.h"

//====================================================//
// 前方宣言
//====================================================//

//====================================================//
// クラス宣言
//====================================================//
class DimentionManager : public REngine::WorldComponentBase, public REngine::IComponentListener
{
private:
	// 状態
	enum State
	{
		World2D,
		World3D,
		ChangeTo2D,
		ChangeTo3D,
	};

private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

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

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	DimentionManager(REngine::IComponentOwner* owner);
	~DimentionManager() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 初期化関数
	void Awake() override;

	// 更新関数
	void Update(const REngine::GameTimer& timer) override;

	// 次元を切り替える関数
	void ChangeDimention(float changeTime = 1.0f);

	bool GetIs2D() const { return (m_nowState == State::World2D); }

	// コンポーネントが追加されたときに通知される関数
	void OnComponentAdded(ComponentBase* component) override
	{
		// ChangeColliderの場合
		if (component->GetID() == REngine::TypeIDGenerator::GetID<ChangeColliderComponent>())
		{
			m_addReserves.push_back(static_cast<ChangeColliderComponent*>(component));
		}

		// カメラの場合
		if (component->GetID() == REngine::TypeIDGenerator::GetID<ProjectionSmoothCamera>())
		{
			m_pCamera = static_cast<ProjectionSmoothCamera*>(component);
		}
	}

	// コンポーネントを削除されたときに通知される関数
	void OnComponentRemoved(ComponentBase* component) override
	{
		// ChangeColliderの場合
		if (component->GetID() == REngine::TypeIDGenerator::GetID<ChangeColliderComponent>()) 
		{
			m_removeReserves.push_back(static_cast<ChangeColliderComponent*>(component));
		}

		// カメラの場合
		if (component->GetID() == REngine::TypeIDGenerator::GetID<ProjectionSmoothCamera>())
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

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return REngine::TypeIDGenerator::GetID<DimentionManager>();
	}

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

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

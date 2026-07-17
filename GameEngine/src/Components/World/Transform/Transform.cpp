//====================================================//
// ファイル名  : Transform.cpp.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/02
//
// 概要        :  トランスフォームコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Transform/Transform.h"

using namespace DirectX;

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	Transform::Transform(IComponentOwner* own)
		: WorldComponentBase(own)
		, m_localPosition{ 0, 0, 0 }
		, m_localRotation{ DirectX::SimpleMath::Quaternion::Identity }
		, m_localScale{ 1, 1, 1 }
		, m_worldMatrix{}
		, m_isDirty{ true }
		, m_nowVersion{ 0 }
		, m_pParent{ nullptr }
		, m_children{}
		, m_addCache{ 0, 0, 0 }
		, m_changeCache{ false }
	{
		ADD_PROPERTY(m_localPosition);
		ADD_PROPERTY(m_localRotation);
		ADD_PROPERTY(m_localScale);
	}

	Transform::~Transform()
	{
		// 親がいれば親から自分を削除
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		// 子供を解放
		RemoveChildren();
	}

	/// <summary>
	/// 親を設定する関数
	/// </summary>
	/// <param name="parent"></param>
	void Transform::SetParent(Transform* parent)
	{
		// 自分を親にしようとしていたら
		if (parent == this) return;

		// 子孫を親にしようとしていたら
		if (IsDescendantOf(parent)) return;

		// ワールド行列を取得
		auto& world = GetWorldMatrix();

		// 別の親に所属していたら
		if (m_pParent) m_pParent->RemoveChild(this);    // 親から自分を削除

		// 親のワールド行列を取得
		SimpleMath::Matrix parentWorld = (parent ? parent->GetWorldMatrix() : SimpleMath::Matrix::Identity);

		// 新しいローカル行列を算出
		SimpleMath::Matrix newLocal = world * parentWorld.Invert();

		// ローカル情報にセット
		SimpleMath::Vector3 s; SimpleMath::Quaternion r; SimpleMath::Vector3 t;
		newLocal.Decompose(s, r, t);

		m_localPosition = t;
		m_localRotation = r;
		m_localScale = s;

		m_pParent = parent;

		// 新しい親に自分を追加
		if (m_pParent) m_pParent->AddChild(this);

		// 変化フラグを立てる
		SetDirty();
	}

	/// <summary>
	/// 全ての子供を解放する関数
	/// </summary>
	void Transform::RemoveChildren()
	{
		// 現在の子リストをコピーする
		auto tempChildren = m_children;

		// コピーしたリストを使って処理
		for (auto* child : tempChildren)
		{
			child->SetParent(nullptr);
		}
		m_children.clear();
	}

	void Transform::UpdateCache() const
	{
		using namespace DirectX::SimpleMath;

		// 各ワールド成分を親から継承
		if (m_pParent) {
			m_worldScaleMatrix = Matrix::CreateScale(m_localScale) * m_pParent->GetWorldScaleMatrix();
			m_worldRotationMatrix = Matrix::CreateFromQuaternion(m_localRotation) * m_pParent->GetWorldRotationMatrix();

			// 親のワールド行列を使用して座標変換
			Vector3 worldPos = Vector3::Transform(m_localPosition, m_pParent->GetWorldMatrix());
			m_worldPositionMatrix = Matrix::CreateTranslation(worldPos);
		}
		else {
			m_worldScaleMatrix = Matrix::CreateScale(m_localScale);
			m_worldRotationMatrix = Matrix::CreateFromQuaternion(m_localRotation);
			m_worldPositionMatrix = Matrix::CreateTranslation(m_localPosition);
		}

		// --- ワールド行列の更新 --- //
		m_worldMatrix = m_worldScaleMatrix * m_worldRotationMatrix * m_worldPositionMatrix;

		// フラグのリセット
		m_isDirty = false;
	}

	void Transform::SetWorldPosition(const DirectX::SimpleMath::Vector3& pos)
	{
		if (m_pParent == nullptr) {
			// 親がいなければワールド＝ローカル
			m_localPosition = pos;
		}
		else {
			// 親がいる場合、親の逆行列をかけて「ローカル座標」に変換して保存
			SimpleMath::Matrix invParentWorld = m_pParent->GetWorldMatrix().Invert();
			m_localPosition = SimpleMath::Vector3::Transform(pos, invParentWorld);
		}
		// 行列の再計算フラグを立てる
		SetDirty();
	}

	void Transform::SetWorldMatrix(const DirectX::SimpleMath::Matrix& mat)
	{
		// 親がいる場合
		if (m_pParent)
		{
			// 親の逆行列を取得
			auto invParent = m_pParent->GetWorldMatrix().Invert();

			// 自身のローカルに変換
			auto local = mat * invParent;

			// 分解し変数に代入
			local.Decompose(
				m_localScale,
				m_localRotation,
				m_localPosition
			);
		}
		// 親がいない場合
		else
		{
			auto world = mat;

			// 分解し変数に代入
			world.Decompose(
				m_localScale,
				m_localRotation,
				m_localPosition
			);
		}

		SetDirty();
	}

	/// <summary>
	/// Dirtyフラグをセットする関数
	/// </summary>
	void Transform::SetDirty()
	{
		if (m_isDirty) return;
		m_isDirty = true;
		m_nowVersion++;

		// 子オブジェクトのフラグもセット
		for (auto* child : m_children) child->SetDirty();
	}
}	// namespace REngine

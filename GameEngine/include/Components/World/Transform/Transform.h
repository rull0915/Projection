//====================================================//
// ファイル名   : Transform.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/02
//
// 概要 : トランスフォームコンポーネント
//
// 更新履歴 :
// 2026/04/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include <unordered_set>

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class Transform : public WorldComponentBase
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 情報
		DirectX::SimpleMath::Vector3 m_localPosition;
		DirectX::SimpleMath::Quaternion m_localRotation;
		DirectX::SimpleMath::Vector3 m_localScale;

		// 行列のキャッシュ
		mutable DirectX::SimpleMath::Matrix m_worldMatrix;

		mutable DirectX::SimpleMath::Matrix m_worldPositionMatrix;    // World座標系の移動行列
		mutable DirectX::SimpleMath::Matrix m_worldRotationMatrix;    // World座標系の回転行列
		mutable DirectX::SimpleMath::Matrix m_worldScaleMatrix;       // World座標系の拡縮行列

		mutable bool m_isDirty; // 値が変わったらtrueにするフラグ

		mutable uint32_t m_nowVersion;  // 現在のバージョン

		// --- 親子関係 --- //

		Transform* m_pParent;   // 親

		std::unordered_set<Transform*> m_children; // 子

		// 位置の変更をためておく変数
		DirectX::SimpleMath::Vector3 m_addCache;
		bool m_changeCache;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Transform(IComponentOwner* own);
		~Transform();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 親を設定する関数
		void SetParent(Transform* parent);

		// キャッシュを更新する関数
		void UpdateCache() const;

		void CheckAndUpdateCache() const
		{
			if (m_isDirty)
			{
				UpdateCache();
			}
		}

		// GUI変更時
		void OnValidate() override
		{
			// Dirtyに
			SetDirty();
		}

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<Transform>();
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------
		const DirectX::SimpleMath::Vector3& GetLocalPosition() const { return m_localPosition; }
		const DirectX::SimpleMath::Quaternion& GetLocalRotation() const { return m_localRotation; }
		const DirectX::SimpleMath::Vector3 GetLocalEulerAngle() const { return m_localRotation.ToEuler(); }
		const DirectX::SimpleMath::Vector3& GetLocalScale() const { return m_localScale; }

		/// <summary>
		/// ワールド座標系での座標を返す関数
		/// </summary>
		const DirectX::SimpleMath::Vector3 GetWorldPosition() const
		{
			if (m_pParent)
			{
				// 最新のワールド行列を取得
				auto& world = GetWorldPositionMatrix();

				// 位置成分を返す
				return world.Translation();
			}

			return m_localPosition;
		}
		/// <summary>
		/// ワールド座標系での回転を返す関数
		/// </summary>
		const DirectX::SimpleMath::Quaternion GetWorldRotation() const
		{
			if (m_pParent)
			{
				// キャッシュ更新
				if (m_isDirty) UpdateCache();

				// 回転行列からクォータニオンを作る
				return DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(m_worldRotationMatrix);
			}
			return m_localRotation;
		}
		/// <summary>
		/// ワールド座標系でのオイラー角を返す関数
		/// </summary>
		const DirectX::SimpleMath::Vector3 GetWorldEulerAngle() const
		{
			if (m_pParent)
			{
				return GetWorldRotation().ToEuler();
			}
			return GetLocalEulerAngle();
		}
		/// <summary>
		/// ワールド座標系でのスケールを返す関数
		/// </summary>
		const DirectX::SimpleMath::Vector3 GetWorldScale() const
		{
			if (m_pParent)
			{
				// キャッシュ更新
				if (m_isDirty) UpdateCache();

				return { m_worldScaleMatrix._11, m_worldScaleMatrix._22, m_worldScaleMatrix._33 };
			}
			return m_localScale;
		}
		// ワールド行列を取得する関数
		const DirectX::SimpleMath::Matrix& GetWorldMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldMatrix;
		}
		// ワールドの移動行列
		const DirectX::SimpleMath::Matrix& GetWorldPositionMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldPositionMatrix;
		}
		// ワールドの回転行列
		const DirectX::SimpleMath::Matrix& GetWorldRotationMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldRotationMatrix;
		}
		// ワールドの拡縮行列
		const DirectX::SimpleMath::Matrix& GetWorldScaleMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldScaleMatrix;
		}

		// 前方向 (Z軸)
		DirectX::SimpleMath::Vector3 GetForward() const {
			auto f = GetWorldMatrix().Forward();
			f.Normalize();
			return f;
		}

		// 上方向 (Y軸)
		DirectX::SimpleMath::Vector3 GetUp() const {
			auto u = GetWorldMatrix().Up();
			u.Normalize();
			return u;
		}

		// 右方向 (X軸)
		DirectX::SimpleMath::Vector3 GetRight() const {
			auto r = GetWorldMatrix().Right();
			r.Normalize();
			return r;
		}

		inline uint32_t GetVersion() const { return m_nowVersion; }

		// 親
		const Transform* GetParent() const { return m_pParent; }

		// 子供
		const std::unordered_set<Transform*>& GetChildren() const { return m_children; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------
		void SetLocalPosition(const DirectX::SimpleMath::Vector3& pos)
		{
			m_localPosition = pos;
			SetDirty();
		}
		void SetLocalRotation(const DirectX::SimpleMath::Quaternion& rot)
		{
			m_localRotation = rot;
			SetDirty();
		}
		void SetLocalEulerAngle(const DirectX::SimpleMath::Vector3& rot)
		{
			m_localRotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot);
			SetDirty();
		}
		void SetLocalScale(const DirectX::SimpleMath::Vector3& scale)
		{
			m_localScale = scale;
			SetDirty();
		}

		void SetWorldPosition(const DirectX::SimpleMath::Vector3 pos);

		//-----------------------------------------------------
		// 補助関数
		//-----------------------------------------------------
		void AddLocalPosition(const DirectX::SimpleMath::Vector3& value)
		{
			if (value.LengthSquared() < FLT_EPSILON) return;
			m_localPosition += value;
			SetDirty();
		}
		void AddLocalEulerAngle(const DirectX::SimpleMath::Vector3& rot)
		{
			if (rot.LengthSquared() < FLT_EPSILON) return;
			DirectX::SimpleMath::Vector3 euler = GetLocalEulerAngle();
			m_localRotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler + rot);
			SetDirty();
		}
		void AddWorldPosition(const DirectX::SimpleMath::Vector3 value)
		{
			if (value.LengthSquared() < FLT_EPSILON) return;
			SetWorldPosition(GetWorldPosition() + value);
			SetDirty();
		}
		// キャッシュの値を増やす関数
		inline void AddCache(const DirectX::SimpleMath::Vector3& value)
		{
			if (value.LengthSquared() < FLT_EPSILON) return;
			m_addCache += value;
			m_changeCache = true;
		};

		// キャッシュの値を反映させる関数
		void ReflectCache()
		{
			if (!m_changeCache) return;

			AddWorldPosition(m_addCache);
			m_addCache = { 0, 0, 0 };
			m_changeCache = false;
		}

		// Dirtyフラグをセットする関数
		void SetDirty();

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// 子供を追加する関数
		void AddChild(Transform* child)
		{
			// nullptrでなければ追加
			if (child) m_children.insert(child);
		}

		// 子供を削除する関数
		void RemoveChild(Transform* child)
		{
			if (!child) return;

			m_children.erase(child);
		}

		// 子供を全て解放する関数
		void RemoveChildren();

		// 指定したトランスフォームが自分のツリーに含まれるか調べる関数
		bool IsDescendantOf(Transform* target)
		{
			Transform* current = target;

			// currentがnullになるまでループ
			while (current != nullptr)
			{
				// 自分と同じかチェック
				if (current == this)
				{
					return true;
				}

				// 親を辿る
				current = current->m_pParent;
			}

			// 最後まで到達した時
			return false;
		}
	};
} // namespace REngine

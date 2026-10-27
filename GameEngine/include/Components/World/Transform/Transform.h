/**
 * @file    Transform.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/02
 *
 * @brief   トランスフォームコンポーネント
 */

#pragma once

#include <vector>
#include <cstdint>
#include <SimpleMath.h>

#include "Components/World/WorldComponentBase.h"

namespace RHEngine
{
	class IComponentOwner;

	class Transform : public WorldComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

	private:

		/** @brief ローカル座標系での位置 */
		DirectX::SimpleMath::Vector3 m_localPosition;

		/** @brief ローカル座標系での回転 */
		DirectX::SimpleMath::Quaternion m_localRotation;

		/** @brief ローカル座標系でのスケール */
		DirectX::SimpleMath::Vector3 m_localScale;

		/// @name 行列のキャッシュ
		/// @{

		/** @brief ワールド行列のキャッシュ */
		mutable DirectX::SimpleMath::Matrix m_worldMatrix;

		/** @brief ワールド座標系での移動行列 */
		mutable DirectX::SimpleMath::Matrix m_worldPositionMatrix;

		/** @brief ワールド座標系での回転行列 */
		mutable DirectX::SimpleMath::Matrix m_worldRotationMatrix;

		/** @brief ワールド座標系での拡縮行列 */
		mutable DirectX::SimpleMath::Matrix m_worldScaleMatrix;

		/// @}

		/** @brief 変更フラグ */
		mutable bool m_isDirty;
		
		/** @brief 現在のバージョン番号 変更されるたびにインクリメントする */
		mutable uint32_t m_nowVersion;

		/// @name 親子関係
		/// @{

		/** @brief 親のTransform */
		Transform* m_pParent;

		/** @brief このリスト */
		std::vector<Transform*> m_children;

		/// @}

		/** @brief 1fの位置変更を貯めておく変数 */
		DirectX::SimpleMath::Vector3 m_addCache;

		/** @brief キャッシュが変更されたかどうか */
		bool m_changeCache;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 
		 */
		Transform(IComponentOwner* pOwner);

		~Transform();

		/// @}

		/**
		 * @brief 親を設定する
		 * @param parent 親のポインタ
		 */
		void SetParent(Transform* parent);

		/**
		 * @brief 各行列のキャッシュを更新する
		 */
		void UpdateCache() const;

		/**
		 * @brief キャッシュ更新が必要か調べ、必要ならば更新する
		 */
		void CheckAndUpdateCache() const
		{
			if (m_isDirty)
			{
				UpdateCache();
			}
		}

		/**
		 * @brief GUI上での値変更時にDirtyフラグを立てる
		 */
		void OnValidate() override
		{
			SetDirty();
		}

		/// @name ゲッター
		/// @{
		 	
		/** @brief バージョン番号 */
		inline uint32_t GetVersion() const { return m_nowVersion; }

		/** @brief 親のトランスフォーム */
		const Transform* GetParent() const { return m_pParent; }

		/** @brief 子供のリスト */
		const std::vector<Transform*>& GetChildren() const { return m_children; }

		/** @brief ローカル座標系での位置 */
		const DirectX::SimpleMath::Vector3& GetLocalPosition() const { return m_localPosition; }

		/** @brief ローカル座標系での回転 */
		const DirectX::SimpleMath::Quaternion& GetLocalRotation() const { return m_localRotation; }

		/** @brief ローカル座標系でのオイラー角 */
		const DirectX::SimpleMath::Vector3 GetLocalEulerAngle() const { return m_localRotation.ToEuler(); }

		/** @brief ローカル座標系でのスケール */
		const DirectX::SimpleMath::Vector3& GetLocalScale() const { return m_localScale; }

		/** @brief ワールド座標系での位置 */
		const DirectX::SimpleMath::Vector3 GetWorldPosition() const;

		/** @brief ワールド座標系での回転 */
		const DirectX::SimpleMath::Quaternion GetWorldRotation() const;

		/** @brief ワールド座標系でのオイラー角 */
		const DirectX::SimpleMath::Vector3 GetWorldEulerAngle() const;

		/** @brief ワールド座標系でのスケール */
		const DirectX::SimpleMath::Vector3 GetWorldScale() const;

		/** @brief ワールド行列 */
		const DirectX::SimpleMath::Matrix& GetWorldMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldMatrix;
		}

		/** @brief ワールド座標系での移動行列 */
		const DirectX::SimpleMath::Matrix& GetWorldPositionMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldPositionMatrix;
		}

		/** @brief ワールド座標系での回転行列 */
		const DirectX::SimpleMath::Matrix& GetWorldRotationMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldRotationMatrix;
		}

		/** @brief ワールド座標系での拡縮行列 */
		const DirectX::SimpleMath::Matrix& GetWorldScaleMatrix() const
		{
			// キャッシュ更新
			if (m_isDirty) UpdateCache();

			return m_worldScaleMatrix;
		}

		/** @brief 前方向 */
		DirectX::SimpleMath::Vector3 GetForward() const {
			auto f = GetWorldMatrix().Forward();
			f.Normalize();
			return f;
		}

		/** @brief 上方向 */
		DirectX::SimpleMath::Vector3 GetUp() const {
			auto u = GetWorldMatrix().Up();
			u.Normalize();
			return u;
		}

		/** @brief 右方向 */
		DirectX::SimpleMath::Vector3 GetRight() const {
			auto r = GetWorldMatrix().Right();
			r.Normalize();
			return r;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief ローカル座標系での位置 */
		void SetLocalPosition(const DirectX::SimpleMath::Vector3& pos)
		{
			m_localPosition = pos;
			SetDirty();
		}

		/** @brief ローカル座標系での回転 */
		void SetLocalRotation(const DirectX::SimpleMath::Quaternion& rot)
		{
			m_localRotation = rot;
			SetDirty();
		}

		/** @brief ローカル座標系でのオイラー角 */
		void SetLocalEulerAngle(const DirectX::SimpleMath::Vector3& rot)
		{
			m_localRotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot);
			SetDirty();
		}

		/** @brief ローカル座標系でのスケール */
		void SetLocalScale(const DirectX::SimpleMath::Vector3& scale)
		{
			m_localScale = scale;
			SetDirty();
		}

		/** @brief ワールド座標系での位置 */
		void SetWorldPosition(const DirectX::SimpleMath::Vector3& pos);

		/** @brief ワールド行列 */
		void SetWorldMatrix(const DirectX::SimpleMath::Matrix& mat);

		/// @}

		/**
		 * @brief ローカル座標系の位置を加算する
		 * @param value 加算するベクトル
		 */
		void AddLocalPosition(const DirectX::SimpleMath::Vector3& value)
		{
			if (value.LengthSquared() < FLT_EPSILON) return;
			m_localPosition += value;
			SetDirty();
		}

		/**
		 * @brief ワールド座標系での位置を加算する
		 * @param value 加算するベクトル
		 */
		void AddWorldPosition(const DirectX::SimpleMath::Vector3 value)
		{
			if (value.LengthSquared() < FLT_EPSILON) return;
			SetWorldPosition(GetWorldPosition() + value);
			SetDirty();
		}

		/**
		 * @brief 移動キャッシュを加算する
		 * @param value 加算するベクトル
		 */
		inline void AddCache(const DirectX::SimpleMath::Vector3& value)
		{
			if (value.LengthSquared() < FLT_EPSILON) return;
			m_addCache += value;
			m_changeCache = true;
		};

		/** @brief キャッシュの値を反映させる */
		void ReflectCache();

		/** @brief 再帰的にDirtyフラグを立てる */
		void SetDirty();

	private:

		/**
		 * @brief 子供を追加する
		 * @param child 追加対象のトランスフォームポインタ
		 */
		void AddChild(Transform* child)
		{
			// nullptrでなければ追加
			if (child) m_children.push_back(child);
		}

		/**
		 * @brief 子供を削除する
		 * @param child 削除対象のトランスフォームポインタ
		 */
		void RemoveChild(Transform* child)
		{
			if (!child) return;

			std::erase_if(m_children, [child](Transform* transform) { return transform == child; });
		}

		/**
		 * @brief 子供をすべて削除する
		 */
		void RemoveChildren();

		/**
		 * @brief 指定したTransformが自分の親子関係に含まれるか調べる
		 * @param target 調べる対象のトランスフォームポインタ
		 * @return 含まれているかどうか
		 */
		bool IsDescendantOf(Transform* target);
	};
} // namespace RHEngine

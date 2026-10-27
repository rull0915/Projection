/**
 * @file    RectTransform.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/11
 *
 * @brief   UIのトランスフォームコンポーネント
 */

#pragma once

#include <vector>
#include <SimpleMath.h>

#include "Components/UI/UIComponentBase.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief UIのRectTransform
	 */
	class RectTransform : public UIComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(UIComponentBase);

	private:

		/** @brief 親のRectTransform */
		RectTransform* m_pParent;

		/** @brief 子の一覧 */
		std::vector<RectTransform*> m_children;

		/** 
		 * @brief アンカーの位置 
		 * @details 親のRectのどこを基準とするか
		 *			0,0 の場合左上基準
		 *			0.5, 0.5 の場合中心基準
		 */
		DirectX::SimpleMath::Vector2 m_anchor;

		/**
		 * @brief アンカー基準の位置
		 * @details アンカーからの相対座標
		 */
		DirectX::SimpleMath::Vector2 m_anchoredPosition;

		/** @brief Rectのサイズ */
		DirectX::SimpleMath::Vector2 m_size;

		/**
		 * @brief ピボットの位置
		 * @details m_anchoredPositionに自身のどこを配置するか
		 *			0,0 の場合m_anchoredPositionに自分のRectの左上が配置される
		 */
		DirectX::SimpleMath::Vector2 m_pivot;

		/** @brief 拡大率 */
		DirectX::SimpleMath::Vector2 m_scale;

		/** @brief 回転角 */
		float m_rotation;

		/** @brief ピボットのピクセル座標のキャッシュ */
		mutable DirectX::SimpleMath::Vector2 m_pivotPixelPos;

		/** @brief アンカーのピクセル座標のキャッシュ */
		mutable DirectX::SimpleMath::Vector2 m_anchorPixelPos;

		/** @brief Rect左上のピクセル座標のキャッシュ */
		mutable DirectX::SimpleMath::Vector2 m_luPixelPos;

		/** @brief 変更済みフラグ */
		mutable bool m_isDirty;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		RectTransform(IComponentOwner* pOwner);

		~RectTransform();

		/// @}

		/**
		 * @brief キャッシュを更新する
		 */
		void UpdateCache() const;

		/**
		 * @brief 特定の点が自身に重なっているかどうか調べる
		 * @param target 調べるスクリーン座標
		 * @return 重なっているかどうか
		 */
		bool Contains(const DirectX::SimpleMath::Vector2& target);

		/** @brief GUI上での値変更時に変更フラグを立てる */
		void OnValidate() override
		{
			SetDirty();
		}
		
		/**
		 * @brief 子供を追加する
		 * @param child 追加する子供のRect
		 */
		void AddChild(RectTransform* child);

		/**
		 * @brief 子供を削除する
		 * @param child 削除対象のRect
		 */
		void RemoveChild(RectTransform* child);

		/**
		 * @brief 全ての子供を削除する
		 */
		void RemoveChildren();

		/**
		 * @brief 再帰的にDirtyフラグを立てる
		 */
		void SetDirty();

		/// @name ゲッター
		/// @{

		/** @brief 親のRectTransform */
		RectTransform* GetParent() { return m_pParent; }

		/** @brief ピボットの座標 */
		DirectX::SimpleMath::Vector2 GetPivot() const { return m_pivot; }

		/** @brief 子供のリスト */
		std::vector<RectTransform*>& GetChildren() { return m_children; }    // 子のRectTransformのリストを取得

		/** @brief Rectの右方向 */
		DirectX::SimpleMath::Vector2 GetRight() const { return { std::cosf(GetWorldRotation()), std::sinf(GetWorldRotation()) }; }

		/** @brief Rectの上方向 */
		DirectX::SimpleMath::Vector2 GetUp() const { return { -std::sinf(GetWorldRotation()), std::cosf(GetWorldRotation()) }; }

		/** @brief サイズ */
		DirectX::SimpleMath::Vector2 GetSize() const { return m_size; }

		/** @brief ワールド座標系でのスケール */
		DirectX::SimpleMath::Vector2 GetWorldScale() const
		{
			return m_scale * (m_pParent ? m_pParent->GetWorldScale() : DirectX::SimpleMath::Vector2(1.0f, 1.0f));
		}

		/** @brief ローカル座標系でのスケール */
		DirectX::SimpleMath::Vector2 GetLocalScale() const 
		{
			return m_scale;
		}

		/** @brief ワールド座標系での回転角 (ラジアン) */
		float GetWorldRotation() const
		{
			return m_rotation + (m_pParent ? m_pParent->GetWorldRotation() : 0.0f);
		}

		/** @brief ピボットのピクセル座標 */
		DirectX::SimpleMath::Vector2 GetPivotPixelPos() const
		{
			if (m_isDirty) UpdateCache();
			return m_pivotPixelPos;
		}

		/** @brief アンカーのピクセル座標 */
		DirectX::SimpleMath::Vector2 GetAnchorPixelPos() const
		{
			if (m_isDirty) UpdateCache();
			return m_pivotPixelPos;
		}

		/** @brief Rect左上のピクセル座標 */
		DirectX::SimpleMath::Vector2 GetLUPixelPos() const
		{
			if (m_isDirty) UpdateCache();
			return m_luPixelPos;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief 親のRect */
		void SetParent(RectTransform* parent);

		/** @brief アンカー座標 */
		void SetAnchor(DirectX::SimpleMath::Vector2 anchor) {
			m_anchor = anchor;
			SetDirty();
		}

		/** @brief アンカー基準の相対座標 */
		void SetAnchoredPosition(DirectX::SimpleMath::Vector2 anchoredPosition) {
			m_anchoredPosition = anchoredPosition;
			SetDirty();
		}

		/** @brief Rectサイズ */
		void SetSize(DirectX::SimpleMath::Vector2 size) {
			m_size = size;
			SetDirty();
		}

		/** @brief ピボット座標 */
		void SetPivot(DirectX::SimpleMath::Vector2 pivot) { // (0,0)なら左下、(1,1)なら右上
			m_pivot = pivot;
			SetDirty();
		}

		/** @brief スケール */
		void SetScale(DirectX::SimpleMath::Vector2 scale) {
			m_scale = scale;
			SetDirty();
		}

		/** @brief 回転角度 (ラジアン) */
		void SetRotation(float rotation) {
			m_rotation = rotation;
			SetDirty();
		}

	private:

		// 指定したRectTransformが自分の親子関係に含まれるか調べる
		bool IsDescendantOf(RectTransform* pTarget);
	};
} // namespace RHEngine

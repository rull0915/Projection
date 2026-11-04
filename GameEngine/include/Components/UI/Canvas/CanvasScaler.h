/**
 * @file    CanvasScaler.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/22
 *
 * @brief   キャンバスのスケールを管理するコンポーネント
 */

#pragma once

#include <SimpleMath.h>
#include <vector>

#include "Components/UI/UIComponentBase.h"
#include "Components/Interface/IWindowSizeDependent.h"

namespace RHEngine
{
	class IComponentOwner;
	class RectTransform;
	struct Property;

	/**
	 * @brief キャンバスのスケールを管理するコンポーネント
	 */
	class CanvasScaler : public UIComponentBase, public IWindowSizeDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(UIComponentBase);

		/**
		 * @brief キャンバスのスケールのモード
		 */
		enum class CanvasMode
		{
			None,				///< エラー値
			ConstantPixelSize,	///< 固定のピクセル
			ScaleWithScreenSize	///< 基準解像度を使用する
		};

		/**
		 * @brief ScaleWithScreenSize時のスケーリングモード
		 */
		enum class ScreenMatchMode
		{
			MatchWidthOrHeight,	///< 割合を使用する
			Expand,				///< 小さい方に合わせる
			Shrink				///< 大きい方に合わせる
		};

	private:

		/** @brief 自身のRectTransformポインタ */
		RectTransform* m_pRectTransform;

		/** @brief スケールのモード */
		CanvasMode m_mode;

		/// @name ConstactPixelSizeの設定
		/// @{

		/** @brief 表示倍率 */
		float m_scaleFactor;

		/// @}

		/// @name ScaleWithScreenSizeの設定
		/// @{

		/** @brief 基準解像度 */
		DirectX::SimpleMath::Vector2 m_referenceResolution;

		/** @brief スケーリングモード */
		ScreenMatchMode m_screenMatchMode;

		/**
		 * @brief 画面の縦横どちらに合わせてUIを拡縮するか
		 * @details 0.0 : 幅基準
		 *			1.0 : 高さ基準
		 */
		float m_matchWidthOrHeight;

		/// @}

	public:

		/// @name ライフサイクル
		/// @{

		CanvasScaler(IComponentOwner* pOwner);

		~CanvasScaler() = default;

		/// @}

		/**
		 * @brief GUI上で値が変更された際の処理
		 */
		void OnValidate() override;
		
		/**
		 * @brief 削除時の処理
		 */
		void OnDestroy() override
		{
			ResetRectTransform();
		}

		/**
		 * @brief ウィンドウサイズ変更時の処理
		 * @param width 変更後のウィンドウ幅
		 * @param height 変更後のウィンドウ高さ
		 */
		void OnWindowSizeChanged(int width, int height) override
		{
			CorrectRectTransform();
		}

		/**
		 * @brief プロパティリストを取得する
		 * @details モードに応じてシリアライズするパラメータを変更する
		 * @return プロパティリスト
		 */
		std::vector<Property> GetProperties() override;

	private:

		/** @brief RectTransformの値をスケーリングモードに応じて補正する */
		void CorrectRectTransform();

		/** @brief RectTransformの値を初期状態にリセットする */
		void ResetRectTransform();

		/** @brief スケールを計算する */
		float CalcScale();
	};
}	// namespace RHEngine

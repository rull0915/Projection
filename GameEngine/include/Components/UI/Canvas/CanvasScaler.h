//====================================================//
// ファイル名   : CanvasScaler.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/22
//
// 概要 : キャンバスのスケールを管理するコンポーネント
//
// 更新履歴 :
// 2026/10/22 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(CanvasScaler)

//====================================================//
// インクルードファイル
//====================================================//

#include "Components/UI/UIComponentBase.h"
#include "Components/UI/RectTransform/RectTransform.h"
#include "Components/Interface/IWindowSizeDependent.h"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class CanvasScaler : public UIComponentBase, public IWindowSizeDependent
	{
		// タイプ
	public:

		enum class CanvasMode
		{
			None,
			ConstantPixelSize,
			ScaleWithScreenSize
		};

		enum class ScreenMatchMode
		{
			MatchWidthOrHeight,
			Expand,
			Shrink
		};

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// RectTransform
		RectTransform* m_pRect;

		// モード
		CanvasMode m_mode;

		//----- ConstactPixelSize ------//

		// 表示倍率
		float m_scaleFactor;

		//----- ScaleWithScreenSize ------//

		// 基準解像度
		DirectX::SimpleMath::Vector2 m_referenceResolution;

		// 拡縮モード
		ScreenMatchMode m_screenMatchMode;

		// 画面の縦横どちらに合わせてUIを拡縮するか
		// 0.0:横幅基準 1.0f 縦幅基準
		float m_matchWidthOrHeight;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CanvasScaler(IComponentOwner* own);
		~CanvasScaler() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(CanvasScaler, UIComponentBase);

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Start() override;

		void Update(const GameTimer& gameTimer) override;

		// 値変更時処理
		void OnValidate() override
		{
			// アクティブなら
			if (IsActive())
			{
				CorrectRectTransform();
			}
			// 非アクティブなら
			else
			{
				ResetRectTransform();
			}
		}

		// 削除時処理
		void OnDestroy() override
		{
			// 非アクティブなら
			ResetRectTransform();
		}

		// ウィンドウサイズ変更時処理
		void OnWindowSizeChanged(int width, int height) override
		{
			CorrectRectTransform();
		}

		// プロパティ取得関数
		std::vector<Property> GetProperties() override;

	private:

		// RectTransformを補正する関数
		void CorrectRectTransform();

		// RectTransformをリセットする関数
		void ResetRectTransform();

		// スケール計算関数
		float CalcScale();
	};
}

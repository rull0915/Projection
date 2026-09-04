//====================================================//
// ファイル名   : TextUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/26
//
// 概要 : 文字列UIコンポーネント
//
// 更新履歴 :
// 2026/05/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "UIGraphicBase.h"
#include "Components/Interface/IAssetDependent.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/Font.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class TextUI : public UIGraphicBase, public IAssetDependent
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		 // 表示文字列
		std::string m_text;

		// フォントサイズ
		float m_fontSize;

		// 描画設定
		DirectX::SimpleMath::Vector2 m_origin;

		// フォントハンドル
		Handle<Font> m_fontHandle;

		// AssetManager
		AssetManager* m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		TextUI(IComponentOwner* owner)
			: UIGraphicBase(owner)
			, m_text{}
			, m_fontHandle{}
			, m_fontSize{ 64 }
			, m_origin{ 0, 0 }
		{
			ADD_PROPERTY(TextUI, m_text);
			ADD_PROPERTY(TextUI, m_fontSize);
			ADD_PROPERTY(TextUI, m_origin);
			ADD_PROPERTY(TextUI, m_fontHandle);
		}

		~TextUI() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(TextUI, UIGraphicBase)

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Draw(Renderer& renderer) override;

		void ReceiveAssetManager(AssetManager& manager) override
		{
			m_assetManager = &manager;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetFont(Handle<Font> font) { m_fontHandle = font; }
		void SetText(const std::string& text) { m_text = text; }
		void SetOrigin(DirectX::SimpleMath::Vector2 origin) { m_origin = origin; }
		void SetFontSize(float size) { m_fontSize = size; }
	};
} // namespace REngine

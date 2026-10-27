/**
 * @file    TextUI.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/26
 *
 * @brief   テキスト表示UIコンポーネント
 */

#pragma once

#include <string>
#include <SimpleMath.h>

#include "UIGraphicBase.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Font.h"

namespace RHEngine
{
	class IComponentOwner;
	class AssetManager;
	class Renderer;

	/**
	 * @brief テキストを描画するコンポーネント
	 */
	class TextUI : public UIGraphicBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(UIGraphicBase);

	private:

		/** @brief 表示する文字列 */
		std::string m_text;

		/** @brief フォントサイズ */
		float m_fontSize;

		/** @brief 基準点にテキストのどこを配置するか */
		DirectX::SimpleMath::Vector2 m_origin;

		/** @brief フォントハンドル */
		Handle<Font> m_fontHandle;

		/** @brief AssetManagerポインタ */
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
			, m_assetManager{}
		{
			ADD_PROPERTY(TextUI, m_text);
			ADD_PROPERTY(TextUI, m_fontSize);
			ADD_PROPERTY(TextUI, m_origin);
			ADD_PROPERTY(TextUI, m_fontHandle);
		}

		~TextUI() = default;

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
} // namespace RHEngine

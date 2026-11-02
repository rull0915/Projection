/**
 * @file    Font.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/27
 *
 * @brief   フォントアセット
 */

#pragma once

#include <SpriteFont.h>
#include <memory>
#include <utility>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	/**
	 * @brief フォントアセット
	 */
	class Font : public AssetBase
	{
	private:

		/** @brief フォント本体 */
		std::unique_ptr<DirectX::SpriteFont> m_font;

		/** 
		 * @brief フォントのサイズ 
		 * @details スケール1の状態で"A"を表示した時のピクセル数をフォントのデフォルトサイズとして扱う
		 */
		float m_size;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param font フォント本体
		 * @param height フォントのデフォルトサイズ
		 */
		Font(std::unique_ptr<DirectX::SpriteFont> font, float height)
			: m_font{ std::move(font) }
			, m_size{ height }
		{}

		~Font() = default;

		/// @}

		/// @name ゲッター
		/// @{
		
		/** @brief フォント本体 */
		DirectX::SpriteFont* Get() const { return m_font.get(); }

		/** @brief フォントサイズ */
		float GetFontSize() const { return m_size; }

		/// @}
	};
}	// namespace RHEngine

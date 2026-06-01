//====================================================//
// ファイル名   : TextProxy.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/26
//
// 概要 : 文字列描画仲介クラス
//
// 更新履歴 :
// 2026/05/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../EachRenderer/Text/TextRenderer.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//

// 各パラメータが既に設定されたかどうかをテンプレート引数として持ちます
template <bool HasRect = false, bool HasExtend = false, bool HasRotate = false, bool HasOrigin = false>
class TextProxy
{
    // 全てのタイプをfriendに指定
    template <bool R, bool E, bool Ro, bool O>
    friend class TextProxy;

private:
    TextRenderer* m_text = nullptr;

    // 描画のパラメータ
	DirectX::SimpleMath::Vector2 m_min = { 0, 0 };          // 描画先の矩形の左上座標
	DirectX::SimpleMath::Vector2 m_max = { 0, 0 };          // 描画先の矩形の右下座標
	DirectX::SimpleMath::Vector2 m_scale = { 1.0f, 1.0f };  // 拡大率
	float m_angle = 0.0f;                                   // 回転角
	DirectX::SimpleMath::Vector2 m_origin = { 0, 0 };       // 回転と拡大の中心座標（0～1の範囲で指定）

public:

	// デフォルトコンストラクタ 
    TextProxy()
        : m_text(nullptr)
        , m_min{ 0, 0 }
        , m_max{ 0, 0 }
        , m_scale{ 1.0f, 1.0f }
        , m_angle(0.0f)
        , m_origin{ 0, 0 }
    {}

    // コピーコンストラクタ（状態が変わる時に設定を引き継ぐため）
    template <bool R, bool E, bool Ro, bool O>
    TextProxy(const TextProxy<R, E, Ro, O>& other)
        : m_text(other.m_text)
        , m_min(other.m_min)
        , m_max(other.m_max)
        , m_scale(other.m_scale)
        , m_angle(other.m_angle)
        , m_origin(other.m_origin)
    {}

	// 描画クラスをセットする関数
    void SetText(TextRenderer* text) { m_text = text; }

    // 各パラメータを追加する関数

	// 描画先の矩形
    template <typename = std::enable_if_t<!HasRect>>
    TextProxy<true, HasExtend, HasRotate, HasOrigin> Rect(DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max) {
        m_min = min;
        m_max = max;
        return TextProxy<true, HasExtend, HasRotate, HasOrigin>(*this);
    }

	// 拡大率
    template <typename = std::enable_if_t<!HasExtend>>
	TextProxy<HasRect, true, HasRotate, HasOrigin> Extend(DirectX::SimpleMath::Vector2 scale) {
		m_scale = scale;
		return TextProxy<HasRect, true, HasRotate, HasOrigin>(*this);
	}
	template <typename = std::enable_if_t<!HasExtend>>
    TextProxy<HasRect, true, HasRotate, HasOrigin> Extend(float scale){
		m_scale = DirectX::SimpleMath::Vector2(scale, scale);
		return TextProxy<HasRect, true, HasRotate, HasOrigin>(*this);
    }

	// 回転角
	template <typename = std::enable_if_t<!HasRotate>>
	TextProxy<HasRect, HasExtend, true, HasOrigin> Rotate(float angle) {
		m_angle = angle;
		return TextProxy<HasRect, HasExtend, true, HasOrigin>(*this);
	}

	// 回転と拡大の中心座標
	template <typename = std::enable_if_t<!HasOrigin>>
	TextProxy<HasRect, HasExtend, HasRotate, true> Origin(DirectX::SimpleMath::Vector2 origin) {
		m_origin = origin;
		return TextProxy<HasRect, HasExtend, HasRotate, true>(*this);
	}
	template <typename = std::enable_if_t<!HasOrigin>>
    TextProxy<HasRect, HasExtend, HasRotate, true> Origin(Origin::Type type) {
		m_origin = Origin::GetPosition(type);
		return TextProxy<HasRect, HasExtend, HasRotate, true>(*this);
    }

	// 状態のリセット
	void Reset() 
    {
		m_min = { 0, 0 };
		m_max = { 0, 0 };
		m_scale = { 1.0f, 1.0f };
		m_angle = 0.0f;
		m_origin = { 0, 0 };
    }

    // 描画を確定する関数
    template <typename = std::enable_if_t<HasRect>> // Rectを指定している場合
    void Execute(DirectX::SpriteFont* spriteFont, const wchar_t* text, int color = 0xFFFFFF)
    {
        // Originを考慮して中心を計算
        DirectX::SimpleMath::Vector2 center = m_min + (m_max - m_min) * m_origin;

	    DirectX::SimpleMath::Vector2 textureSize = TextRenderer::GetTextureSize(spriteFont, text);

		// スケールを計算（矩形のサイズをテクスチャのサイズで割る）
        DirectX::SimpleMath::Vector2 finalScale = (m_max - m_min) / textureSize;
        if constexpr (HasExtend) {
            finalScale *= m_scale; // Extendが呼ばれている時だけ掛け算する
        }

	    // 描画

        // TextRendererの描画関数を呼び出す
        m_text->Draw(
            spriteFont,
            text,
            center,
            finalScale,
            m_angle,
            textureSize * m_origin,
            color
        );
    }
    template <typename = std::enable_if_t<!HasRect>> // Rectを指定していない場合
    void Execute(DirectX::SpriteFont* spriteFont, const wchar_t* text, DirectX::SimpleMath::Vector2 position, int color = 0xFFFFFF)
    {
        if constexpr (HasOrigin)
        {
            DirectX::SimpleMath::Vector2 textureSize = TextRenderer::GetTextureSize(spriteFont, text);

            // TextRendererの描画関数を呼び出す
            m_text->Draw(
                spriteFont,
                text,
                position,
                m_scale,
                m_angle,
                textureSize * m_origin,
                color
            );
        }
        else
        {
            // TextRendererの描画関数を呼び出す
            m_text->Draw(
                spriteFont,
                text,
                position,
                m_scale,
                m_angle,
                { 0, 0 },
                color
            );
        }
    }
};

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
#include <SpriteFont.h>
#include "UIGraphicBase.h"
#include "Components/Interface/IResourceReader.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class TextUI : public UIGraphicBase, public IResourceReader
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	 // 表示文字列
	std::string m_text;

	// フォント
	DirectX::SpriteFont* m_pFont;

	// フォント名
	std::string m_fontName;

	// フォントサイズ
	float m_fontSize;

	// 描画設定
	DirectX::SimpleMath::Vector2 m_origin;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	TextUI(IComponentOwner* owner)
		: UIGraphicBase(owner)
		, m_text{}
		, m_pFont{ nullptr }
		, m_fontName{}
		, m_fontSize{ 64 }
		, m_origin{ 0, 0 }
	{
		ADD_PROPERTY(m_text);
		ADD_PROPERTY(m_fontSize);
		ADD_PROPERTY(m_origin);
		ADD_PROPERTY(m_fontName);
	}
	
	~TextUI() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override
	{
		LoadResource();
		ReflectLoading();
	}

	void Draw(Renderer& renderer) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<TextUI>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetFont(const std::string& key) 
	{
		m_fontName = key;

		LoadResource();
		ReflectLoading();
	}
	void SetText(const std::string& text) { m_text = text; }
	void SetOrigin(DirectX::SimpleMath::Vector2 origin) { m_origin = origin; }
	void SetFontSize(float size) { m_fontSize = size; }

	// ---------- リソース関連 ---------- //

	// 読み込みを反映する
	void ReflectLoading() override {};
	
private:
	// リソースタイプ
	Type GetType() const override { return Type::Font; }

	// キー名
	const std::string& GetKeyName() const override { return m_fontName; }

	// リソースポインタポインタ
	void** GetMyResource() const override { return (void**) &m_pFont; }
};

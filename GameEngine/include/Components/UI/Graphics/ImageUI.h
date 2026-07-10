//====================================================//
// ファイル名   : ImageUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要 : 画像描画コンポーネント
//
// 更新履歴 :
// 2026/05/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/Interface/IResourceReader.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ImageUI : public UIGraphicBase, public IResourceReader
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// テクスチャ
	ID3D11ShaderResourceView* m_pTexture;

	// テクスチャ名
	std::string m_textureName;

	// Rayと衝突するかどうか
	bool m_raycastTarget;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ImageUI(IComponentOwner* owner)
		: UIGraphicBase(owner)
		, m_pTexture{ nullptr }
		, m_textureName{}
		, m_raycastTarget{ true }
	{
		ADD_PROPERTY(m_textureName);
	}
	
	~ImageUI() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override
	{
		LoadResource();
		ReflectLoading();
	}

	void Draw(Renderer& renderer) override;

	// GUI変更時
	void OnValidate() override
	{
		LoadResource();
		ReflectLoading();
	}

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<ImageUI>();
	}

	bool IsRaycastTarget() const { return m_raycastTarget; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetTexture(const std::string& key) 
	{
		m_textureName = key;

		LoadResource();
		ReflectLoading();
	}
	void SetRaycastTarget(bool f) { m_raycastTarget = f; }

	// ---------- リソース関連 ---------- //

	// 読み込みを反映する
	void ReflectLoading() override {};
	
private:
	// リソースタイプ
	Type GetType() const override { return Type::Texture; }

	// キー名
	const std::string& GetKeyName() const override { return m_textureName; }

	// リソースポインタポインタ
	void** GetMyResource() const override { return (void**) &m_pTexture; }
};

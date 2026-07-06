//====================================================//
// ファイル名   : ModelComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/03
//
// 概要 :
//
// 更新履歴 :
// 2026/05/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <Model.h>
#include "RendererBase.h"
#include "System/ResourceManager.h"
#include "Components/Interface/IResourceReader.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ModelComponent : public RendererBase, public IResourceReader
{
	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------
	DirectX::Model* m_model;

	std::string m_modelName;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ModelComponent(IComponentOwner* own)
		: RendererBase(own)
		, m_model{ nullptr }
	{
		ADD_PROPERTY(m_modelName);
	};
	~ModelComponent() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	// 描画関数
	void Draw(Renderer& renderer) override;

	// モデルをセットする関数
	void SetModel(const std::string& keyName)
	{
		m_modelName = keyName;

		LoadResource();
		ReflectLoading();
	}

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<ModelComponent>();
	}

	// ---------- リソース関連 ---------- //

	// 読み込みを反映する
	void ReflectLoading() override {};

private:
	// リソースタイプ
	Type GetType() const override { return Type::Model; }

	// キー名
	const std::string& GetKeyName() const override { return m_modelName; }

	// リソースポインタポインタ
	void** GetMyResource() const override { return (void**)& m_model; }
};

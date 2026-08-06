//====================================================//
// ファイル名  : MaterialTester.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "MaterialTester.h"

#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
MaterialTester::MaterialTester(REngine::IComponentOwner* own)
	: RendererBase(own)
	, m_pTransform{ nullptr }
{
	ADD_PROPERTY(m_material);
}

// 生成直後に一度呼ばれます
void MaterialTester::Awake()
{
}

// 最初のUpdate関数の直線に一度呼ばれます
void MaterialTester::Start()
{}

// 毎フレーム呼ばれます
void MaterialTester::Update(const REngine::GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void MaterialTester::LateUpdate(const REngine::GameTimer & gameTimer)
{}

void MaterialTester::Draw(REngine::Renderer & renderer)
{
	if (!m_pTransform) m_pTransform = GetComponent<REngine::Transform>();

	// マテリアルのセット
	renderer.SetMaterial(m_material);

	// ワールド行列のセット
	renderer.SetWorld(m_pTransform->GetWorldMatrix());

	// テスト描画
	renderer.Draw().Primitive().DrawRect(
		{ -1, 1, 0 }, { 1, 1, 0 }, { 1, -1, 0 }, { -1, -1, 0 }, { 1, 1, 1, 1 }, true
	);

	// マテリアルのリセット
	renderer.SetMaterial(REngine::ERROR_HANDLE<REngine::MaterialAsset>);
}

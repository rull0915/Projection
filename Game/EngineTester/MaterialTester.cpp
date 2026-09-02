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
#include "Assets/Managers/AssetManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
MaterialTester::MaterialTester(REngine::IComponentOwner* own)
	: RendererBase(own)
	, m_pTransform{ nullptr }
	, m_assetManager{ nullptr }
	, m_texture{}
	, m_samplerType{ REngine::SamplerType::None }
	, m_sumTime{ 0 }
	, m_mulColor{  }
{
	ADD_PROPERTY(m_material);
	ADD_PROPERTY(m_texture);
	ADD_PROPERTY(m_samplerType);
	ADD_PROPERTY(m_mulColor);
}

// 生成直後に一度呼ばれます
void MaterialTester::Awake()
{
}

// 最初のUpdate関数の直線に一度呼ばれます
void MaterialTester::Start()
{
	SetBuffer();
}

// 毎フレーム呼ばれます
void MaterialTester::Update(const REngine::GameTimer & gameTimer)
{
	// PSのTime定数を変更する
	m_sumTime += gameTimer.GetElapsedTime();

	// Materialを取得
	REngine::MaterialAsset* material = m_assetManager->Get(m_material);

	if (material)
	{
		material->SetParam(REngine::ShaderType::Pixel, "time", m_sumTime);
	}
}

void MaterialTester::Draw(REngine::Renderer & renderer)
{
	if (!m_pTransform) m_pTransform = GetComponent<REngine::Transform>();

	// マテリアルのセット
	renderer.SetMaterial(m_material);

	// ワールド行列のセット
	renderer.SetWorld(m_pTransform->GetWorldMatrix());

	// テスト描画
	renderer.Draw().Primitive().DrawRect(
		{ -1, 1, 0 }, { 0, 0 },
		{ 1, 1, 0 }, { 1, 0 },
		{ 1, -1, 0 }, { 1, 1 }, 
		{ -1, -1, 0 }, { 0, 1 },
		{ 1, 1, 1, 1 }, true
	);

	// マテリアルのリセット
	renderer.SetMaterial(REngine::ERROR_HANDLE<REngine::MaterialAsset>);

	// ワールド行列のリセット
	renderer.SetWorld(DirectX::SimpleMath::Matrix::Identity);
}

void MaterialTester::OnValidate()
{
	SetBuffer();
}

void MaterialTester::SetBuffer()
{
	// Materialを取得
	REngine::MaterialAsset* material = m_assetManager ? m_assetManager->Get(m_material) : nullptr;

	if (material)
	{
		material->SetParam(REngine::ShaderType::Pixel, "tex", m_texture);
		material->SetParam(REngine::ShaderType::Pixel, "sam", m_samplerType);
		material->SetParam(REngine::ShaderType::Pixel, "mulColor", m_mulColor);
	}
}

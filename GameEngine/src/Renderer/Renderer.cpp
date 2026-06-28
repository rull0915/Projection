//====================================================//
// ファイル名  : Renderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要        :  描画を統括するクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
Renderer::Renderer()
	: m_device{ nullptr }
	, m_nowContext{ nullptr }
	, m_states{ nullptr }
	, m_drawState{}
	, m_proxy{ this, SwitchRendererProxy }
	, m_primitiveRenderer{}
	, m_modelRenderer{}
	, m_spriteRenderer{}
	, m_textRenderer{}
	, m_uiRenderer{}
	, m_isStarted{ false }
	, m_isInitialized{ false }
	, m_nowRenderer{ nullptr }
	, m_nowType{ RendererType::None }
{
}

/// <summary>
/// 初期化関数
/// </summary>
void Renderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, DirectX::CommonStates* state)
{
	if (m_isInitialized) return;

	// メンバの初期化
	m_device = device;

	m_nowContext = context;

	m_states = state;

	// 各レンダーの初期化

	m_primitiveRenderer = std::make_unique<PrimitiveRenderer>(*this);
	m_modelRenderer = std::make_unique<ModelRenderer>(*this);
	m_spriteRenderer = std::make_unique<SpriteRenderer>(*this);
	m_textRenderer = std::make_unique<TextRenderer>(*this);
	m_uiRenderer = std::make_unique<UIRenderer>(*this);

	m_primitiveRenderer->Initialize();
	m_modelRenderer->Initialize();
	m_spriteRenderer->Initialize();
	m_textRenderer->Initialize();
	m_uiRenderer->Initialize();

	// ProxyにRendererを渡す
	m_proxy.SetPrimitive(m_primitiveRenderer.get());
	m_proxy.SetModel(m_modelRenderer.get());
	m_proxy.SetSprite(m_spriteRenderer.get());
	m_proxy.SetText(m_textRenderer.get());
	m_proxy.SetUI(m_uiRenderer.get());

	// フラグのセット
	m_isInitialized = true;

	// レンダラーの初期化

	m_nowRenderer = nullptr;
	m_nowType = RendererType::None;
}

void Renderer::Start(ID3D11DeviceContext* context)
{
	if (m_isStarted) return;

	// コンテキストの更新
	m_nowContext = context;

	// 実行中レンダラーをリセット
	m_nowRenderer = nullptr;
	m_nowType = RendererType::None;

	// フラグのセット
	m_isStarted = true;
}

void Renderer::End()
{
	if (!m_isStarted) return;

	// 実行中のレンダラーの終了処理
	if (m_nowRenderer) m_nowRenderer->End();

	// 実行中レンダラーをリセット
	m_nowRenderer = nullptr;
	m_nowType = RendererType::None;

	m_isStarted = false;
}

RenderProxy& Renderer::Draw()
{
	// 初期化チェック
	assert(m_isInitialized);

	// 描画開始チェック
	assert(m_isStarted);

	// 仲介クラスを返す
	return m_proxy;
}

IRenderer* Renderer::GetRenderer(RendererType type)
{
	// 指定した描画タイプのレンダラーを返す
	switch (type)
	{
	case RendererType::Primitive:
		return m_primitiveRenderer.get();
	case RendererType::Model:
		return m_modelRenderer.get();
	case RendererType::Sprite:
		return m_spriteRenderer.get();
	case RendererType::Text:
		return m_textRenderer.get();
	case RendererType::UI:
		return m_uiRenderer.get();
	default:
		return nullptr;
	}
}

void Renderer::SwitchRenderer(RendererType type)
{
	// 変更がなければスキップ
	if (m_nowType == type) return;

	// 現在の描画の終了処理
	if (m_nowRenderer) m_nowRenderer->End();

	// レンダラーの変更
	m_nowType = type;
	m_nowRenderer = GetRenderer(type);

	// 次のレンダラーの開始処理
	m_nowRenderer->Start();
}

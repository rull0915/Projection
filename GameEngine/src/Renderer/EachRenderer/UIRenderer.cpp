//====================================================//
// ファイル名  : UIRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/09
//
// 概要        :  UI描画クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/EachRenderer/UIRenderer.h"
#include "Renderer/Renderer.h"
#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

#pragma region Setting

/// <summary>
/// コンストラクタ
/// </summary>
UIRenderer::UIRenderer(Renderer& renderer)
	: m_renderer{ renderer }
	, m_primitiveBatch{}
	, m_basicEffect{}
	, m_inputLayout{}
	, m_renderState{ renderer.GetRenderState() }
	, m_isStarted{ false }
{
}

/// <summary>
/// 初期化関数
/// </summary>
void UIRenderer::Initialize()
{
	ID3D11Device* device = m_renderer.GetDevice();
	ID3D11DeviceContext* context = m_renderer.GetContext();

	// プリミティブバッチの初期化
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// ベーシックエフェクトの初期化
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);

	// 設定
	m_basicEffect->SetVertexColorEnabled(true);	// 頂点カラーの使用をオン
	m_basicEffect->SetLightingEnabled(false);	// ライトの使用をオフ
	m_basicEffect->SetTextureEnabled(false);	// テクスチャの使用をオフ

	// インプットレイアウトの作成
	// 例外処理
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()));

	// WVP行列の初期化
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);			// ワールド
	m_basicEffect->SetView(DirectX::SimpleMath::Matrix::Identity);			// ビュー
	m_basicEffect->SetProjection(DirectX::SimpleMath::Matrix::Identity);	// プロジェクション
}

/// <summary>
/// 描画を開始する関数
/// </summary>
/// <param name="context"></param>
/// <param name="states"></param>
void UIRenderer::Start()
{
	// 描画開始済みならスキップ
	if (m_isStarted) return;

	// フラグをセット
	m_isStarted = true;

	// 各項目の設定
	ID3D11DeviceContext* context = m_renderer.GetContext();
	DirectX::CommonStates* states = m_renderer.GetStates();

	// 透明度を使用
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// デプスソートを使用
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリング不使用
	context->RSSetState(states->CullNone());

	// インプットレイアウトをセット
	context->IASetInputLayout(m_inputLayout.Get());

	// サンプラーをリセット
	ID3D11SamplerState* samplers[] = { nullptr };
	context->PSSetSamplers(0, 1, samplers);

	// エフェクトを適用
	m_basicEffect->Apply(context);

	// 描画の開始
	m_primitiveBatch->Begin();
}

void UIRenderer::End()
{
	// 開始済みでなければスキップ
	if (!m_isStarted) return;

	// 描画終了
	m_primitiveBatch->End();

	// 描画開始フラグをオフに
	m_isStarted = false;
}

#pragma endregion

#pragma region Draw

void UIRenderer::DrawTriangle(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Color color, bool fillFlag)
{
	color.w = m_renderState.GetAlpha();

	DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(p1), color);
	DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(p2), color);
	DirectX::VertexPositionColor v3(WindowManager::Instance().ScreenToPixel(p3), color);

	DirectX::VertexPositionColor vertices[3] = { v1, v2, v3 };

	// 描画

	// 塗りつぶすなら
	if (fillFlag)
	{
		m_primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vertices, 3);
	}
	else
	{
		uint16_t indices[] = { 0, 1, 1, 2, 2, 0 };
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_LINELIST, indices, 6, vertices, 3);
	}
}

void UIRenderer::DrawRect(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Vector2 p4, DirectX::SimpleMath::Color color, bool fillFlag)
{
	color.w = m_renderState.GetAlpha();

	DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(p1), color);
	DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(p2), color);
	DirectX::VertexPositionColor v3(WindowManager::Instance().ScreenToPixel(p3), color);
	DirectX::VertexPositionColor v4(WindowManager::Instance().ScreenToPixel(p4), color);

	DirectX::VertexPositionColor vertices[4] = { v1, v2, v3, v4 };

	// 塗りつぶすなら
	if (fillFlag)
	{
		uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);
	}
	else
	{
		uint16_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_LINELIST, indices, 8, vertices, 4);
	}
}

void UIRenderer::DrawLine(DirectX::SimpleMath::Vector2 start, DirectX::SimpleMath::Vector2 end, DirectX::SimpleMath::Color color)
{
	color.w = m_renderState.GetAlpha();

	DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(start), color);
	DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(end), color);

	m_primitiveBatch->DrawLine(v1, v2);
}

void UIRenderer::DrawCircle(DirectX::SimpleMath::Vector2 centerPos, float radius, uint16_t division, DirectX::SimpleMath::Color color, bool fillFlag)
{
	if (division < 3) return;

	// --- 色の展開 ---
	color.w = m_renderState.GetAlpha();

	// 角度の間隔を算出
	float step = 2.0f * PI_F / static_cast<float>(division);

	// 頂点座標を格納する配列を用意
	std::vector<DirectX::VertexPositionColor> vertices;
	vertices.resize(division + 1);

	// 分割数分ループ
	for (int i = 0; i < division; ++i)
	{
		float theta = step * i;

		// 頂点情報を計算
		DirectX::VertexPositionColor v(WindowManager::Instance().ScreenToPixel(DirectX::SimpleMath::Vector2{ centerPos.x + cosf(theta) * radius, centerPos.y + sinf(theta) * radius }), color);

		vertices[i] = v;
	}

	std::vector<uint16_t> indices;

	// 塗りつぶすとき
	if (fillFlag)
	{
		vertices[division] = DirectX::VertexPositionColor(WindowManager::Instance().ScreenToPixel(centerPos), color);

		indices.resize(division * 3);

		for (uint16_t i = 0; i < division; i++)
		{
			indices[i * 3 + 0] = i;
			indices[i * 3 + 1] = (i + 1) % division;
			indices[i * 3 + 2] = division;
		}

		// 三角形を構築して渡す
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices.data(), division * 3, vertices.data(), division + 1);
	}
	// ワイヤーフレームの時
	else
	{
		indices.resize(division * 2);

		for (uint16_t i = 0; i < division; i++)
		{
			indices[i * 2] = i;
			indices[i * 2 + 1] = (i + 1) % division;
		}

		// 線を構築して渡す
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_LINELIST, indices.data(), division * 2, vertices.data(), division);
	}
}

void UIRenderer::DrawBox(DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Color color, bool fillFlag)
{
	color.w = m_renderState.GetAlpha();

	DirectX::SimpleMath::Vector2 p1{ min.x, min.y };
	DirectX::SimpleMath::Vector2 p2{ min.x, max.y };
	DirectX::SimpleMath::Vector2 p3{ max.x, max.y };
	DirectX::SimpleMath::Vector2 p4{ max.x, min.y };

	DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(p1), color);
	DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(p2), color);
	DirectX::VertexPositionColor v3(WindowManager::Instance().ScreenToPixel(p3), color);
	DirectX::VertexPositionColor v4(WindowManager::Instance().ScreenToPixel(p4), color);

	DirectX::VertexPositionColor vertices[4] = { v1, v2, v3, v4 };

	// 塗りつぶすなら
	if (fillFlag)
	{
		uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);
	}
	else
	{
		uint16_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
		m_primitiveBatch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_LINELIST, indices, 8, vertices, 4);
	}
}

#pragma endregion

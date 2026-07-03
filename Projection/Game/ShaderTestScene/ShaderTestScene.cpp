//====================================================//
// ファイル名  : ShaderTestScene.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/03
//
// 概要       : ShaderTestシーン
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ShaderTestScene.h"

#include "Game/Game.h"
#include "Components/World/Camera/Derived/StandardCamera.h"

#include "System/ResourceManager.h"

// シェーダーをコンパイルできるlibをリンク
#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ShaderTestScene::ShaderTestScene(Game* pGame)
	: Scene(pGame->GetSceneManager())
	, m_pGame{ pGame }
	, m_camera{ nullptr }
{
}

ShaderTestScene::~ShaderTestScene()
{
}

// 初期化関数
void ShaderTestScene::Initialize()
{
	// オブジェクトの追加
	// カメラ
	m_camera = Generate({ 0, 10, 0});

	// メインカメラに設定
	auto cameraComponent = m_camera->AddComponent<StandardCamera>();
	SetMainCamera(cameraComponent);

	// シェーダーの初期化
	InitializeShader();
}

// 更新関数
void ShaderTestScene::Update(const GameTimer& gameTimer)
{
	gameTimer;
}

// 描画関数
void ShaderTestScene::Render(Renderer& renderer)
{
	auto* context = ResourceManager::Instance().GetResources()->GetD3DDeviceContext();

	// 1. C++側のデータを準備（行列の計算など）
	ConstBuffer cbData;

	cbData.world = renderer.GetRenderState().GetWorld();
	cbData.view = renderer.GetRenderState().GetView();
	cbData.projection = renderer.GetRenderState().GetProjection();
	cbData.lightDir = { 0, -1, 0 };

	// 2. 準備したデータを定数バッファに転送（Map/Unmap）
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(m_constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &cbData, sizeof(ConstBuffer));
	context->Unmap(m_constBuffer.Get(), 0);

	// 3. 【ここがポイント！】初期化時に作っておいたシェーダー達をセットするだけ
	context->IASetInputLayout(m_inputLayout.Get());
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// ポストにバッファを紐付ける
	context->VSSetConstantBuffers(0, 1, m_constBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_constBuffer.GetAddressOf());

	UINT size = sizeof(ID3D11Buffer);
	UINT offset = 0;

	// 4. 描画命令（DirectXTKのモデル描画や、Draw命令など）
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &size,&offset);
	// context->IASetIndexBuffer();
}

// 終了関数
void ShaderTestScene::Finalize()
{
}

void ShaderTestScene::InitializeShader()
{
	// 一時的に使うBlob
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

	auto* device = ResourceManager::Instance().GetResources()->GetD3DDevice();

	// コンパイル

	// 頂点シェーダ
	D3DCompileFromFile(L"Shaders/VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);

	// ピクセルシェーダ
	D3DCompileFromFile(L"Shaders/PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);

	// インプットレイアウトの作成 頂点シェーダの引数と同じに
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);

	// 定数バッファの作成
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(ConstBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&cbDesc, nullptr, &m_constBuffer);

	// 頂点バッファの作成
	DirectX::VertexPositionColor vertices[] =
	{
		{ DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, 0.0f }, DirectX::SimpleMath::Vector4{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ DirectX::SimpleMath::Vector3{ 1.0f, 0.0f, 0.0f }, DirectX::SimpleMath::Vector4{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ DirectX::SimpleMath::Vector3{ 1.0f, 1.0f, 0.0f }, DirectX::SimpleMath::Vector4{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ DirectX::SimpleMath::Vector3{ 0.0f, 1.0f, 0.0f }, DirectX::SimpleMath::Vector4{ 1.0f, 1.0f, 1.0f, 1.0f } },
	};

	D3D11_BUFFER_DESC desc = { sizeof(vertices), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER };
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	device->CreateBuffer(&desc, &initData, m_vertexBuffer.GetAddressOf());

	// インデックスバッファの作成
	uint16_t indices[] =
	{ 0, 1, 2, 1, 2, 3 };

	D3D11_BUFFER_DESC indexDesc = { sizeof(indices), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER };
	D3D11_SUBRESOURCE_DATA initIndex = {};
	initIndex.pSysMem = indices;

	device->CreateBuffer(&indexDesc, &initIndex, m_indexBuffer.GetAddressOf());
}

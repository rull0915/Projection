//====================================================//
// ファイル名  : ShaderLoader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/04
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Types/Shader/ShaderLoader.h"

#include "Assets/Types/Shader/ShaderAsset.h"
#include "System/GraphicsManager.h"
#include "ShaderReflection.h"

#include <filesystem>
#include <d3dcompiler.h>

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	namespace Loader
	{
		std::unique_ptr<ShaderAsset> ShaderLoader(const std::wstring& path)
		{
			// パスに変換
			std::filesystem::path p(path);

			// シェーダーアセットを生成
			std::unique_ptr<ShaderAsset> shader = std::make_unique<ShaderAsset>();

			// サブ拡張子を取得
			// シェーダーファイルは.ps.csoのように扱います。
			// このとき、.psの部分によってどのステージのシェーダーであるかを判別します。
			// ピクセルシェーダであれば.ps.cso
			// 頂点シェーダであれば.vs.cso	とします。
			std::string ext = p.stem().extension().string();

			// Blobを生成
			Microsoft::WRL::ComPtr<ID3DBlob> blob;

			// コンパイル済みシェーダーからバイナリ文字列を取得
			HRESULT hr = D3DReadFileToBlob(p.c_str(), &blob);

			// 失敗したら早期リターン
			if (FAILED(hr)) return nullptr;

			// デバイスを取得
			ID3D11Device* device = GraphicsManager::Instance().GetDeviceResources()->GetD3DDevice();

			// 拡張子によって処理を変更
			if (ext == ".vs")	// 頂点シェーダの場合
			{
				// 頂点シェーダに設定
				shader->m_type = ShaderType::Vertex;

				// 頂点シェーダを生成
				Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;

				// バイナリファイルから読み取る
				if (FAILED(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vs.GetAddressOf()))) return nullptr;

				// シェーダを渡す
				shader->m_shader = vs;

				// インプットレイアウトの作成
				Reflection::CreateInputLayout(device, blob, shader->m_inputLayout);
			}
			else if (ext == ".ps")	// ピクセルシェーダの場合
			{
				// ピクセルシェーダに設定
				shader->m_type = ShaderType::Pixel;

				// ピクセルシェーダを生成
				Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;

				// バイナリファイルから読み取る
				if (FAILED(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, ps.GetAddressOf()))) return nullptr;

				// シェーダを渡す
				shader->m_shader = ps;
			}
			else
			{
				// 未対応の拡張子
				return nullptr;
			}

			// データの作成
			Reflection::ReflectShader(
				blob, shader->m_params, shader->m_cBuffers
			);

			// blobを渡す
			shader->m_byteCode = blob;

			return shader;
		}
	}
}

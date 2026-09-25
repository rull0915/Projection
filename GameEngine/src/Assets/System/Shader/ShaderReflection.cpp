//====================================================//
// ファイル名  : ShaderReflection.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/05
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ShaderReflection.h"

#include <iostream>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	namespace Reflection
	{
		HRESULT CreateInputLayout(ID3D11Device* device, ID3DBlob* vsBlob, const std::vector<VertexElementInfo>& layoutInfo, Microsoft::WRL::ComPtr<ID3D11InputLayout>& outInputLayout)
		{
			// 引数が不足している場合
			if (!device || !vsBlob || layoutInfo.empty()) return E_INVALIDARG;

			//----- シェーダーリフレクションの初期化 -----//

			// リフレクションを受け取る変数
			Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflection;

			// リフレクト
			HRESULT hr = D3DReflect(
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				IID_ID3D11ShaderReflection,
				reinterpret_cast<void**>(reflection.GetAddressOf())
			);

			// 例外処理
			if (FAILED(hr))
			{
				std::cerr << "D3DReflectの実行に失敗しました。" << std::endl;
				return hr;
			}

			//----- VS_INPUT情報の取得 -----//

			// Shaderの情報を受け取る変数
			D3D11_SHADER_DESC shaderDesc{};

			// 取得
			reflection->GetDesc(&shaderDesc);

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

			// 入力の要素数分ループ
			for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
			{
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc{};
				reflection->GetInputParameterDesc(i, &paramDesc);

				// VSが要求しているセマンティックと一致する要素をから探索
				auto it = std::find_if(layoutInfo.begin(), layoutInfo.end(),
					[&paramDesc](const VertexElementInfo& info)
					{
						// セマンティック名とインデックスの一致チェック
						return (info.semanticName == paramDesc.SemanticName) &&
							(info.semanticIndex == paramDesc.SemanticIndex);
					});

				// 一致した場合
				if (it != layoutInfo.end())
				{
					D3D11_INPUT_ELEMENT_DESC elem{};

					// VertexElementInfoから情報を取得
					elem.SemanticName = paramDesc.SemanticName;
					elem.SemanticIndex = paramDesc.SemanticIndex;
					elem.Format = it->format;
					elem.InputSlot = it->inputSlot;
					elem.AlignedByteOffset = it->byteOffset;

					elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 頂点毎のデータ
					elem.InstanceDataStepRate = 0;

					inputElements.push_back(elem);
				}

				// 例外処理
				else
				{
					// VSが要求している情報がない場合はエラーログを出力
					std::cerr << "VSが要求するセマンティック '"
						<< paramDesc.SemanticName << paramDesc.SemanticIndex
						<< "' が C++ 頂点構造体側に存在しません。" << std::endl;
					return E_FAIL;
				}
			}

			//----- InputLayout を作成 -----//
			hr = device->CreateInputLayout(
				inputElements.data(),
				static_cast<UINT>(inputElements.size()),
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				outInputLayout.ReleaseAndGetAddressOf()
			);

			// 例外処理
			if (FAILED(hr))
			{
				std::cerr << "CreateInputLayout の呼び出しに失敗しました。" << std::endl;
			}

			return hr;
		}

		void ReflectShader(const Microsoft::WRL::ComPtr<ID3DBlob>& blob, std::vector<ShaderParam>& params, std::vector<ConstantBufferInfo>& infos)
		{
			// リフレクションを受け取る変数
			Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflection;

			// リフレクト
			HRESULT hr = D3DReflect(
				blob->GetBufferPointer(),	// バイナリデータの先頭のアドレス
				blob->GetBufferSize(),		// バイナリデータのサイズ
				IID_ID3D11ShaderReflection,
				reinterpret_cast<void**>(reflection.GetAddressOf()));

			// 失敗したらリターン
			if (FAILED(hr)) return;

			// Shaderの情報を取得する
			D3D11_SHADER_DESC desc;
			reflection->GetDesc(&desc);

			// 定数バッファを全て調べる
			for (UINT cbIndex = 0; cbIndex < desc.ConstantBuffers; ++cbIndex)
			{
				// インターフェース取得
				auto* c = reflection->GetConstantBufferByIndex(cbIndex);

				// descの取得
				D3D11_SHADER_BUFFER_DESC cbDesc;
				if (FAILED(c->GetDesc(&cbDesc))) continue;

				// バインド情報の取得
				D3D11_SHADER_INPUT_BIND_DESC bindDesc;
				if (FAILED(reflection->GetResourceBindingDescByName(cbDesc.Name, &bindDesc))) continue;

				// スロット番号の取得
				UINT slotNum = bindDesc.BindPoint;

				// 定数バッファ情報を作成
				ConstantBufferInfo info{};

				info.slot = slotNum;
				info.size = cbDesc.Size;

				// リストに追加
				infos.push_back(info);

				// 1つの定数バッファにある変数を全て調べる
				for (UINT i = 0; i < cbDesc.Variables; ++i)
				{
					// インターフェース取得
					auto* v = c->GetVariableByIndex(i);

					// 情報を取得
					D3D11_SHADER_VARIABLE_DESC vaDesc;
					if (FAILED(v->GetDesc(&vaDesc))) continue;

					// パラメータを作成
					ShaderParam param{};

					param.name = vaDesc.Name;
					param.slot = slotNum;
					param.offset = vaDesc.StartOffset;
					param.size = vaDesc.Size;
					param.type = GetTypeFromInterface(v);

					// 配列に追加
					params.push_back(param);
				}
			}

			// 全てのリソースを調べる
			for (UINT i = 0; i < desc.BoundResources; ++i)
			{
				// バインドデスクを取得
				D3D11_SHADER_INPUT_BIND_DESC bindDesc;
				if (FAILED(reflection->GetResourceBindingDesc(i, &bindDesc))) continue;

				// テクスチャなら
				if (bindDesc.Type == D3D_SIT_TEXTURE || bindDesc.Type == D3D_SIT_SAMPLER)
				{
					// パラメータを作成
					ShaderParam param{};

					param.name = bindDesc.Name;
					param.slot = bindDesc.BindPoint;
					param.type = bindDesc.Type == D3D_SIT_TEXTURE ? 
						ShaderParamType::Texture2D : ShaderParamType::Sampler;

					// 配列に追加
					params.push_back(param);
				}
			}
		}

		ShaderParamType GetTypeFromInterface(ID3D11ShaderReflectionVariable* var)
		{
			// タイプ情報を取得
			auto* iType = var->GetType();

			// 情報を取得
			D3D11_SHADER_TYPE_DESC desc;
			if (FAILED(iType->GetDesc(&desc))) return ShaderParamType::None;

			// 型の種類で分岐
			switch (desc.Class)
			{
				// スカラーの時
			case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_SCALAR:

				switch (desc.Type)
				{
					// floatの時
				case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT: return ShaderParamType::Float;
				default: break;
				}

				break;
				// ベクトルの時
			case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_VECTOR:

				switch (desc.Type)
				{
					// floatの時
				case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
					// 要素数
					switch (desc.Columns)
					{
					case 2: return ShaderParamType::Float2;
					case 3: return ShaderParamType::Float3;
					case 4: return ShaderParamType::Float4;
					default: break;
					}
					break;
				default: break;
				}

				break;
				// 行列の時
			case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_COLUMNS:
			case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_ROWS:

				switch (desc.Type)
				{
					// floatの時
				case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:

					// 4*4なら
					if (desc.Rows == 4 && desc.Columns == 4)
						return ShaderParamType::Float4x4;

					break;
				default: break;
				}

				break;
			default:
				break;
			}

			// 未対応の型の場合の例外処理
			return ShaderParamType::None;
		}
	}
}	// namespace REngine

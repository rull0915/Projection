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

#include "FormatGetter.h"
#include <d3dcompiler.h>

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{

	namespace Reflection
	{
		// InputLayoutを作成する関数
		void CreateInputLayout(ID3D11Device* device, const Microsoft::WRL::ComPtr<ID3DBlob>& blob, Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout)
		{
			// リフレクションを受け取る変数
			Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflection;

			// リフレクト
			HRESULT hr = D3DReflect(
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				IID_ID3D11ShaderReflection,
				(void**)reflection.GetAddressOf());

			// 失敗したらリターン
			if (FAILED(hr)) return;

			// Shaderの情報を取得する
			D3D11_SHADER_DESC desc;
			reflection->GetDesc(&desc);

			// InputLayoutの要素を格納する配列
			std::vector<D3D11_INPUT_ELEMENT_DESC> elements(desc.InputParameters);

			// Inputのパラメータを全て調べる
			for (UINT i = 0; i < desc.InputParameters; i++)
			{
				// 受け取る変数を用意
				D3D11_SIGNATURE_PARAMETER_DESC param;

				// i番目のパラメータを受け取る
				reflection->GetInputParameterDesc(i, &param);

				// 入力パラメータを生成
				// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
				D3D11_INPUT_ELEMENT_DESC elem{};
				elem.SemanticName = param.SemanticName;					// セマンティック名
				elem.SemanticIndex = param.SemanticIndex;				// セマンティック番号
				elem.Format = Format::GetFormat(param.ComponentType, param.Mask);	// フォーマット
				elem.InputSlot = 0;										// 単一頂点バッファ前提
				elem.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// オフセット自動計算
				elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		// 頂点毎のデータ
				elem.InstanceDataStepRate = 0;

				// 配列に追加
				elements[i] = elem;
			}

			// InputLayoutの作成
			hr = device->CreateInputLayout(elements.data(), (UINT)elements.size(), blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf());

			// 失敗時処理
			if (FAILED(hr))
			{
				// Todo
			}
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
					param.cbSlot = slotNum;
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
				if (bindDesc.Type == D3D_SIT_TEXTURE)
				{
					// パラメータを作成
					ShaderParam param{};

					param.name = bindDesc.Name;
					param.tSlot = bindDesc.BindPoint;
					param.type = ShaderParamType::Texture2D;

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
						return ShaderParamType::Float4×4;

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

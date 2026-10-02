//====================================================//
// ファイル名   : ShaderReflections.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/04
//
// 概要 : ShaderReflectionを扱う関数群
//
// 更新履歴 :
// 2026/08/04 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <d3d11shader.h>
#include "Assets/Types/Shader/ShaderParam.h"
#include "Assets/Types/Vertex/VertexElementInfo.h"

namespace REngine
{
	namespace Reflection
	{
		/// <summary>
		/// VertexElementInfoとVertexShaderのバイナリからInputLayoutを作成する関数
		/// </summary>
		/// <param name="device">D3D11デバイス</param>
		/// <param name="vsBlob">VertexShader のコンパイル済みバイナリ</param>
		/// <param name="layoutInfo">C++頂点構造体から取得した VertexElementInfo の配列</param>
		/// <param name="outInputLayout">生成される ID3D11InputLayout</param>
		/// <returns>結果</returns>
		HRESULT CreateInputLayout(
			ID3D11Device* device,
			ID3DBlob* vsBlob,
			const std::vector<VertexElementInfo>& layoutInfo,
			Microsoft::WRL::ComPtr<ID3D11InputLayout>& outInputLayout
		);

		/// <summary>
		/// シェーダーを読み取ってエンジン用のデータを作成する関数
		/// </summary>
		/// <param name="blob">Shaderのコンパイル済みバイナリ</param>
		/// <param name="params">結果を格納するパラメータ配列</param>
		/// <param name="infos">定数バッファ全体の情報を格納する配列</param>
		void ReflectShader(const Microsoft::WRL::ComPtr<ID3DBlob>& blob, std::vector<ShaderParam>& params, std::vector<ConstantBufferInfo>& infos);

		// リフレクションインターフェースからタイプを取得する関数
		ShaderParamType GetTypeFromInterface(ID3D11ShaderReflectionVariable* var, const std::string& name);
	}
}	// namespace REngine

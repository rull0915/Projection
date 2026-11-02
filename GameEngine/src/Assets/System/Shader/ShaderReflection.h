/**
 * @file    ShaderReflections.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/04
 *
 * @brief   ShaderReflectionを扱う関数
 */

#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "Assets/Types/Shader/ShaderParam.h"
#include "Assets/Types/Vertex/VertexElementInfo.h"

namespace RHEngine
{
	namespace Reflection
	{
		/**
		 * @brief VertexElementInfoとVertexShaderのバイナリからInputLayoutを作成する関数
		 * @param pDevice D3D11デバイス
		 * @param pVsBlob VertexShader のコンパイル済みバイナリ
		 * @param layoutInfo C++頂点構造体から取得した VertexElementInfo の配列
		 * @param outInputLayout 生成される ID3D11InputLayout
		 * @return 結果
		 */
		HRESULT CreateInputLayout(
			ID3D11Device* pDevice,
			ID3DBlob* pVsBlob,
			const std::vector<VertexElementInfo>& layoutInfo,
			Microsoft::WRL::ComPtr<ID3D11InputLayout>& outInputLayout
		);

		/**
		 * @brief シェーダーを読み取ってエンジン用の定数バッファデータを作成する関数
		 * @param blob Shaderのコンパイル済みバイナリ
		 * @param params 結果を格納するパラメータ配列
		 * @param infos 定数バッファ全体の情報を格納する配列
		 */
		void ReflectShader(const Microsoft::WRL::ComPtr<ID3DBlob>& blob, std::vector<ShaderParam>& params, std::vector<ConstantBufferInfo>& infos);

		/**
		 * @brief リフレクションからパラメータのタイプを取得する
		 * @param pVar リフレクション
		 * @param name パラメータ名
		 * @return タイプ
		 */
		ShaderParamType GetTypeFromInterface(ID3D11ShaderReflectionVariable* pVar, const std::string& name);
	}
}	// namespace RHEngine

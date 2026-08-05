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

#include "Assets/Types/Shader/ShaderParam.h"
#include <d3d11shader.h>

namespace REngine
{
	namespace Reflection
	{
		// InputLayoutを作成する関数
		void CreateInputLayout(ID3D11Device* device, const Microsoft::WRL::ComPtr<ID3DBlob>& blob, Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout);

		// シェーダーを読み取ってエンジン用のデータを作成する関数
		void ReflectShader(const Microsoft::WRL::ComPtr<ID3DBlob>& blob, std::vector<ShaderParam>& params, std::vector<ConstantBufferInfo>& infos);

		// リフレクションインターフェースからタイプを取得する関数
		ShaderParamType GetTypeFromInterface(ID3D11ShaderReflectionVariable* var);
	}
}	// namespace REngine

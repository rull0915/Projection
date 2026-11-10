/**
 * @file    Texture.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/10
 */

#include "pch.h"
#include "Assets/Types/Texture.h"

namespace RHEngine
{
	Texture::Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
		: m_srv{ srv }
		, m_dimention{}
		, m_format{}
	{
		//----- 画像の情報を調べる -----//

		// DESCを取得
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		m_srv->GetDesc(&desc);

		// フォーマットを保存
		m_format = desc.Format;

		// 次元を保存
		m_dimention = desc.ViewDimension;
	}
}

/**
 * @file    DepthStencilStateList.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/02
 */

#include "pch.h"
#include "System/Render/DepthStencilStateList.h"

namespace RHEngine
{
	void DepthStencilStateList::Initialize(ID3D11Device* pDevice)
	{

	}

	const Microsoft::WRL::ComPtr<ID3D11SamplerState>& DepthStencilStateList::GetDepthStencilState(DepthStencilStateType type) const
	{
		// TODO: return ステートメントをここに挿入します
	}

	void DepthStencilStateList::CreateSampler(ID3D11Device* device, DepthStencilStateType type, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode)
	{
		D3D11_DEPTH_STENCIL_DESC desc{};

		desc.

		device->CreateDepthStencilState();
	}
}

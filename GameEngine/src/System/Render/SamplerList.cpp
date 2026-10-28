/**
 * @file    SamplerList.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/08
 */

#include "pch.h"
#include "System/Render/SamplerList.h"

namespace RHEngine
{
	void SamplerList::Initialize(ID3D11Device* device)
	{
		// 各タイプを作成する
		CreateSampler(device, SamplerType::LinearWrap, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
		CreateSampler(device, SamplerType::PointWrap, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);
		CreateSampler(device, SamplerType::LinearClamp, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
		CreateSampler(device, SamplerType::PointClamp, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);
		CreateSampler(device, SamplerType::Anisotropic, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	const Microsoft::WRL::ComPtr<ID3D11SamplerState>& SamplerList::GetSampler(SamplerType type) const
	{
		// 作成済みのタイプなら
		if (auto it = m_samplers.find(type); it != m_samplers.end())
		{
			// サンプラーを返す
			return it->second;
		}

		static Microsoft::WRL::ComPtr<ID3D11SamplerState> empty = nullptr;

		// 未作成ならnullptr
		return empty;
	}

	void SamplerList::CreateSampler(ID3D11Device* device, SamplerType type, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode)
	{
		// Descの作成
		D3D11_SAMPLER_DESC desc{};

		// 設定
		desc.Filter = filter;	// テスト
		desc.AddressU = desc.AddressV = desc.AddressW = mode;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		// サンプラーの作成
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		device->CreateSamplerState(&desc, sampler.GetAddressOf());

		// マップに追加
		m_samplers[type] = sampler;
	}
}	// namespace RHEngine

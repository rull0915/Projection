/**
 * @file    RasterizerStateList.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/04
 */

#include "pch.h"
#include "System/Render/RasterizerStateList.h"

namespace RHEngine
{
	/** @brief このファイルでのみ使用可能にするための無名名前空間 */
	namespace
	{
		/**
		 * @brief 1変数のハッシュを結合する
		 * @param seed 対象のハッシュ値
		 * @param v 対象の変数
		 */
		inline void HashCombine(size_t& seed, size_t v)
		{
			seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	}

	size_t RasterizerStateList::DescHash::operator()(const D3D11_RASTERIZER_DESC& d) const
	{
		size_t seed = 0;

		// 全変数のハッシュを結合
		HashCombine(seed, static_cast<size_t>(d.AntialiasedLineEnable));
		HashCombine(seed, static_cast<size_t>(d.CullMode));
		HashCombine(seed, static_cast<size_t>(d.DepthBias));
		HashCombine(seed, static_cast<size_t>(d.DepthBiasClamp));
		HashCombine(seed, static_cast<size_t>(d.DepthClipEnable));
		HashCombine(seed, static_cast<size_t>(d.FillMode));
		HashCombine(seed, static_cast<size_t>(d.FrontCounterClockwise));
		HashCombine(seed, static_cast<size_t>(d.MultisampleEnable));
		HashCombine(seed, static_cast<size_t>(d.ScissorEnable));
		HashCombine(seed, static_cast<size_t>(d.SlopeScaledDepthBias));

		return seed;
	}

	bool RasterizerStateList::DescEqual::operator()(const D3D11_RASTERIZER_DESC& a, const D3D11_RASTERIZER_DESC& b) const
	{
		return
			// 全変数の一致をチェック
			a.AntialiasedLineEnable == b.AntialiasedLineEnable &&
			a.CullMode == b.CullMode &&
			a.DepthBias == b.DepthBias &&
			a.DepthBiasClamp == b.DepthBiasClamp &&
			a.DepthClipEnable == b.DepthClipEnable &&
			a.FillMode == b.FillMode &&
			a.FrontCounterClockwise == b.FrontCounterClockwise &&
			a.MultisampleEnable == b.MultisampleEnable &&
			a.ScissorEnable == b.ScissorEnable &&
			a.SlopeScaledDepthBias == b.SlopeScaledDepthBias;
	}

	void RasterizerStateList::Initialize(ID3D11Device* pDevice)
	{
		// デバイスを保持
		m_pDevice = pDevice;

		using Type = RasterizerType;

		auto add = [this](Type type, const D3D11_RASTERIZER_DESC& desc)
			{
				m_presets[type] = GetOrCreate(desc);
			};

		add(Type::Default, MakeRasterizerDesc({}));
		add(Type::CullNone, MakeRasterizerDesc({ .cullMode = D3D11_CULL_NONE }));
		add(Type::CullFront, MakeRasterizerDesc({ .cullMode = D3D11_CULL_FRONT }));
		add(Type::Wireframe, MakeRasterizerDesc({ .fillMode = D3D11_FILL_WIREFRAME,
												  .cullMode = D3D11_CULL_NONE }));
		add(Type::Scissor, MakeRasterizerDesc({ .scissorEnable = true }));
		add(Type::ShadowMap, MakeRasterizerDesc({	.cullMode = D3D11_CULL_FRONT,
													.depthBias = 1000,
													.slopeScaledDepthBias = 1.5f }));
	}

	const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& RasterizerStateList::GetState(RasterizerType type)
	{
		// プリセットを検索
		auto it = m_presets.find(type);

		// なかった場合
		assert(it != m_presets.end());

		return it->second;
	}

	const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& RasterizerStateList::GetOrCreate(const D3D11_RASTERIZER_DESC& desc)
	{
		// キャッシュを検索
		auto it = m_cache.find(desc);

		// 見つかれば返す
		if (it != m_cache.end()) return it->second;

		// なければ作成する
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pState{};
		HRESULT hr = m_pDevice->CreateRasterizerState(&desc, pState.GetAddressOf());

		if (FAILED(hr))
		{
			// エラー時用の静的変数
			static Microsoft::WRL::ComPtr<ID3D11RasterizerState> s_error{};

			assert(false && "Failed to create RasterizerState.");
			return s_error;
		}

		// キャッシュに追加
		return m_cache.emplace(desc, pState).first->second;
	}

	D3D11_RASTERIZER_DESC RasterizerStateList::MakeRasterizerDesc(const DescInfo& info)
	{
		D3D11_RASTERIZER_DESC desc{};

		desc.FillMode = info.fillMode;
		desc.CullMode = info.cullMode;
		desc.DepthBias = info.depthBias;
		desc.SlopeScaledDepthBias = info.slopeScaledDepthBias;
		desc.DepthBiasClamp = info.depthBiasClamp;
		desc.ScissorEnable = info.scissorEnable;
		desc.FrontCounterClockwise = info.frontCounterClockwise;
		desc.DepthClipEnable = info.depthClipEnable;
		desc.MultisampleEnable = info.multisampleEnable;
		desc.AntialiasedLineEnable = info.antialiasedLineEnable;

		return desc;
	}
}

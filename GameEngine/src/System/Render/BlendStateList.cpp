/**
 * @file    BlendStateList.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/02
 */

#include "pch.h"
#include "System/Render/BlendStateList.h"

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

		/**
		 * @brief D3D11_DEPTH_STENCILOP_DESCを比較する
		 */
		inline bool Equal(const D3D11_RENDER_TARGET_BLEND_DESC& a, const D3D11_RENDER_TARGET_BLEND_DESC& b)
		{
			return a.BlendEnable == b.BlendEnable
				&& a.BlendOp == b.BlendOp
				&& a.BlendOpAlpha == b.BlendOpAlpha
				&& a.DestBlend == b.DestBlend
				&& a.DestBlendAlpha == b.DestBlendAlpha
				&& a.RenderTargetWriteMask == b.RenderTargetWriteMask
				&& a.SrcBlend == b.SrcBlend
				&& a.SrcBlendAlpha == b.SrcBlendAlpha;
		}

		/**
		 * @brief D3D11_DEPTH_STENCILOP_DESCをハッシュ化する
		 */
		inline void Hash(size_t& seed, const D3D11_RENDER_TARGET_BLEND_DESC& o)
		{
			HashCombine(seed, static_cast<size_t>(o.BlendEnable));
			HashCombine(seed, static_cast<size_t>(o.BlendOp));
			HashCombine(seed, static_cast<size_t>(o.BlendOpAlpha));
			HashCombine(seed, static_cast<size_t>(o.DestBlend));
			HashCombine(seed, static_cast<size_t>(o.DestBlendAlpha));
			HashCombine(seed, static_cast<size_t>(o.RenderTargetWriteMask));
			HashCombine(seed, static_cast<size_t>(o.SrcBlend));
			HashCombine(seed, static_cast<size_t>(o.SrcBlendAlpha));
		}
	}

	size_t BlendStateList::DescHash::operator()(const D3D11_BLEND_DESC& d) const
	{
		size_t seed = 0;

		// 全変数のハッシュを結合
		HashCombine(seed, static_cast<size_t>(d.AlphaToCoverageEnable));
		HashCombine(seed, static_cast<size_t>(d.IndependentBlendEnable));

		// 複数ブレンド使用の場合は全て、未使用なら0版のみをハッシュ化
		size_t rtCount = d.IndependentBlendEnable ? 8 : 1;

		for (size_t i = 0; i < rtCount; i++)
		{
			Hash(seed, d.RenderTarget[i]);
		}

		return seed;
	}

	bool BlendStateList::DescEqual::operator()(const D3D11_BLEND_DESC& a, const D3D11_BLEND_DESC& b) const
	{
		// 全変数の一致をチェック
		bool flag = a.AlphaToCoverageEnable == b.AlphaToCoverageEnable && a.IndependentBlendEnable == b.IndependentBlendEnable;

		if (!flag) return false;

		// 複数ブレンド使用の場合は全て、未使用なら0版のみを比較
		size_t rtCount = a.IndependentBlendEnable ? 8 : 1;

		for (size_t i = 0; i < rtCount; i++)
		{
			if (!Equal(a.RenderTarget[0], b.RenderTarget[0])) return false;
		}

		return true;
	}

	void BlendStateList::Initialize(ID3D11Device* pDevice)
	{
		// デバイスを保持
		m_pDevice = pDevice;

		using Type = BlendType;

		// プリセットを登録するヘルパー
		auto add = [this](Type type, const D3D11_BLEND_DESC& desc)
			{
				m_presets[type] = GetOrCreate(desc);
			};

		// 色を書かない / ブレンドしない設定は共通なので先に作る
		const auto noBlend = MakeRenderTargetBlendDesc(
			false,
			D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD);

		// 不透明:ブレンドなし(上書き)
		add(Type::Opaque, MakeBlendDesc(false, noBlend));

		// 通常の半透明:色 = Src×α + Dest×(1-α)
		add(Type::AlphaBlend, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD)));

		// 乗算済みアルファ:色 = Src + Dest×(1-α)
		add(Type::PremultipliedAlpha, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD)));

		// 加算(α付き):色 = Src×α + Dest
		add(Type::Additive, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD)));

		// 加算(αなし):色 = Src + Dest
		add(Type::AdditiveNoAlpha, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD)));

		// 乗算:色 = Src × Dest
		add(Type::Multiply, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_DEST_COLOR, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD)));

		// スクリーン:色 = Src + Dest×(1-Src)
		add(Type::Screen, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD)));

		// 減算:色 = Dest - Src×α
		add(Type::Subtractive, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_OP_REV_SUBTRACT,
			D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD)));

		// AlphaToCoverage:ブレンドは使わず、カバレッジだけ有効
		add(Type::AlphaToCoverage, MakeBlendDesc(true, noBlend));

		// 色を書かない:深度プリパスやステンシル書き込み用
		add(Type::ColorWriteOff, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			false,
			D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
			0)));

		// BlendFactorを使用
		add(Type::BlendFactorRGB, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD)));

		add(Type::BlendFactorRGBA, MakeBlendDesc(false, MakeRenderTargetBlendDesc(
			true,
			D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_OP_ADD,
			D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_OP_ADD)));
	}

	const Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendStateList::GetState(BlendType type)
	{
		// プリセットを検索
		auto it = m_presets.find(type);

		// なかった場合
		assert(it != m_presets.end());

		return it->second;
	}

	const Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendStateList::GetOrCreate(const D3D11_BLEND_DESC& desc)
	{
		// キャッシュを検索
		auto it = m_cache.find(desc);

		// 見つかれば返す
		if (it != m_cache.end()) return it->second;

		// なければ作成する
		Microsoft::WRL::ComPtr<ID3D11BlendState> pState{};
		HRESULT hr = m_pDevice->CreateBlendState(&desc, pState.GetAddressOf());

		if (FAILED(hr))
		{
			// エラー時用の静的変数
			static Microsoft::WRL::ComPtr<ID3D11BlendState> s_error{};

			assert(false && "Failed to create DepthStencilState.");
			return s_error;
		}

		// キャッシュに追加
		return m_cache.emplace(desc, pState).first->second;
	}

	D3D11_BLEND_DESC BlendStateList::MakeBlendDesc(bool alphaToCoverage, D3D11_RENDER_TARGET_BLEND_DESC rtDesc)
	{
		D3D11_BLEND_DESC desc{};

		desc.AlphaToCoverageEnable = alphaToCoverage;
		desc.IndependentBlendEnable = false;

		// 0版のみ使用
		desc.RenderTarget[0] = rtDesc;

		return desc;
	}

	D3D11_BLEND_DESC BlendStateList::MakeBlendDesc(bool alphaToCoverage, bool independentBlendEnable, std::initializer_list<D3D11_RENDER_TARGET_BLEND_DESC> rtDescs)
	{
		D3D11_BLEND_DESC desc{};

		desc.AlphaToCoverageEnable = alphaToCoverage;
		desc.IndependentBlendEnable = independentBlendEnable;

		// 最大8スロットまでコピー
		size_t i = 0;
		for (const auto& rt : rtDescs)
		{
			if (i >= 8) break;
			desc.RenderTarget[i++] = rt;
		}

		return desc;
	}

	D3D11_RENDER_TARGET_BLEND_DESC BlendStateList::MakeRenderTargetBlendDesc(bool enable, D3D11_BLEND src, D3D11_BLEND dest, D3D11_BLEND_OP op, D3D11_BLEND srcA, D3D11_BLEND destA, D3D11_BLEND_OP opA, UINT8 writeMask)
	{
		D3D11_RENDER_TARGET_BLEND_DESC desc{};

		desc.BlendEnable = enable;
		desc.SrcBlend = src;
		desc.DestBlend = dest;
		desc.BlendOp = op;
		desc.SrcBlendAlpha = srcA;
		desc.DestBlendAlpha = destA;
		desc.BlendOpAlpha = opA;
		desc.RenderTargetWriteMask = writeMask;

		return desc;
	}
}

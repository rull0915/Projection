/**
 * @file    DepthStencilStateList.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/02
 */

#include "pch.h"
#include "System/Render/DepthStencilStateList.h"

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
		inline bool Equal(const D3D11_DEPTH_STENCILOP_DESC& a, const D3D11_DEPTH_STENCILOP_DESC& b)
		{
			return a.StencilFailOp == b.StencilFailOp
				&& a.StencilDepthFailOp == b.StencilDepthFailOp
				&& a.StencilPassOp == b.StencilPassOp
				&& a.StencilFunc == b.StencilFunc;
		}

		/**
		 * @brief D3D11_DEPTH_STENCILOP_DESCをハッシュ化する
		 */
		inline void Hash(size_t& seed, const D3D11_DEPTH_STENCILOP_DESC& o)
		{
			HashCombine(seed, static_cast<size_t>(o.StencilFailOp));
			HashCombine(seed, static_cast<size_t>(o.StencilDepthFailOp));
			HashCombine(seed, static_cast<size_t>(o.StencilPassOp));
			HashCombine(seed, static_cast<size_t>(o.StencilFunc));
		}
	}

	size_t DepthStencilStateList::DescHash::operator()(const D3D11_DEPTH_STENCIL_DESC& d) const
	{
		size_t seed = 0;

		// 全変数のハッシュを結合
		HashCombine(seed, static_cast<size_t>(d.DepthEnable));
		HashCombine(seed, static_cast<size_t>(d.DepthWriteMask));
		HashCombine(seed, static_cast<size_t>(d.DepthFunc));
		HashCombine(seed, static_cast<size_t>(d.StencilEnable));
		HashCombine(seed, static_cast<size_t>(d.StencilReadMask));
		HashCombine(seed, static_cast<size_t>(d.StencilWriteMask));
		Hash(seed, d.BackFace);
		Hash(seed, d.FrontFace);

		return seed;
	}

	bool DepthStencilStateList::DescEqual::operator()(const D3D11_DEPTH_STENCIL_DESC& a, const D3D11_DEPTH_STENCIL_DESC& b) const
	{
		return
			// 全変数の一致をチェック
			a.DepthEnable == b.DepthEnable &&
			a.DepthWriteMask == b.DepthWriteMask &&
			a.DepthFunc == b.DepthFunc &&
			a.StencilEnable == b.StencilEnable &&
			a.StencilReadMask == b.StencilReadMask &&
			a.StencilWriteMask == b.StencilWriteMask &&
			Equal(a.BackFace, b.BackFace) &&
			Equal(a.FrontFace, b.FrontFace);
	}

	void DepthStencilStateList::Initialize(ID3D11Device* pDevice)
	{
		// デバイスを保持
		m_pDevice = pDevice;

		using Type = DepthStencilType;

		// プリセットステートを作成する

		m_presets[Type::Default] = GetOrCreate(MakeDepthDesc(TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS));
		m_presets[Type::DepthNone] = GetOrCreate(MakeDepthDesc(FALSE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS));
		m_presets[Type::DepthRead] = GetOrCreate(MakeDepthDesc(TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS));
		m_presets[Type::DepthGreater] = GetOrCreate(MakeDepthDesc(TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_GREATER));
		m_presets[Type::SkyBox] = GetOrCreate(MakeDepthDesc(TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS_EQUAL));

		m_presets[Type::StencilWrite] = GetOrCreate(MakeStencilDesc(D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_REPLACE));
		m_presets[Type::StencilEqual] = GetOrCreate(MakeStencilDesc(D3D11_COMPARISON_EQUAL, D3D11_STENCIL_OP_KEEP));
		m_presets[Type::StencilNotEqual] = GetOrCreate(MakeStencilDesc(D3D11_COMPARISON_NOT_EQUAL, D3D11_STENCIL_OP_KEEP));
	}

	const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& DepthStencilStateList::GetState(DepthStencilType type)
	{
		// プリセットを検索
		auto it = m_presets.find(type);

		// なかった場合
		assert(it != m_presets.end());

		return it->second;
	}

	const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& DepthStencilStateList::GetOrCreate(const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		// キャッシュを検索
		auto it = m_cache.find(desc);

		// 見つかれば返す
		if (it != m_cache.end()) return it->second;

		// なければ作成する
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pState{};
		HRESULT hr = m_pDevice->CreateDepthStencilState(&desc, pState.GetAddressOf());

		if (FAILED(hr))
		{
			// エラー時用の静的変数
			static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> s_error{};

			assert(false && "Failed to create DepthStencilState.");
			return s_error;
		}

		// キャッシュに追加
		return m_cache.emplace(desc, pState).first->second;
	}

	D3D11_DEPTH_STENCIL_DESC DepthStencilStateList::MakeDepthDesc(BOOL enable, D3D11_DEPTH_WRITE_MASK write, D3D11_COMPARISON_FUNC func)
	{
		D3D11_DEPTH_STENCIL_DESC desc{};

		desc.DepthEnable = enable;
		desc.DepthWriteMask = write;
		desc.DepthFunc = func;

		return desc;
	}

	D3D11_DEPTH_STENCIL_DESC DepthStencilStateList::MakeStencilDesc(D3D11_COMPARISON_FUNC stencilFunc, D3D11_STENCIL_OP passOp)
	{
		// デフォルトの深度ステートを扱う
		D3D11_DEPTH_STENCIL_DESC d = MakeDepthDesc(TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS);

		d.StencilEnable = TRUE;
		d.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		d.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		D3D11_DEPTH_STENCILOP_DESC op = {};
		op.StencilFunc = stencilFunc;
		op.StencilPassOp = passOp;
		op.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		op.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		d.FrontFace = op;
		d.BackFace = op;

		return d;
	}
}

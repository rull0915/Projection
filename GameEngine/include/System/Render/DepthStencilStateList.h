/**
 * @file    DepthStencilStateList.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/02
 *
 * @brief   デプスステンシルステートの所有者
 */

#pragma once

#include <unordered_map>
#include <wrl/client.h>
#include <d3d11.h>

namespace RHEngine
{
	enum DepthStencilType
	{
		Default,		///< 深度テストON / 書き込みON / LESS(不透明用)
		DepthNone,		///< 深度テストOFF / 書き込みOFF(UI・ポストエフェクト用)
		DepthRead,		///< 深度テストON / 書き込みOFF(半透明用)
		SkyBox,			///< LESS_EQUAL / 書き込みOFF
		StencilWrite,	///< ステンシルに Ref を書き込む(ALWAYS + REPLACE)
		StencilEqual,	///< ステンシル == Ref の場所だけ描く
		StencilNotEqual,///< ステンシル != Ref の場所だけ描く(アウトライン用)
	};

	/**
	 * @brief 深度ステンシルステートを保持する
	 */
	class DepthStencilStateList
	{
	private:
		/** @brief Desc のハッシュ関数 */
		struct DescHash
		{
			size_t operator()(const D3D11_DEPTH_STENCIL_DESC& d) const;
		};

		/** @brief Desc の等値比較 */
		struct DescEqual
		{
			bool operator()(const D3D11_DEPTH_STENCIL_DESC& a,
			                const D3D11_DEPTH_STENCIL_DESC& b) const;
		};

	private:

		/** @brief DepthStencilStateのリスト */
		std::unordered_map<DepthStencilType, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_presets;

		/** @brief Desc をキーにした汎用キャッシュ */
		std::unordered_map<D3D11_DEPTH_STENCIL_DESC, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, DescHash, DescEqual> m_cache;

		/** @brief デバイス */
		ID3D11Device* m_pDevice = nullptr;

	public:

		/// @name ライフサイクル
		/// @}

		DepthStencilStateList() = default;

		~DepthStencilStateList() = default;

		/// @}

		/**
		 * @brief 各タイプに対応するデプスステンシルステートの作成をする
		 * @param pDevice デバイス
		 */
		void Initialize(ID3D11Device* pDevice);

		/**
		 * @brief タイプに対応する深度ステンシルステートを取得する
		 * @param type タイプ
		 * @return 深度ステンシルステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& GetState(DepthStencilType type);

		/**
		 * @brief 任意の深度ステンシルステートを取得する
		 * @param desc ステートの設定
		 * @return 深度ステンシルステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& GetOrCreate(const D3D11_DEPTH_STENCIL_DESC& desc);

	private:

		/**
		 * @brief 深度のみのDescを作るヘルパー
		 * @param enable 深度
		 * @param write 
		 * @param func 
		 * @return 
		 */
		static D3D11_DEPTH_STENCIL_DESC MakeDepthDesc(BOOL enable,
			D3D11_DEPTH_WRITE_MASK write,
			D3D11_COMPARISON_FUNC func);

		/**
		 * @brief ステンシルのみのDescを作るヘルパー
		 * @param stencilFunc 
		 * @param passOp 
		 * @return 
		 */
		static D3D11_DEPTH_STENCIL_DESC MakeStencilDesc(
			D3D11_COMPARISON_FUNC stencilFunc,
			D3D11_STENCIL_OP passOp
		);
	};
}

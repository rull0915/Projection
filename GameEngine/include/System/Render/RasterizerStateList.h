/**
 * @file    RasterizerStateList.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/02
 *
 * @brief   ラスタライザステートの所有者
 */

#pragma once

#include <unordered_map>
#include <wrl/client.h>
#include <d3d11.h>
#include <Windows.h>

namespace RHEngine
{
	enum class RasterizerType
	{
		Default,		///< ソリッド / 裏面カリング
		CullNone,		///< ソリッド / 両面描画
		CullFront,		///< ソリッド / 表面カリング
		Wireframe,		///< ワイヤーフレーム
		Scissor,		///< シザー矩形有効
		ShadowMap,		///< 深度バイアス付き(影用)

		Custom,
	};

	/**
	 * @brief ラスタライザステートを保持する
	 */
	class RasterizerStateList
	{
	private:
		/** @brief Desc のハッシュ関数 */
		struct DescHash
		{
			size_t operator()(const D3D11_RASTERIZER_DESC& d) const;
		};

		/** @brief Desc の等値比較 */
		struct DescEqual
		{
			bool operator()(const D3D11_RASTERIZER_DESC& a,
				const D3D11_RASTERIZER_DESC& b) const;
		};

		/** @brief DESCを作る際に使用する情報のまとめ */
		struct DescInfo
		{
			D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID;	///< 塗りつぶし方
			D3D11_CULL_MODE cullMode = D3D11_CULL_BACK;		///< カリング方法
			INT             depthBias = 0;					///< 固定の深度バイアス
			float           slopeScaledDepthBias = 0.0f;	///< 傾きに応じた深度バイアス
			float           depthBiasClamp = 0.0f;			///< 深度バイアスの上限
			bool            scissorEnable = false;			///< シザー矩形を有効にするか
			bool            frontCounterClockwise = true;	///< 反時計回りを表とするか
			bool            depthClipEnable = true;			///< 近・遠クリップを行うか
			bool            multisampleEnable = false;		///< 用のラスタライズを行うか
			bool            antialiasedLineEnable = false;	///< 線のアンチエイリアスを行うか
		};

	private:

		/** @brief RasterizerStateのリスト */
		std::unordered_map<RasterizerType, Microsoft::WRL::ComPtr<ID3D11RasterizerState>> m_presets;

		/** @brief Desc をキーにした汎用キャッシュ */
		std::unordered_map<D3D11_RASTERIZER_DESC, Microsoft::WRL::ComPtr<ID3D11RasterizerState>, DescHash, DescEqual> m_cache;

		/** @brief デバイス */
		ID3D11Device* m_pDevice = nullptr;

	public:

		/// @name ライフサイクル
		/// @}

		RasterizerStateList() = default;

		~RasterizerStateList() = default;

		/// @}

		/**
		 * @brief 各タイプに対応するデプスステンシルステートの作成をする
		 * @param pDevice デバイス
		 */
		void Initialize(ID3D11Device* pDevice);

		/**
		 * @brief タイプに対応するラスタライザステートを取得する
		 * @param type タイプ
		 * @return ラスタライザステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& GetState(RasterizerType type);

		/**
		 * @brief 任意のラスタライザステートを取得する
		 * @param desc ステートの設定
		 * @return ラスタライザステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11RasterizerState>& GetOrCreate(const D3D11_RASTERIZER_DESC& desc);

	private:

		/**
		 * @brief D3D11_RASTERIZER_DESCを作成するヘルパー
		 */
		static D3D11_RASTERIZER_DESC MakeRasterizerDesc(const DescInfo& info);
	};
}

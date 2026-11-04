/**
 * @file    BlendStateList.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/04
 *
 * @brief   ブレンドステートの所有者
 */

#pragma once

#include <unordered_map>
#include <wrl/client.h>
#include <d3d11.h>
#include <initializer_list>
#include <Windows.h>

namespace RHEngine
{
	enum class BlendType
	{
		Opaque,				///< 不透明(ブレンドなし)
		AlphaBlend,			///< 通常の半透明
		PremultipliedAlpha,	///< 乗算済みアルファ
		Additive,			///< 加算合成(α付き)
		AdditiveNoAlpha,	///< 加算合成(αなし)
		Multiply,			///< 乗算
		Screen,				///< スクリーン
		Subtractive,		///< 減算
		AlphaToCoverage,	///< MSAA 用アルファ抜き
		ColorWriteOff,		///< 色を書き込まない
		BlendFactorRGB,		///< RGBでBlendFactorを使用
		BlendFactorRGBA,	///< RGBA全てでBlendFactorを使用

		Custom,
	};

	/**
	 * @brief ブレンドステートを保持する
	 */
	class BlendStateList
	{
	private:
		/** @brief Desc のハッシュ関数 */
		struct DescHash
		{
			size_t operator()(const D3D11_BLEND_DESC& d) const;
		};

		/** @brief Desc の等値比較 */
		struct DescEqual
		{
			bool operator()(const D3D11_BLEND_DESC& a,
			                const D3D11_BLEND_DESC& b) const;
		};

	private:

		/** @brief BlendStateのリスト */
		std::unordered_map<BlendType, Microsoft::WRL::ComPtr<ID3D11BlendState>> m_presets;

		/** @brief Desc をキーにした汎用キャッシュ */
		std::unordered_map<D3D11_BLEND_DESC, Microsoft::WRL::ComPtr<ID3D11BlendState>, DescHash, DescEqual> m_cache;

		/** @brief デバイス */
		ID3D11Device* m_pDevice = nullptr;

	public:

		/// @name ライフサイクル
		/// @}

		BlendStateList() = default;

		~BlendStateList() = default;

		/// @}

		/**
		 * @brief 各タイプに対応するデプスステンシルステートの作成をする
		 * @param pDevice デバイス
		 */
		void Initialize(ID3D11Device* pDevice);

		/**
		 * @brief タイプに対応するブレンドステートを取得する
		 * @param type タイプ
		 * @return ブレンドステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11BlendState>& GetState(BlendType type);

		/**
		 * @brief 任意のブレンドステートを取得する
		 * @param desc ステートの設定
		 * @return ブレンドステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11BlendState>& GetOrCreate(const D3D11_BLEND_DESC& desc);

	private:

		/**
		 * @brief D3D11_BLEND_DESCを作成するヘルパー
		 * @param alphaToCoverage アルファ対カバレッジを使用するかどうか
		 * @param rtDesc 全レンダーターゲットで共通で使用する設定
		 * @return 
		 */
		static D3D11_BLEND_DESC MakeBlendDesc(
			bool alphaToCoverage, 
			D3D11_RENDER_TARGET_BLEND_DESC rtDesc
		);

		/**
		 * @brief D3D11_BLEND_DESCを作成するヘルパー (複数のブレンド設定使用版)
		 * @param alphaToCoverage アルファ対カバレッジを使用するかどうか
		 * @param independentBlendEnable 複数レンダーターゲット時に独立したブレントを有効化するか
		 * @param rtDescs レンダーターゲットの設定項目
		 * @return 
		 */
		static D3D11_BLEND_DESC MakeBlendDesc(
			bool alphaToCoverage, 
			bool independentBlendEnable,
			std::initializer_list<D3D11_RENDER_TARGET_BLEND_DESC> rtDescs
		);

		/**
		 * @brief  D3D11_RENDER_TARGET_BLEND_DESCを作成するヘルパー
		 * @param enable ブレンドを行うか
		 * @param src ピクセルに書き込まれる色に対して何を乗算するか
		 * @param dest ピクセルに存在する色に対して何を乗算するか
		 * @param op srcとdescの組み合わせ方法
		 * @param srcA ピクセルに書き込まれるAlphaに対して何を乗算するか
		 * @param destA ピクセルに存在するAlphaに対して何を乗算するか
		 * @param opA srcAとdestAの組み合わせ方法
		 * @param writeMask 書き込むチャンネル
		 * @return 作成したdesc
		 */
		static D3D11_RENDER_TARGET_BLEND_DESC MakeRenderTargetBlendDesc(
			bool enable,
			D3D11_BLEND src, D3D11_BLEND dest, D3D11_BLEND_OP op,
			D3D11_BLEND srcA, D3D11_BLEND destA, D3D11_BLEND_OP opA,
			UINT8 writeMask = D3D11_COLOR_WRITE_ENABLE_ALL
			);
	};
}

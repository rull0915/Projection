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
	enum DepthStencilStateType
	{

	};

	class DepthStencilStateList
	{
	private:

		/** @brief DepthStencilStateのリスト */
		std::unordered_map<DepthStencilStateType, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_depthStencilStateList;

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
		 * @brief デプスステンシルステートを取得する
		 * @param type 取得するデプスステンシルステートのタイプ
		 * @return デプスステンシルステート
		 */
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& GetDepthStencilState(DepthStencilStateType type) const;

	private:

		/**
		 * @brief サンプラーを作成する
		 * @param device デバイス
		 * @param type 作成するタイプ
		 * @param filter フィルター
		 * @param mode モード
		 */
		void CreateSampler(ID3D11Device* device, DepthStencilStateType type, 
			bool depthEnable, 
			D3D11_DEPTH_WRITE_MASK mask, 
			D3D11_COMPARISON_FUNC func
		);
	};
}

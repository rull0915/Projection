//====================================================//
// ファイル名   : GraphicsManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要 : 描画を管理するシングルトンクラス
//
// 更新履歴 :
// 2026/07/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "System/DeviceResources.h"
#include "InputLayoutCache.h"
#include "SamplerList.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class GraphicsManager
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// デバイスリソース
		std::unique_ptr<DX::DeviceResources> m_deviceResources;

		// インプットレイアウトキャッシュ
		InputLayoutCache m_inputLayoutCache;

		// サンプラーリスト
		SamplerList m_samplerList;

	private:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		GraphicsManager()
			: m_deviceResources{ std::make_unique<DX::DeviceResources>() }
			, m_inputLayoutCache{}
			, m_samplerList{}
		{
		};
		~GraphicsManager() = default;

	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// インスタンス
		static GraphicsManager& Instance()
		{
			static GraphicsManager instance;
			return instance;
		}

		// 初期化
		void Initialize()
		{
			// デバイス取得
			ID3D11Device* pDevice = m_deviceResources->GetD3DDevice();

			// サンプラーリストの初期化
			m_samplerList.Initialize(pDevice);
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// デバイスリソース
		DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }

		// インプットレイアウトキャッシュ
		InputLayoutCache& GetInputLayoutCache() { return m_inputLayoutCache; };

		// サンプラーリスト
		SamplerList& GetSamplerList() { return m_samplerList; };
	};
}

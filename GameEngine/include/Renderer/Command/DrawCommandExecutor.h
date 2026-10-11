//====================================================//
// ファイル名   : DrawCommandExecutor.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/18
//
// 概要 : 描画コマンドを実行するクラス
//
// 更新履歴 :
// 2026/07/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <VertexTypes.h>

#include "DrawCommandContainer.h"
#include "Renderer/RendererInfo.h"

namespace REngine
{
	class AssetManager;
	class SamplerList;

	struct alignas(16) PerObjectBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix wvp;
		DirectX::SimpleMath::Matrix worldInvTranspose;
	};

	//====================================================//
	// クラス宣言
	//====================================================//
	class DrawCommandExecutor
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		//----- 各描画システム -----//

		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;

		// ベーシックエフェクト
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

		// スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

		// インプットレイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		//----- 定数バッファ -----//

		// フレーム毎に更新のバッファ
		FrameInfo m_perFrameInfo;

		// オブジェクト毎に更新のバッファ


		//----- デバイス -----//

		// デバイス
		ID3D11Device* m_pDevice;

		// デバイスコンテキスト
		ID3D11DeviceContext* m_pContext;

		// コモンステート
		DirectX::CommonStates* m_pStates;

		//----- 固定のバッファ -----//

		// PerFrame
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perFrameConstantBuffer;

		// World行列
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_worldConstantBuffer;

		//----- その他 -----//

		// アセットマネージャー
		AssetManager& m_assetManager;

		// サンプラーリスト
		std::unique_ptr<SamplerList> m_samplerList;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		DrawCommandExecutor(AssetManager& assetManager);
		~DrawCommandExecutor();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize();

		// 描画実行
		void DrawCommandExecute(DrawCommandContainer& container, const FrameInfo& perFrame);

	private:

		// プリミティブコマンド
		void DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands);

		// スプライト フォント
		void DrawSpriteAndFontCommandExecute(const std::vector<DrawCommandContainer::DrawUICommand>& commands);

		// モデル
		void DrawModelCommandExecute(const std::vector<DrawModelCommand>& commands);

		// プリミティブの描画を開始する関数
		void PreparePrimitiveRendering();

		// フレーム毎のバッファをバインドする関数
		void BindPerFrameBuffer(const FrameInfo& perFrame);

		// World行列をバインドする関数
		void BindPerObjectBuffer(const DirectX::SimpleMath::Matrix& world);
	};
}	// namespace REngine

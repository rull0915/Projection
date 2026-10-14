//====================================================//
// ファイル名   : ShadowRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要 : 
//
// 更新履歴 :
// 2026/10/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <vector>

#include "Renderer/Command/DrawCommand.h"
#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Objects/Handle.h"

#include "ShadowMap.h"

namespace REngine
{
	// 前方宣言
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ShadowRenderer
	{
		// 定数バッファ用の構造体
		struct alignas(16) ShadowPassPerFrameCBuffer
		{
			DirectX::SimpleMath::Matrix VP;
		};

		// 定数バッファ用の構造体
		struct alignas(16) ShadowPassPerObjectCBuffer
		{
			DirectX::SimpleMath::Matrix World;
		};

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// モデル描画コマンド配列
		std::vector<DrawModelCommand> m_drawMeshCommands;

		// VSハンドル
		Handle<ShaderAsset> m_vsShaderHandle;

		// アセット解決クラス
		AssetManager& m_assetManager;

		// デバイス
		ID3D11Device* m_pDevice;

		// デバイスコンテキスト
		ID3D11DeviceContext* m_pContext;

		// コモンステート
		DirectX::CommonStates* m_pStates;

		// 使用中のShadowMap
		ShadowMap* m_currentShadowMap;

		// ----- Buffer ----- //
		
		// PerFrame
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perFrameConstantBuffer;

		// World行列
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_perObjectConstantBuffer;

		// DepthStencilState
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ShadowRenderer(AssetManager& assetManager);
		~ShadowRenderer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize();

		// シャドウマップへの描画を開始する関数
		void Begin(ShadowMap* shadowMap);

		// メッシュ描画コマンドを発行する関数
		void DrawMesh(Mesh* mesh, size_t subMeshIndex, DirectX::SimpleMath::Matrix world)
		{
			// コマンドの生成
			auto& command = m_drawMeshCommands.emplace_back();

			// コマンドの設定
			command.pMesh = mesh;
			command.subMeshIndex = subMeshIndex;
			command.world = world;
		}

		// シャドウマップへの描画を終了する関数
		void End();

		// 描画コマンドを実行する関数
		void ExecuteCommands(const DirectX::SimpleMath::Matrix& lightView, const DirectX::SimpleMath::Matrix& lightProj);

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// PerFrameバッファのバインド
		void BindPerFrame(const ShadowPassPerFrameCBuffer& buf);

		// PerObjectバッファのバインド
		void BindPerObject(const ShadowPassPerObjectCBuffer& buf);
	};
}

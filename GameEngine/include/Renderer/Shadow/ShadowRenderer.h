/**
 * @file    ShadowRenderer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 *
 * @brief   シャドウマップへ深度の書き込みを行う
 */

#pragma once

#include <vector>
#include <CommonStates.h>
#include <wrl/client.h>
#include <d3d11.h>

#include "Renderer/Command/DrawCommand.h"
#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Objects/Handle.h"
#include "ShadowMap.h"
#include "Renderer/RenderInfoManager.h"
#include "Assets/Types/Model/Mesh.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	class AssetManager;

	/**
	 * @brief シャドウマップへ深度の書き込みを行う
	 */
	class ShadowRenderer
	{
	private:

		/** @brief 使用可能フラグ */
		bool m_usable;

		/** @brief モデル描画コマンド配列 */
		std::vector<DrawMeshCommand> m_drawMeshCommands;

		/** @brief VSハンドル */
		Handle<ShaderAsset> m_vsShaderHandle;

		/** @brief アセット解決クラス */
		AssetManager& m_assetManager;

		/** @brief デバイス */
		ID3D11Device* m_pDevice;

		/** @brief デバイスコンテキスト */
		ID3D11DeviceContext* m_pContext;

		/** @brief コモンステート */
		DirectX::CommonStates* m_pStates;

		/** @brief 使用中のShadowMap */
		ShadowMap* m_currentShadowMap;

		/** @brief 描画情報マネージャー */
		RenderInfoManager m_infoManager;

		/** @brief DepthStencilState */
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetManager アセットマネージャー
		 */
		ShadowRenderer(AssetManager& assetManager);

		~ShadowRenderer() = default;

		/// @}

		/**
		 * @brief DSVの作成を行う
		 */
		void Initialize();

		/**
		 * @brief シャドウパスの描画を開始する
		 * @param shadowMap 描画対象のシャドウマップ
		 */
		void Begin(ShadowMap* shadowMap);

		/**
		 * @brief メッシュ描画コマンドを発行する
		 * @param mesh 描画するメッシュ
		 * @param subMeshIndex サブメッシュインデックス
		 * @param world ワールド行列
		 */
		void DrawMesh(Mesh* mesh, size_t subMeshIndex, DirectX::SimpleMath::Matrix world)
		{
			// コマンドの生成
			auto& command = m_drawMeshCommands.emplace_back();

			// コマンドの設定
			command.pMesh = mesh;
			command.subMeshIndex = subMeshIndex;
			command.world = world;
		}

		/**
		 * @brief シャドウパスの描画を終了する
		 */
		void End();

		/**
		 * @brief 描画コマンドを実行する
		 * @param data ライトをカメラとみなしたときのカメラデータ
		 */
		void ExecuteCommands(const SceneCameraData& data);
	};
}	// namespace RHEngine

/**
 * @file    DrawCommandExecutor.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/18
 *
 * @brief   描画コマンドを実行するクラス
 */

#pragma once

#include <memory>
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <VertexTypes.h>
#include <d3d11.h>

#include "DrawCommandContainer.h"
#include "Renderer/RenderInfoManager.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	class AssetManager;
	class SamplerList;

	/**
	 * @brief 描画コマンドを実行する
	 * @TODO プリミティブバッチから脱却して自作描画システムに適用させる
	 */
	class DrawCommandExecutor
	{
	private:

		/** @brief プリミティブバッチ */
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;

		/** @brief  スプライトバッチ */
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

		/** @brief  描画情報マネージャー */
		RenderInfoManager& m_infoManager;

		/** @brief デバイス */
		ID3D11Device* m_pDevice;

		/** @brief デバイスコンテキスト */
		ID3D11DeviceContext* m_pContext;

		/** @brief コモンステート */
		DirectX::CommonStates* m_pStates;

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

		/** @brief デフォルトマテリアル */
		Handle<MaterialAsset> m_defaultMaterial;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetManager アセットマネージャー
		 * @param infoManager 描画情報マネージャー
		 */
		DrawCommandExecutor(AssetManager& assetManager, RenderInfoManager& infoManager);

		~DrawCommandExecutor();

		/// @}

		/**
		 * @brief デバイスの取得、各描画システムの初期化を行う
		 */
		void Initialize();

		/**
		 * @brief 描画コマンドを実行する
		 * @param container 描画コマンドのコンテナ
		 */
		void DrawCommandExecute(DrawCommandContainer& container);

	private:

		/// @name コマンド実行
		/// @{

		/** @brief プリミティブコマンド */
		void DrawPrimitiveCommandExecute(const std::vector<DrawPrimitiveCommand>& commands);

		/** @brief スプライト フォント */
		void DrawSpriteAndFontCommandExecute(const std::vector<DrawCommandContainer::DrawUICommand>& commands);

		/** @brief メッシュ */
		void DrawMeshCommandExecute(const std::vector<DrawMeshCommand>& commands);

		/// @}
	};
}	// namespace RHEngine

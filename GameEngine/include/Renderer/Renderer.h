/**
 * @file    Renderer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/29
 *
 * @brief   描画を統括するクラス
 */

#pragma once

#include <cassert>

#include "GraphicsSystem.h"
#include "RenderProxy.h"
#include "RenderInfoManager.h"
#include "Scene/SceneInfo.h"

#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"

namespace RHEngine
{
	class AssetManager;

	/**
	 * @brief 描画を統括する
	 */
	class Renderer
	{
	private:

		/** @brief 描画仲介クラス */
		RenderProxy m_renderProxy;

		/** @brief 描画システム */
		GraphicsSystem m_graphicSystem;

		/** @brief 描画情報 */
		RenderInfoManager m_infoManager;

		/** @brief 初期化フラグ */
		bool m_isInitialized;

		/** @brief 使用中のカメラデータ */
		SceneCameraData m_currentCamera;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetManager アセットマネージャー
		 */
		Renderer(AssetManager& assetManager);

		~Renderer() = default;

		/// @}

		/**
		 * @brief 初期化関数
		 */
		void Initialize();

		/**
		 * @brief 描画を開始する
		 * @param data 描画シーンのデータ
		 */
		void Begin(const SceneData& data);

		/**
		 * @brief カメラを変更する
		 * @detials カメラを変更し、PerCamera定数バッファの再バインドをする
		 * @param data 変更後のカメラデータ
		 */
		void ChangeCamera(const SceneCameraData& data);

		/**
		 * @brief 描画仲介クラスを取得する
		 * @details renderer.Draw().Mesh() のように呼び出す
		 * @return 描画仲介クラス
		 */
		RenderProxy& Draw()
		{
			// 初期化チェック
			assert(m_isInitialized);

			// 仲介クラスを返す
			return m_renderProxy;
		}

		/**
		 * @brief 描画コマンドを実行する
		 */
		void ExecuteCommands();

		/**
		 * @brief 描画を終了する
		 */
		void End();

		/// @name ゲッター
		/// @{

		/** @brief ビュー行列 */
		const DirectX::SimpleMath::Matrix& GetView() const { return m_currentCamera.view; };

		/** @brief 射影行列 */
		const DirectX::SimpleMath::Matrix& GetProjection() const { return m_currentCamera.proj; };

		/// @}

		/// @name セッター
		/// @{

		/** @brief ワールド行列 */
		void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_graphicSystem.SetWorld(world); }

		/** @brief マテリアルハンドル */
		void SetMaterial(Handle<MaterialAsset> handle) { m_graphicSystem.SetMaterial(handle); };

		/// @}
	};
}	// namespace RHEngine

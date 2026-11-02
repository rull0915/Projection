/**
 * @file    CameraBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/16
 *
 * @brief   カメラをコンポーネント化したもの
 */

#pragma once

#include <SimpleMath.h>

#include "Components/World/WorldComponentBase.h"
#include "Components/Interface/IDebugRenderable.h"
#include "Components/Interface/IWindowSizeDependent.h"
#include "Physics/Ray.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class RenderTarget;

	/**
	 * @brief 投影モード
	 */
	enum class ProjectionType
	{
		Perspective,
		Orthographic,
	};

	/**
	 * @brief カメラの基底
	 */
	class CameraBase : public WorldComponentBase, public IDebugRenderable, public IWindowSizeDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

	private:

		/// @name 行列
		/// @{

		/** @brief ビュー行列 */
		DirectX::SimpleMath::Matrix m_view;

		/** @brief 射影行列 */
		DirectX::SimpleMath::Matrix m_proj;

		/** @brief ビュー逆行列 */
		DirectX::SimpleMath::Matrix m_inverseView;

		/** @brief 射影逆行列 */
		DirectX::SimpleMath::Matrix m_inverseProj;

		/// @}

		/** @brief 射影行列の更新が必要かどうか */
		bool m_needUpdateProj;

		/** @brief 描画時に使用するマテリアルアセットのハンドル */
		Handle<MaterialAsset> m_materialHandle;

		/** @brief 描画先 nullの場合はメインディスプレイ */
		RenderTarget* m_pRenderTarget;

		/** @brief 背景カラー */
		DirectX::SimpleMath::Color m_backColor;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		CameraBase(IComponentOwner* pOwner);

		virtual ~CameraBase() = default;

		/// @}

		/**
		 * @brief ビュー行列を更新する
		 */
		virtual void UpdateView() {};

		/**
		 * @brief 射影行列を更新する
		 */
		virtual void UpdateProj() {};

		/**
		 * @brief カメラから指定したスクリーン座標へのRayを作成する
		 * @param point ターゲットとなるスクリーン座標
		 * @return 作成したRay
		 */
		Ray GetRayToScreenPoint(const DirectX::SimpleMath::Vector2& point);

		/**
		 * @brief GUI上での値変更時
		 */
		void OnValidate() override
		{
			m_needUpdateProj = true;
		}

		/**
		 * @brief カメラデータを構築する
		 * @return 構築したデータ
		 */
		SceneCameraData CreateCameraData();
		
		/**
		 * @brief 試錐台のデバッグ描画
		 */
		void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color);

		/**
		 * @brief ウィンドウサイズの変更時処理
		 */
		virtual void OnWindowSizeChanged(int, int) override
		{
			m_needUpdateProj = true;
		}
		
		/// @name ゲッター

		/** @brief ビュー行列 */
		const DirectX::SimpleMath::Matrix& GetView()
		{
			UpdateView();
			return m_view;
		}

		/** @brief 射影行列 */
		const DirectX::SimpleMath::Matrix& GetProj()
		{
			if (m_needUpdateProj)
			{
				UpdateProj();
				m_needUpdateProj = false;
			}
			return m_proj;
		}

		/** @brief ビュー逆行列 */
		const DirectX::SimpleMath::Matrix& GetInverseView() const { return m_inverseView; }

		/** @brief 射影逆行列 */
		const DirectX::SimpleMath::Matrix& GetInverseProj() const { return m_inverseProj; }

		/** @brief マテリアルアセット */
		Handle<MaterialAsset> GetMaterial() const { return m_materialHandle; }

		/** @brief 描画先 */
		RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

		/** @brief 背景色 */
		DirectX::SimpleMath::Color GetBackColor() const { return m_backColor; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief マテリアルアセット */
		void SetMaterial(Handle<MaterialAsset> mat) { m_materialHandle = mat; }

		/** @brief 描画先 */
		void SetTarget(RenderTarget* pTarget) { m_pRenderTarget = pTarget; }

		/** @brief 背景色 */
		void SetBackColor(DirectX::SimpleMath::Color color) { m_backColor = color; }

		/// @}

	protected:

		/**
		 * @brief ビュー行列をセットする
		 */
		void SetView(const DirectX::SimpleMath::Matrix& view)
		{
			m_view = view;
			m_inverseView = m_view.Invert();
		}

		/**
		 * @brief 射影行列をセットする
		 */
		void SetProj(const DirectX::SimpleMath::Matrix& proj)
		{
			m_proj = proj;
			m_inverseProj = m_proj.Invert();
		}

		/**
		 * @brief 射影行列の更新が必要であることをセットする
		 */
		void SetNeedUpdateProj(bool f) { m_needUpdateProj = f; }
	};
} // namespace RHEngine

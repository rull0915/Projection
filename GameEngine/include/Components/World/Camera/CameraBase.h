//====================================================//
// ファイル名   : CameraBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/16
//
// 概要 : カメラをコンポーネント化したもの
//
// 更新履歴 :
// 2026/04/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "System/Render/RenderTarget.h"

#include "Physics/Ray.h"
#include "Assets/Objects/Handle.h"
#include "Assets/Types/Material/MaterialAsset.h"

#include "Scene/SceneInfo.h"
#include "Components/Interface/IDebugRenderable.h"

namespace RHEngine
{
	//====================================================//
	// 列挙型宣言
	//====================================================//

	enum class ProjectionType
	{
		Perspective,
		Orthographic,
	};

	//====================================================//
	// クラス宣言
	//====================================================//
	class CameraBase : public WorldComponentBase, public IDebugRenderable
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// --- 各種行列 --- //
		DirectX::SimpleMath::Matrix m_view; // ビュー
		DirectX::SimpleMath::Matrix m_proj; // 射影
		DirectX::SimpleMath::Matrix m_inverseView;  // ビュー逆行列
		DirectX::SimpleMath::Matrix m_inverseProj;  // 射影逆行列

		bool m_needUpdateProj;

		// マテリアル
		Handle<MaterialAsset> m_materialHandle;

		// 描画対象
		RenderTarget* m_pRenderTarget;

		// 背景カラー
		DirectX::SimpleMath::Color m_backColor;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CameraBase(IComponentOwner* own)
			: WorldComponentBase(own)
			, m_view{}
			, m_proj{}
			, m_inverseView{}
			, m_inverseProj{}
			, m_needUpdateProj{ true }
			, m_pRenderTarget{ nullptr }
			, m_backColor{}
		{
			ADD_PROPERTY(CameraBase, m_materialHandle);
			ADD_PROPERTY(CameraBase, m_backColor);
		}

		virtual ~CameraBase() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(CameraBase, WorldComponentBase)

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 各行列を更新する関数
		virtual void UpdateView() {};
		virtual void UpdateProj() {};

		// カメラから指定したスクリーン座標へのRayを作る関数
		Ray GetRayToScreenPoint(const DirectX::SimpleMath::Vector2& point);

		// 値変更時
		void OnValidate() override
		{
			m_needUpdateProj = true;
		}

		// カメラデータを構築する関数
		SceneCameraData CreateCameraData();
		
		// デバッグ表示
		void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color);
		
		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Matrix& GetView()
		{
			UpdateView();
			return m_view;
		}
		const DirectX::SimpleMath::Matrix& GetProj()
		{
			if (m_needUpdateProj)
			{
				UpdateProj();
				m_needUpdateProj = false;
			}
			return m_proj;
		}
		const DirectX::SimpleMath::Matrix& GetInverseView() const { return m_inverseView; }

		const DirectX::SimpleMath::Matrix& GetInverseProj() const { return m_inverseProj; }

		Handle<MaterialAsset> GetMaterial() const { return m_materialHandle; }

		RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

		DirectX::SimpleMath::Color GetBackColor() const { return m_backColor; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		// マテリアルのセット
		void SetMaterial(Handle<MaterialAsset> mat)
		{
			m_materialHandle = mat;
		}

		// 描画対象のセット
		void SetTarget(RenderTarget* target)
		{
			m_pRenderTarget = target;
		}

		// 背景色のセット
		void SetBackColor(DirectX::SimpleMath::Color color)
		{
			m_backColor = color;
		}

		// 各行列を設定する関数
	protected:

		// ビュー行列を設定する関数
		void SetView(const DirectX::SimpleMath::Matrix& view)
		{
			m_view = view;
			m_inverseView = m_view.Invert();
		}
		// 射影行列を設定する関数
		void SetProj(const DirectX::SimpleMath::Matrix& proj)
		{
			m_proj = proj;
			m_inverseProj = m_proj.Invert();
		}
		void SetNeedUpdateProj(bool f) { m_needUpdateProj = f; }
	};
} // namespace RHEngine

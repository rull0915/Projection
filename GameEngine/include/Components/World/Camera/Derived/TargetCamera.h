//====================================================//
// ファイル名   : TargetCamera.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/20
//
// 概要 : 
//
// 更新履歴 :
// 2026/05/20 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../CameraBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class TargetCamera : public CameraBase
	{
	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------


		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// projection行列のパラメータ

		float m_nearZ;  // ニアクリップ
		float m_farZ;   // ファークリップ
		float m_aspect; // アスペクト比
		float m_fov;    // 画角

		// ターゲット
		IComponentOwner* m_target;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		TargetCamera
		(
			IComponentOwner* own
		);
		~TargetCamera() {};

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void UpdateView() override;
		void UpdateProj() override;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<TargetCamera>();
		}

		float GetAspect() const
		{
			return m_aspect;
		}
		float GetFov() const
		{
			return m_fov;
		}
		float GetNearClip() const
		{
			return m_nearZ;
		}
		float GetFarClip() const
		{
			return m_farZ;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetAspect(float r)
		{
			m_aspect = r;
			SetNeedUpdateProj(true);
		}
		void SetFov(float f)
		{
			m_fov = f;
			SetNeedUpdateProj(true);
		}
		void SetNearClip(float n)
		{
			m_nearZ = n;
			SetNeedUpdateProj(true);
		}
		void SetFarClip(float f)
		{
			m_farZ = f;
			SetNeedUpdateProj(true);
		}
		void SetTarget(IComponentOwner* target) { m_target = target; }

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	};
} // namespace REngine

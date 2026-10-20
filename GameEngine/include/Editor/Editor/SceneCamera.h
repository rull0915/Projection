//====================================================//
// ファイル名   : SceneCamera.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : シーンビューのカメラコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

#ifdef ENGINE_GUI

//====================================================//
// インクルードファイル
//====================================================//

#include "Scene/SceneInfo.h"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class SceneCamera
	{
	private:

		// 動くかどうか
		bool m_movable;

		// projection行列のパラメータ

		float m_nearZ;  // ニアクリップ
		float m_farZ;   // ファークリップ
		float m_aspect; // アスペクト比

		float m_fov;    // 画角

		// 注視点の位置
		DirectX::SimpleMath::Vector3 m_targetPoint;

		// 自分の位置
		DirectX::SimpleMath::Vector3 m_position;

		// 角度
		DirectX::SimpleMath::Quaternion m_rotation;

		// 行列
		DirectX::SimpleMath::Matrix m_view;
		DirectX::SimpleMath::Matrix m_proj;

		// 半径
		float m_radius;

		// 回転
		float m_yaw = 0;
		float m_pitch = 0;

		// マウスの移動量に対するカメラ移動の割合
		float m_moveRatio;

		// マウスの移動量に対するカメラの回転の割合
		float m_rotateRatio;

		// ホイールの移動量に対する拡大の割合
		float m_scaleRatio;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SceneCamera();
		~SceneCamera() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Start();

		void Update(const GameTimer& gameTimer);

		void UpdateView();
		void UpdateProj();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		const DirectX::SimpleMath::Matrix GetView() const { return m_view; }
		const DirectX::SimpleMath::Matrix GetProj() const { return m_proj; }

		const SceneCameraData GetData() const
		{
			SceneCameraData data;

			data.view = m_view;
			data.proj = m_proj;
			data.pos = m_position;

			return data;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetMovable(bool f) { m_movable = f; }
	};
} // namespace RHEngine

#endif // ENGINE_GUI

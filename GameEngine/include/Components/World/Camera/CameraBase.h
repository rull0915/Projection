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

#include "Physics/Ray.h"

//====================================================//
// 前方宣言
//====================================================//

enum class ProjectionType
{
    Perspective,
    Orthographic,
};

//====================================================//
// クラス宣言
//====================================================//
class CameraBase : public WorldComponentBase
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
    {
    }

    virtual ~CameraBase() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // カテゴリをカメラに指定
    ComponentCategory GetCategory() const override { return ComponentCategory::Camera; }

    const DirectX::SimpleMath::Matrix& GetView() { return m_view; }
    const DirectX::SimpleMath::Matrix& GetProj() { return m_proj; }
    const DirectX::SimpleMath::Matrix& GetInverseView() { return m_inverseView; }
    const DirectX::SimpleMath::Matrix& GetInverseProj() { return m_inverseProj; }

    bool IsNeedUpdateProj() const { return m_needUpdateProj; }

    // 各行列を更新する関数
    virtual void UpdateView() {};
    virtual void UpdateProj() {};

    // カメラから指定したスクリーン座標へのRayを作る関数
    Ray GetRayToScreenPoint(const DirectX::SimpleMath::Vector2& point);

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
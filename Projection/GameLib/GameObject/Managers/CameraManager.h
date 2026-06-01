//====================================================//
// ファイル名   : CameraManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : カメラ管理クラス
//
// 更新履歴 :
// 2026/04/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../Components/Camera/BaseCamera.h"
#include "GameLib/GameObject/GameObject.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class CameraManager
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // メインカメラがないときに使用するカメラ
    GameObject m_spareCamera;

    // 使用しているメインカメラ
    BaseCamera* m_mainCamera;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
public:
    CameraManager();
    ~CameraManager();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Update();

    // メインカメラを設定する関数
    void SetMainCamera(BaseCamera* camera)
    {
        m_mainCamera = camera;
    }
    void UnSetMainCamera(BaseCamera* camera)
    {
        if (camera == m_mainCamera) m_mainCamera = nullptr;
    }

    // メインカメラを取得する関数
    BaseCamera* GetMainCamera()
    {
        return (!m_mainCamera || !m_mainCamera->IsActive()) ? m_spareCamera.GetComponent<BaseCamera>() : m_mainCamera;
    }

    // メインカメラの行列の取得関数
    const DirectX::SimpleMath::Matrix& GetView() { return GetMainCamera()->GetView(); }
    const DirectX::SimpleMath::Matrix& GetProj() { return GetMainCamera()->GetProj(); }
    const DirectX::SimpleMath::Matrix& GetInverseView() { return GetMainCamera()->GetInverseView(); }
    const DirectX::SimpleMath::Matrix& GetInverseProj() { return GetMainCamera()->GetInverseProj(); }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

};
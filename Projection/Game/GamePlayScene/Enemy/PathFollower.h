//====================================================//
// ファイル名   : PathFollower.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/23
//
// 概要 : 敵が持つパスを管理するクラス
//
// 更新履歴 :
// 2026/06/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//


//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class PathFollower
{
public:
    //-----------------------------------------------------
    // 構造体
    //-----------------------------------------------------
    struct Path
    {
        DirectX::SimpleMath::Vector3 start;     // 始点
        DirectX::SimpleMath::Vector3 goal;      // 終点

        float time; // 移動にかかる時間
    };
    struct Path2D
    {
        DirectX::SimpleMath::Vector2 start;     // 始点
        DirectX::SimpleMath::Vector2 goal;      // 終点

        float time; // 移動にかかる時間
    };

private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 経路
    std::vector<Path> m_way;
    std::vector<Path> m_nextWay;    // 変更予定の経路

    // 2D経路
    std::vector<Path2D> m_way2D;
    std::vector<Path2D> m_nextWay2D;    // 変更予定の経路

    // 辿っている道のインデックス
    int m_nowIndex;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    PathFollower()
        : m_way{}
        , m_nextWay{}
        , m_way2D{}
        , m_nextWay2D{}
        , m_nowIndex{ -1 }
    {}
    ~PathFollower() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

    // 現在通っているパスのポインタを返す関数
    const Path* GetNowPath() const
    {
        // 今のインデックスが範囲外ならnullptr
        if (m_nowIndex < 0 || m_nowIndex >= m_way.size()) return nullptr;

        // 指定したインデックスの要素を返す
        return &(m_way[m_nowIndex]);
    }
    const Path2D* GetNowPath2D() const
    {
        // 今のインデックスが範囲外ならnullptr
        if (m_nowIndex < 0 || m_nowIndex >= m_way2D.size()) return nullptr;

        // 指定したインデックスの要素を返す
        return &(m_way2D[m_nowIndex]);
    }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------
    
    // 3Dの道
    void SetWay(const std::vector<Path>& way) 
    {
        m_nextWay = way; 

        // 2Dの情報をリセット
        m_way2D.clear();
        m_nextWay2D.clear();
    }
    // 2Dの道
    void SetWay(const std::vector<Path2D>& way) 
    {
        m_nextWay2D = way; 

        // 3Dの情報をリセット
        m_way.clear();
        m_nextWay.clear();
    }

    // 道の更新が必要かどうか
    bool NeedUpdateWay() const
    {
        return m_nextWay.size() > 0 || m_nextWay2D.size() > 0;
    }

    void ToNextPath()
    {
        // 道の更新があれば
        if (NeedUpdateWay())
        {
            m_way = m_nextWay;
            m_way2D = m_nextWay2D;

            m_nowIndex = -1;

            m_nextWay.clear();
            m_nextWay2D.clear();
        }

        // インデックスを一つ進める
        m_nowIndex++;
    }
};

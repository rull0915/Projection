//====================================================//
// ファイル名   : ObjectManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/07
//
// 概要 :
//
// 更新履歴 :
// 2026/04/07 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../GameObject.h"
#include <vector>
#include "GameLib/Common/Renderer/Renderer.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ObjectManager
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 生成予約中のオブジェクトリスト
    std::vector<GameObject*> m_reservations;

    // 管理しているオブジェクトリスト
    std::vector<std::unique_ptr<GameObject>> m_objects;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
public:
    ObjectManager();
    ~ObjectManager();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------
    
    // 更新関数
    void Update(const GameTimer& gameTimer);
    void LateUpdate(const GameTimer& gameTimer);

    void RemoveDeadComponent();

    // 描画関数
    void Render(Renderer& renderer);

    // 終了関数
    void Finalize();

    // キャッシュを適用する関数
    void AllReflectCache();

    // オブジェクト追加関数
    template<typename T>
    void AddObject(T* object);

    // オブジェクト削除関数
    template<typename T>
    void Destroy(T* object);

    void AllDestroy();

    // 死亡オブジェクトを削除する関数
    void RemoveDeadObject();

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------


    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

    // 予約されているオブジェクトを全てリストへ追加する関数
    void AddReservedObject();
};

template<typename T>
inline void ObjectManager::AddObject(T* object)
{
    // GameObject派生クラスであれば
    if constexpr (std::is_base_of_v<GameObject, T>)
    {
        // 予約リストに追加
        m_reservations.push_back(object);
    }
}

template<typename T>
inline void ObjectManager::Destroy(T* object)
{
    // GameObject派生クラスであれば
    if constexpr (std::is_base_of_v<GameObject, T>)
    {
        // 削除フラグを立てる
        object->Destroy();
    }
}

//====================================================//
// ファイル名   : ObjectSaver.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : コンポーネントをjsonファイルに保存するクラス
//
// 更新履歴 :
// 2026/06/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>
#include <functional>

#include "Components/ComponentBase.h"

class GameObject;

namespace nlohmann
{
    class json;
}

//====================================================//
// クラス宣言
//====================================================//
class ObjectSaver
{
    // 別名宣言
    std::function<nlohmann::json(ComponentBase* component)> m_a;

private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 関数マップ
    inline static std::unordered_map<unsigned int, Saver> m_funcMap;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ObjectSaver() = default;
    ~ObjectSaver() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 登録関数
    static void Register(unsigned int id, Saver saver);

    // 実行関数
    static nlohmann::json Save(ComponentBase* component);

    // 保存関数
    static nlohmann::json SaveObject(GameObject* obj);

    // 保存関数
    static void SaveToFile(const std::wstring& filePath, GameObject* obj);
};

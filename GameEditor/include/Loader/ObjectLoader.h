//====================================================//
// ファイル名   : ObjectLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/29
//
// 概要 : コンポーネントをjsonファイルから読みとるクラス
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

#include <nlohmann/json.hpp>
#include "Components/ComponentBase.h"

class GameObject;

//====================================================//
// クラス宣言
//====================================================//
class ObjectLoader
{
    // 別名宣言
    using Loader = std::function<void(const nlohmann::json&, ComponentBase* component)>;

private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 関数マップ
    inline static std::unordered_map<unsigned int, Loader> m_funcMap;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ObjectLoader() = default;
    ~ObjectLoader() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 登録関数
    static void Register(unsigned int id, Loader loader);

    // 実行関数
    static void Load(const nlohmann::json& js, ComponentBase* component);

    // 保存関数
    static void LoadObject(const nlohmann::json& json, GameObject* obj);

    // 保存関数
    static void LoadFromFile(const std::wstring& filePath, GameObject* obj);
};

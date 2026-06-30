//====================================================//
// ファイル名   : ComponentFactory.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/30
//
// 概要 : コンポーネントの作成を担うクラス
//
// 更新履歴 :
// 2026/06/30 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <unordered_map>
#include <string>

#include "Components/ComponentBase.h"

class GameObject;

//====================================================//
// クラス宣言
//====================================================//
class ComponentFactory
{
private:
    // 別名宣言
    using CreateFunc = std::function<ComponentBase*(GameObject*)>;

    // 関数マップ
    static inline std::unordered_map<std::string, CreateFunc> m_creatorMap;

public:

    // 登録関数
    static void Register(const std::string& name, CreateFunc func);

    // 作成を実行
    static ComponentBase* Create(const std::string& id, GameObject* owner);
};
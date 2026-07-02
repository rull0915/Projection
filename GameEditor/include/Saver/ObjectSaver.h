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

#include <nlohmann/json.hpp>
#include "Components/ComponentBase.h"

class GameObject;

//====================================================//
// クラス宣言
//====================================================//
class ObjectSaver
{
public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ObjectSaver() = default;
    ~ObjectSaver() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

	// 保存関数
    static nlohmann::json SaveProperty(const PropertyObject& obj);

    // 保存関数
    static nlohmann::json SaveObject(const GameObject* obj);

    // 保存関数
    static void SaveToFile(const std::wstring& filePath, GameObject* obj);
};

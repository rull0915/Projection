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
#include <nlohmann/json.hpp>
#include "Common/PropatyObject.h"

class GameObject;

//====================================================//
// クラス宣言
//====================================================//
class ObjectLoader
{
public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ObjectLoader() = default;
    ~ObjectLoader() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 保存関数
    static void LoadPropaty(const nlohmann::json& json, PropatyObject& obj);

	// 保存関数
    static void LoadObject(const nlohmann::json& json, GameObject* obj);

    // 保存関数
    static void LoadFromFile(const std::wstring& filePath, GameObject* obj);
};

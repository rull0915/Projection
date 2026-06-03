//====================================================//
// ファイル名  : GameObject.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/01
//
// 概要        : ゲームオブジェクト基底クラスです
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameObject.h"

#include "GameLib/Input/KeyInput.h"

struct HitContact;

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
GameObject::GameObject()
    : m_isActive{ true }
    , m_isDead{ false }
    , m_pScene{ nullptr }
    , m_components{ this }
{
}

void GameObject::BaseUpdate(float elapsedTime)
{
    m_components.UpdateComponets(elapsedTime);
}

void GameObject::BaseLateUpdate(float elapsedTime)
{
    m_components.LateUpdateComponents(elapsedTime);
}

void GameObject::BaseRender(Renderer& renderer)
{
    // F3キーでコライダーのデバッグ描画
//    if(KeyInput::GetKey(DirectX::Keyboard::Keys::F3)) 
        CollidersDebugDraw(renderer, 0x00FFFF, true);
}

void GameObject::BaseFinalize()
{
    // コンポーネント削除
    RemoveComponents();

    GetComponentContainer().RemoveRegistered();
}

/// <summary>
/// コライダーのデバッグ描画をする関数
/// </summary>
void GameObject::CollidersDebugDraw(Renderer& renderer, int color, bool drawBoundingBox)
{
    static std::vector<BaseCollider*> colliders;
    static std::vector<BaseCollider2D*> colliders2D;

    GetComponents<BaseCollider>(colliders);
    GetComponents<BaseCollider2D>(colliders2D);

    for (auto& col : colliders)
    {
        if (!col->IsActive()) continue;

        col->DebugDraw(renderer, color);

        if (drawBoundingBox) col->DebugDrawAABB(renderer, color);
    }

    for (auto& col : colliders2D)
    {
        if (!col->IsActive()) continue;

        col->DebugDraw(renderer, color);

        if (drawBoundingBox) col->DebugDrawAABB(renderer, color);
    }
}

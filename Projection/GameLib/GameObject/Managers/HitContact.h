//====================================================//
// ファイル名   : HitContact.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/24
//
// 概要 : 衝突情報をまとめた構造体
//
// 更新履歴 :
// 2026/04/24 新規作成
//====================================================//

#pragma once

class IComponentOwner;
class BaseCollider;
class BaseCollider2D;
class RigidBody;
class RigidBody2D;

//====================================================//
// 構造体宣言
//====================================================//

struct ObjectPair
{
    BaseCollider* a;
    BaseCollider* b;

public:
    ObjectPair(BaseCollider* pA, BaseCollider* pB)
    {
        a = pA > pB ? pA : pB;
        b = pA < pB ? pA : pB;
    }

    bool operator==(const ObjectPair& other) const
    {
        return (a == other.a && b == other.b) ||
            (a == other.b && b == other.a);
    }
};

struct ObjectPairHash
{
    size_t operator()(const ObjectPair& p) const
    {
        size_t h1 = std::hash<BaseCollider*>()(p.a);
        size_t h2 = std::hash<BaseCollider*>()(p.b);
        return h1 ^ (h2 << 1);
    }
};

struct ObjectPair2D
{
    BaseCollider2D* a;
    BaseCollider2D* b;

public:
    ObjectPair2D(BaseCollider2D* pA, BaseCollider2D* pB)
    {
        a = pA > pB ? pA : pB;
        b = pA < pB ? pA : pB;
    }

    bool operator==(const ObjectPair2D& other) const
    {
        return (a == other.a && b == other.b) ||
            (a == other.b && b == other.a);
    }
};

struct ObjectPairHash2D
{
    size_t operator()(const ObjectPair2D& p) const
    {
        size_t h1 = std::hash<BaseCollider2D*>()(p.a);
        size_t h2 = std::hash<BaseCollider2D*>()(p.b);
        return h1 ^ (h2 << 1);
    }
};

struct HitContact
{
    // 衝突した2つのオブジェクト
    IComponentOwner* own = nullptr;
    IComponentOwner* other = nullptr;

    // 衝突したリジッドボディ
    RigidBody* ownRigid = nullptr;
    RigidBody* otherRigid = nullptr;

    // 衝突したコライダー
    BaseCollider* ownCol = nullptr;
    BaseCollider* otherCol = nullptr;

    // トリガーフラグ
    bool isTrigger = false;

    // スタティックかどうか
    bool ownIsStatic = false;
    bool otherIsStatic = false;

    // 衝突法線
    DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;

    // めり込み量
    float penetration = 0.0f;

    // 衝突点(未使用)
    DirectX::SimpleMath::Vector3 point = DirectX::SimpleMath::Vector3::Zero;

    HitContact Inverse() const
    {
        HitContact hit;

        hit.own = other;
        hit.other = own;

        hit.ownRigid = otherRigid;
        hit.otherRigid = ownRigid;

        hit.ownCol = otherCol;
        hit.otherCol = ownCol;

        hit.isTrigger = isTrigger;

        hit.ownIsStatic = otherIsStatic;
        hit.otherIsStatic = ownIsStatic;

        hit.normal = -normal;
        hit.penetration = penetration;
        hit.point = point;

        return hit;
    }
};

struct HitContact2D
{
    // 衝突した2つのオブジェクト
    IComponentOwner* own = nullptr;
    IComponentOwner* other = nullptr;

    // 衝突したリジッドボディ
    RigidBody2D* ownRigid = nullptr;
    RigidBody2D* otherRigid = nullptr;

    // 衝突したコライダー
    BaseCollider2D* ownCol = nullptr;
    BaseCollider2D* otherCol = nullptr;

    // トリガーフラグ
    bool isTrigger = false;

    // スタティックかどうか
    bool ownIsStatic = false;
    bool otherIsStatic = false;

    // 衝突法線
    DirectX::SimpleMath::Vector2 normal = DirectX::SimpleMath::Vector2::Zero;

    // めり込み量
    float penetration = 0.0f;

    // 衝突点(未使用)
    DirectX::SimpleMath::Vector2 point = DirectX::SimpleMath::Vector2::Zero;

    HitContact2D Inverse() const
    {
        HitContact2D hit;

        hit.own = other;
        hit.other = own;

        hit.ownRigid = otherRigid;
        hit.otherRigid = ownRigid;

        hit.ownCol = otherCol;
        hit.otherCol = ownCol;

        hit.isTrigger = isTrigger;

        hit.ownIsStatic = otherIsStatic;
        hit.otherIsStatic = ownIsStatic;

        hit.normal = -normal;
        hit.penetration = penetration;
        hit.point = point;

        return hit;
    }
};

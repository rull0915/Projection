//====================================================//
// ファイル名   : RenderProxy.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/29
//
// 概要 : 描画の関数を呼び出す仲介関数
//
// 更新履歴 :
// 2026/04/29 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../EachRenderer/Primitive/PrimitiveRenderer.h"
#include "SpriteProxy.h"
#include "../EachRenderer/Model/ModelRenderer.h"
#include "TextProxy.h"
#include "../EachRenderer/UI/UIRenderer.h"

#include <utility>

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// 列挙型宣言
//====================================================//
class UIProxy
{
private:
    UIRenderer* m_ui;

public:
    void SetUI(UIRenderer* ui) { m_ui = ui; }

    // ----- 描画仲介関数 ----- //
    void Triangle(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, int color, bool fillFlag = true) {
        m_ui->DrawTriangle(p1, p2, p3, color, fillFlag);
    };
    void Rect(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Vector2 p4, int color, bool fillFlag = true) {
        m_ui->DrawRect(p1, p2, p3, p4, color, fillFlag);
    };
    void Line(DirectX::SimpleMath::Vector2 start, DirectX::SimpleMath::Vector2 end, int color) {
        m_ui->DrawLine(start, end, color);
    };
    void Circle(DirectX::SimpleMath::Vector2 centerPos, float radius, uint16_t division, int color, bool fillFlag = true) {
        m_ui->DrawCircle(centerPos, radius, division, color, fillFlag);
    };
    void Box(DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, int color, bool fillFlag = true) {
        m_ui->DrawBox(min, max, color, fillFlag);
    };
};

enum class RendererType
{
    None,
    Primitive,
    Model,
    Sprite,
    Text,
    UI,
};

//====================================================//
// クラス宣言
//====================================================//
class RenderProxy
{
private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
    PrimitiveRenderer* m_primitive;
    ModelRenderer* m_model;

	SpriteProxy<> m_spriteProxy;
    TextProxy<> m_textProxy;
    UIProxy m_uiProxy;

    void* m_renderer;
    // レンダラーの変更を通知する関数ポインタ
    void(*m_switchRenderer)(void*, RendererType);

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    RenderProxy(void* renderer, void(*switchRenderer)(void*, RendererType))
        : m_primitive{ nullptr }
        , m_model{ nullptr }
        , m_spriteProxy{}
        , m_textProxy{}
        , m_uiProxy{}
        , m_renderer{ renderer }
        , m_switchRenderer{ switchRenderer }
    {}
    ~RenderProxy() = default;

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

    void SetPrimitive(PrimitiveRenderer* primitive) { m_primitive = primitive; }
    void SetModel(ModelRenderer* model) { m_model = model; }
    void SetSprite(SpriteRenderer* sprite) { m_spriteProxy.SetSprite(sprite); }
    void SetText(TextRenderer* text) { m_textProxy.SetText(text); }
    void SetUI(UIRenderer* ui) { m_uiProxy.SetUI(ui); }

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // ----------- PrimitiveRenderer ------------- //

    // Triangle
    void Triangle(DirectX::VertexPositionColor v1, DirectX::VertexPositionColor v2, DirectX::VertexPositionColor v3, bool fillFrag = true){
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawTriangle(v1, v2, v3, fillFrag);}
    void Triangle(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 p3, int color, bool fillFrag = true){
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawTriangle(p1, p2, p3, color, fillFrag);}

    // Rect
    void Rect(DirectX::VertexPositionColor v1, DirectX::VertexPositionColor v2, DirectX::VertexPositionColor v3, DirectX::VertexPositionColor v4, bool fillFrag = true) {
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawRect(v1, v2, v3, v4, fillFrag);}
    void Rect(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 p3, DirectX::SimpleMath::Vector3 p4, int color, bool fillFrag = true) {
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawRect(p1, p2, p3, p4, color, fillFrag);}

    // Line
    void Line(DirectX::VertexPositionColor v1, DirectX::VertexPositionColor v2) {
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawLine(v1, v2); }
    void Line(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, int color){
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawLine(start, end, color);}

    // Circle
    void Circle(DirectX::SimpleMath::Vector3 centerPos, DirectX::SimpleMath::Vector3 normal, float radius, uint16_t division, int color, bool fillFrag = true){
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawCircle(centerPos, normal, radius, division, color, fillFrag);}

    // Arc
    void Arc(const DirectX::SimpleMath::Vector3& center, DirectX::SimpleMath::Vector3 vStart, DirectX::SimpleMath::Vector3 vEnd, uint16_t division, float radius, int color, bool fillFlag = true) {
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawArc(center, vStart, vEnd, division, radius, color, fillFlag);}

    // Index
    void PrimitiveIndex(D3D11_PRIMITIVE_TOPOLOGY topology, uint16_t* indexes, uint16_t indexCount, DirectX::VertexPositionColor* vertexes, uint16_t vertexCount) {
        m_switchRenderer(m_renderer, RendererType::Primitive);
        m_primitive->DrawIndex(topology, indexes, indexCount, vertexes, vertexCount);
    }

    // ------------- ModelRenderer --------------- //

    void Model(DirectX::Model* model, DirectX::SimpleMath::Matrix world) {
        m_switchRenderer(m_renderer, RendererType::Model);
        m_model->DrawModel(model, world);}

    // ------------- TextRenderer --------------- //

    // UI
    UIProxy& UI() 
    {
        m_switchRenderer(m_renderer, RendererType::UI);
        return m_uiProxy; 
    }
    // Sprite
	SpriteProxy<>& Sprite()
	{
		m_switchRenderer(m_renderer, RendererType::Sprite);
        // 状態をリセット
		m_spriteProxy.Reset();
		return m_spriteProxy;
	}
    // Text
    TextProxy<>& Text()
    {
        m_switchRenderer(m_renderer, RendererType::Text);
        // 状態をリセット
        m_textProxy.Reset();
        return m_textProxy;
    }
};

//====================================================//
// ファイル名   : IRenderer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/02
//
// 概要 : レンダラーのインターフェース
//
// 更新履歴 : 
// 2026/05/02 新規作成
//====================================================//

#pragma once

//====================================================//
// クラス宣言
//====================================================//
class IRenderer
{
public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    IRenderer() {};
    virtual ~IRenderer() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 初期化関数
    virtual void Initialize() = 0;

    // 描画開始関数
    virtual void Start() = 0;

    // 描画終了関数
    virtual void End() = 0;

protected:
    DirectX::XMVECTOR GetColor(int color, float alpha) const
    {
		float r = ((color >> 16) & 0xFF) / 255.0f;
		float g = ((color >> 8 ) & 0xFF) / 255.0f;
		float b = ((color >> 0 ) & 0xFF) / 255.0f;
		return DirectX::XMVectorSet(r, g, b, alpha);
    }
};


namespace Origin
{
    //====================================================//
    // 列挙型宣言
    //====================================================//
    enum class Type
    {
    	RightTop,
    	RightCenter,
    	RightBottom,
    	CenterTop,
    	Center,
    	CenterBottom,
    	LeftTop,
    	LeftCenter,
    	LeftBottom,
    };

	inline static constexpr DirectX::SimpleMath::Vector2 GetPosition(Type originType)
	{
		switch (originType)
		{
		case Type::RightTop:
			return { 1, 0 };
		case Type::RightCenter:
			return { 1, 0.5f };
		case Type::RightBottom:
			return { 1, 1 };
		case Type::CenterTop:
			return { 0.5f, 0 };
		case Type::Center:
			return { 0.5f, 0.5f };
		case Type::CenterBottom:
			return { 0.5f, 1 };
		case Type::LeftTop:
			return { 0, 0 };
		case Type::LeftCenter:
			return { 0, 0.5f };
		case Type::LeftBottom:
			return { 0, 1 };
		default:
			return { 0, 0 };
		}
	}
}

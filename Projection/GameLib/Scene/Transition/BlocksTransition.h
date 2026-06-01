//====================================================//
// ファイル名   : BlockTransition.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要 : ブロック状の遷移演出
//
// 更新履歴 :
// 2026/05/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "SceneTransition.h"
#include "GameLib/Common/Renderer/Renderer.h"

//====================================================//
// 前方宣言
//====================================================//

class BlocksTransition : public SceneTransition
{
private:
	int m_blocksColor;

	DirectX::SimpleMath::Vector2 m_startPos;

	int m_divisionX, m_divisionY;

	std::vector<std::vector<int>> m_blockMap;

	int m_maxNum;

	// 描画可能な番号
	int m_canDrawNum;

public:
	BlocksTransition(float transTime, Transition::Mode mode, int blocksColor = 0x000000, DirectX::SimpleMath::Vector2 startPos = { 0, 0 }, int divX = 16, int divY = 9);

	void Initialize() override;

	bool Update(float elapsedTime) override;

	void Render(Renderer& renderer) override;
};


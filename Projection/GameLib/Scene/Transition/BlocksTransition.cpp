//====================================================//
// ファイル名  : BlocksTransition.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要        :  ブロック状の遷移演出
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "BlocksTransition.h"

//====================================================//
// 関数の実体宣言
//====================================================//

BlocksTransition::BlocksTransition(float transTime, Transition::Mode mode, int blocksColor, DirectX::SimpleMath::Vector2 startPos, int divX, int divY)
	: SceneTransition(transTime, mode)
	, m_blocksColor{ blocksColor }
	, m_startPos{ startPos }
	, m_divisionX{ divX }
	, m_divisionY{ divY }
	, m_blockMap{}
	, m_maxNum{ -1 }
	, m_canDrawNum{ 0 }
{
}

void BlocksTransition::Initialize()
{
	if (m_divisionX <= 0) m_divisionX = 1;
	if (m_divisionY <= 0) m_divisionY = 1;

	// ブロックマップ初期化
	m_blockMap.resize(m_divisionY);
	for(auto& coll : m_blockMap)
	{
		coll.resize(m_divisionX);
		for (auto& cell : coll) cell = INT_MAX;
	}

	// 開始地点が所属するブロックを探索
	int startPosX = (int)( m_startPos.x / ((float)Screen::WIDTH / m_divisionX) );
	int startPosY = (int)( m_startPos.y / ((float)Screen::HEIGHT / m_divisionY) );

	if (startPosX >= m_divisionX) startPosX = m_divisionX - 1;
	if (startPosY >= m_divisionY) startPosY = m_divisionY - 1;

	// 探索の開始地点を決定
	m_blockMap[startPosY][startPosX] = 0;

	// 探索の開始
	while (true)
	{
		bool change = false;

		for(int i = 0; i < m_divisionY; i++)
		{
			for(int j = 0; j < m_divisionX; j++)
			{
				// 既に割り当てられているセルなら
				if(m_blockMap[i][j] != INT_MAX)
				{
					// 上
					if (i - 1 >= 0 && m_blockMap[i - 1][j] > m_blockMap[i][j] + 1)
					{
						m_blockMap[i - 1][j] = m_blockMap[i][j] + 1;
						change = true;
					}
					// 下
					if (i + 1 < m_divisionY && m_blockMap[i + 1][j] > m_blockMap[i][j] + 1)
					{
						m_blockMap[i + 1][j] = m_blockMap[i][j] + 1;
						change = true;
					}
					// 左
					if (j - 1 >= 0 && m_blockMap[i][j - 1] > m_blockMap[i][j] + 1)
					{
						m_blockMap[i][j - 1] = m_blockMap[i][j] + 1;
						change = true;
					}
					// 右
					if (j + 1 < m_divisionX && m_blockMap[i][j + 1] > m_blockMap[i][j] + 1)
					{
						m_blockMap[i][j + 1] = m_blockMap[i][j] + 1;
						change = true;
					}
				}
			}
		}

		if (!change) break;
	}

	// 最大地を取得
	for (int i = 0; i < m_divisionY; i++)
	{
		for (int j = 0; j < m_divisionX; j++)
		{
			if (m_blockMap[i][j] > m_maxNum) m_maxNum = m_blockMap[i][j];
		}
	}
}

bool BlocksTransition::Update(float elapsedTime)
{
	// 経過時間の加算
	m_elapsedTime += elapsedTime;

	if (m_elapsedTime >= m_transSec) m_elapsedTime = m_transSec;

	m_canDrawNum = static_cast<int>((m_maxNum + 1) * (m_elapsedTime / m_transSec));

	// 遷移時間を超えたかどうか
	return (m_elapsedTime >= m_transSec);
}

void BlocksTransition::Render(Renderer& renderer)
{
	DirectX::SimpleMath::Vector2 blockSize = { (float)Screen::WIDTH / m_divisionX, (float)Screen::HEIGHT / m_divisionY };

	for (int i = 0; i < m_divisionY; i++)
	{
		for (int j = 0; j < m_divisionX; j++)
		{
			switch (m_mode)
			{
			case Transition::Mode::In:
				if (m_blockMap[i][j] <= m_canDrawNum)
				{
					renderer.Draw().UI().Box({ blockSize.x * j, blockSize.y * i }, { blockSize.x * (j + 1), blockSize.y * (i + 1) }, m_blocksColor, true);
				}
				break;
			case Transition::Mode::Out:
				if (m_blockMap[i][j] >= m_canDrawNum)
				{
					renderer.Draw().UI().Box({ blockSize.x * j, blockSize.y * i }, { blockSize.x * (j + 1), blockSize.y * (i + 1) }, m_blocksColor, true);
				}
				break;
			default:
				break;
			}
		}
	}
}

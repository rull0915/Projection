/**
 * @file    TreeManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/08
 */

#include "pch.h"
#include "TreeManager.h"

namespace RHEngine
{
	RHEngine::TreeManager::TreeManager(DirectX::SimpleMath::Vector3 size, unsigned int maxLevel, DirectX::SimpleMath::Vector3 center)
		: m_cells{}
		, m_rootSize{ size }
		, m_rootPosition{ center }
		, m_rootMin{}
		, m_rootMax{}
		, m_minimumSize{}
		, m_maxLevel{ std::min(MANAGER_MAX_LEVEL - 1, maxLevel) }
		, m_cellCount{ 0 }
		, m_pow{ 0 }
	{
		// 最大最小を算出
		m_rootMin = m_rootPosition - m_rootSize / 2;
		m_rootMax = m_rootPosition + m_rootSize / 2;

		// べき乗配列を作成
		m_pow[0] = 1;
		for (int i = 1; i < MANAGER_MAX_LEVEL; i++) m_pow[i] = m_pow[i - 1] * 8;

		// 最小空間のサイズを算出
		m_minimumSize = m_rootSize / (std::powf(2, static_cast<float>(m_maxLevel)));

		// セル数を算出
		for (unsigned int i = 0; i <= m_maxLevel; i++) m_cellCount += m_pow[i];

		// 配列用のメモリ確保
		m_cells.resize(m_cellCount);

		std::fill(m_cells.begin(), m_cells.end(), nullptr);
	}

	TreeManager::~TreeManager()
	{
		for (auto& cell : m_cells)
		{
			delete cell;
		}
		m_cells.clear();
	}

	void TreeManager::AddObject(ObjectForTree* obj)
	{
		// BoundingBoxを取得
		BoundingBox box = obj->m_pObject->GetBoundingBox();

		unsigned int index = GetMortonNumber(box);

		// セルが範囲内
		if (index < m_cellCount)
		{
			// セルがまだ存在しない
			if (!m_cells[index])
			{
				CreateCell(index);
			}

			//オブジェクトの追加
			m_cells[index]->AddObject(obj);
		}

		return;
	}

	void TreeManager::CreateCell(unsigned int morton)
	{
		// 作成済みの空間にたどり着くまでループ
		while (!m_cells[morton])
		{
			// 新しい空間を作成
			m_cells[morton] = new Cell();

			// 親空間へ
			morton = (morton - 1) >> 3;
			if (morton >= m_cellCount) break;
		}
	}

	unsigned int TreeManager::GetMortonNumber(const BoundingBox& box)
	{
		// 2点の最小空間モートン番号を取得
		unsigned int minMorton = GetMinimumIndex(box.min);
		unsigned int maxMorton = GetMinimumIndex(box.max);

		// 排他的論理和をとって所属する空間を調べる
		unsigned int def = minMorton ^ maxMorton;

		unsigned int HiLevel = 0;
		unsigned int i;
		for (i = 0; i < m_maxLevel; i++)
		{
			// 3bitずつずらしながら値が0でないレベルをチェック
			unsigned int Check = (def >> (i * 3)) & 0b111;
			if (Check != 0)
				HiLevel = i + 1;    // レベルを更新
		}
		// 片方の番号から自身の所属するレベルでの番号を取得する
		unsigned int SpaceNum = (maxMorton >> (HiLevel * 3));

		// 自分より上位のレベルの空間数を等比数列を使用し取得
		unsigned int AddNum = (m_pow[m_maxLevel - HiLevel] - 1) / 7;
		SpaceNum += AddNum; // 合計して線形配列でのインデックスを作成

		// セル数より大きき番号の場合
		if (SpaceNum > m_cellCount)
			return 0xffffffff;

		return SpaceNum;
	}

	unsigned int TreeManager::GetMortonNumber(uint8_t x, uint8_t y, uint8_t z)
	{
		return (BitDivision(x)) | (BitDivision(y) << 1) | (BitDivision(z) << 2);
	}

	unsigned int TreeManager::BitDivision(uint8_t bit)
	{
		unsigned int s = bit;
		s = (s | s << 8) & 0b000000001111000000001111;
		s = (s | s << 4) & 0b000011000011000011000011;
		s = (s | s << 2) & 0b001001001001001001001001;
		return s;
	}

	unsigned int TreeManager::GetMinimumIndex(const DirectX::SimpleMath::Vector3& point)
	{
		// 各要素を最小空間座標に変換
		uint8_t x = static_cast<uint8_t>((point.x - m_rootMin.x) / m_minimumSize.x);
		uint8_t y = static_cast<uint8_t>((point.y - m_rootMin.y) / m_minimumSize.y);
		uint8_t z = static_cast<uint8_t>((point.z - m_rootMin.z) / m_minimumSize.z);

		// 最小空間座標からモートン番号を取得
		return GetMortonNumber(x, y, z);
	}
}	// namespace RHEngine

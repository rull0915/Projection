/**
 * @file    RangeTester.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/23
 */

#include "pch.h"
#include "RangeTester.h"

RangeTester::RangeTester(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_range{ 0.0f, 1.0f }
{
	ADD_PROPERTY(RangeTester, m_range);
}

void RangeTester::Start()
{}

void RangeTester::Update(const RHEngine::GameTimer & gameTimer)
{}

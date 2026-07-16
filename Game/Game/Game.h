//
// Game.h
//

#pragma once

#include "System/DeviceResources.h"
#include "Timer/StepTimer.h"

#include <memory>

#include "GameEngine.h"
#include "Scene/Transition/TransitionBase.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
	// ========= テンプレートの処理 ========= //
public:

	Game() noexcept(false);
	~Game();

	Game(Game&&) = default;
	Game& operator= (Game&&) = default;

	Game(Game const&) = delete;
	Game& operator= (Game const&) = delete;

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// IDeviceNotify
	void OnDeviceLost() override;
	void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowMoved();
	void OnDisplayChange();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const noexcept;

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	// Device resources.
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;

	// FPSカウンタ
	uint32_t m_frameCount;
	float m_timeAccumulator;
	float m_fps;

	// ================ 自分の処理 ================== //
	
	// タイトルの文字列
public:
	static constexpr const wchar_t TITLE_STRING[]{ L"Projection" };

private:

	// ゲームエンジン本体
	std::unique_ptr<REngine::GameEngine> m_gameEngine;

	// 終了演出
	std::unique_ptr<REngine::Transition::Base> m_exitTrans;

private:
	void TitleNameUpdate(float elapsedTime);

	void RequestExit();
};

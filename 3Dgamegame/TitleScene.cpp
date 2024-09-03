#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include"GamePlayingScene.h"
#include"Game.h"
#include"Pad.h"

namespace
{
	constexpr int kFadeFrameMax = 60;
	constexpr int kStandByFrame = 120;

	constexpr int kLightningFrameMax = 200;
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager)
{
	m_updateFunc = &TitleScene::NormalUpdate;
	m_drawFunc = &TitleScene::FadeDraw;
}

TitleScene::~TitleScene()
{
	
}

void TitleScene::Load()
{
}

void TitleScene::Update()
{

	(this->*m_updateFunc)();

	Pad::Update();
}

void TitleScene::Draw()
{

	(this->*m_drawFunc)();

	if (m_isGamePlaying)
	{
		ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
	}
}

void TitleScene::FadeInUpdate()
{
	m_fps = GetFPS();

	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &TitleScene::NormalUpdate;
		m_drawFunc = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate()
{
	m_fps = GetFPS();

	
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_isGamePlaying = true;
		m_updateFunc = &TitleScene::FadeOutUpdate;
		m_drawFunc = &TitleScene::FadeDraw;
	}

	m_btnFrame++;
}

void TitleScene::FadeOutUpdate()
{

	m_fps = GetFPS();
	m_frame++;
	if (m_frame >= 60) {
		m_isGamePlaying = true;
	}
}

void TitleScene::ChangeScene(std::shared_ptr<Scene> next)
{
	StopSoundMem(m_stageBgm);
	m_manager.ChangeScene(next);

}

void TitleScene::FadeDraw()
{
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / kFadeFrameMax));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawFormatString(0, 0, 0xffffff, "TitleScene");

	DrawRotaGraph(800, 450, 1, 0, m_titleHandle, true);

	//DrawString(10, 100, "TitleScene", 0xffffff);
	DrawFormatString(730, 650, 0xffffff, "Push Z to Start");
}

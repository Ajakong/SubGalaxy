#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "ClearScene.h"
#include "TitleScene.h"

#include"Game.h"
#include"Pad.h"


ClearScene::ClearScene(SceneManager& mgr) :
	Scene(mgr)
{
	m_frame = 60;
	m_updateFunc = &ClearScene::FadeInUpdate;
	m_drawFunc = &ClearScene::FadeDraw;


}

ClearScene::~ClearScene()
{

}

void ClearScene::Load()
{
}

void ClearScene::Update()
{
	(this->*m_updateFunc)();

	Pad::Update();
}

void ClearScene::Draw()
{
	(this->*m_drawFunc)();
}

void ClearScene::FadeInUpdate()
{
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &ClearScene::NormalUpdate;
		m_drawFunc = &ClearScene::NormalDraw;
	}
}

void ClearScene::NormalUpdate()
{
	m_btnFrame++;
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_updateFunc = &ClearScene::FadeOutUpdate;
		m_drawFunc = &ClearScene::FadeDraw;
	}
}

void ClearScene::FadeOutUpdate()
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
	}
}

void ClearScene::FadeDraw()
{
	DrawRotaString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 5, 5, 0, 0, 0, 0xffffff, 0, 0, "Clear");
	DrawRotaString(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + 200, 5, 5, 0, 0, 0, 0xffffff, 0, 0, "タイトルへ");
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 2000, 2000, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ClearScene::NormalDraw()
{
	DrawRotaString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 5, 5, 0, 0, 0, 0xffffff, 0, 0, "Clear");
	DrawRotaString(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + 200, 5, 5, 0, 0, 0, 0xffffff, 0, 0, "タイトルへ");
	auto& app = Application::GetInstance();
	auto size = app.GetWindowSize();
	int idx = m_btnFrame / 10 % 3;
	constexpr int kButtonSize = 16;
	constexpr float kBtnScale = 3.0f;

}

#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "GameOverScene.h"
#include"GamePlayingScene.h"
#include "TitleScene.h"
#include"Pad.h"
#include"Game.h"
#include"GraphManager.h"
#include"Quaternion.h"
#include"SoundManager.h"
#include"FontManager.h"
#include"WorldTimer.h"

namespace
{
	const TCHAR* kModekHandlePath = "Player/jump_Falling.mv1";
	const char* kGraphPath = "Elements_pro.png";
	const char* kFrameName = "Frame.png";

	constexpr int kSelectFrameLeft = 350;
	constexpr int kSelectFrameRight = 450;
	constexpr int kSelectFrameTop = 270;
	constexpr int kSelectFrameBottom = 370;

	constexpr int kSelectFrameWidth = 230;
	constexpr int kSelectFrameHeight = 230;
	constexpr int kSelectFrameDistanceX = 50;
	constexpr int kSelectFrameDistanceY = 50;
	constexpr int kSelectFrameLeftTopSrkX = 820;
	constexpr int kSelectFrameLeftTopSrkY = 800;
	constexpr float kSelectFrameExrate = 0.3f;
	constexpr int kFadeFrameMax = 60;
}


GameOverScene::GameOverScene(SceneManager& mgr) :
	Scene(mgr),
	m_fadeSoundFrame(200),
	m_handleVelo(0,0,0),
	m_graphHandle(GraphManager::GetInstance().GetGraphData(kGraphPath)),
	m_angle(0),
	m_frameHandle(GraphManager::GetInstance().GetGraphData(kFrameName)),
	m_btnFrame(0),
	m_fadeSpeed(1)
{
	Vec3 centerPos = Vec3(800, 450, 0);

	m_graphVelocity.push_back(centerPos-Vec3(0,0,0));
	m_graphVelocity.push_back(centerPos - Vec3(0, 900, 0));
	m_graphVelocity.push_back(centerPos - Vec3(1600,0, 0));
	m_graphVelocity.push_back(centerPos - Vec3(1600, 900, 0));

	m_graphPos.push_back(Vec3(kSelectFrameLeft, kSelectFrameTop, 0));
	m_graphPos.push_back(Vec3(kSelectFrameLeft, kSelectFrameBottom, 0));
	m_graphPos.push_back(Vec3(kSelectFrameRight, kSelectFrameTop, 0));
	m_graphPos.push_back(Vec3(kSelectFrameRight, kSelectFrameBottom, 0));

	m_frame = 60;
	m_updateFunc = &GameOverScene::FadeInUpdate;
	m_drawFunc = &GameOverScene::FadeDraw;
	m_materialXPos.push_back(Vec3(0, 0, 0));
	m_fontHandle = FontManager::GetInstance().GetFontData("SF_font.ttf", "廻想体 ネクスト UP B", 60, 7, DX_FONTTYPE_NORMAL);

}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Load()
{
}

void GameOverScene::Update()
{
	(this->*m_updateFunc)();

	Pad::Update();
}

void GameOverScene::Draw()
{
	(this->*m_drawFunc)();
}

void GameOverScene::FadeInUpdate()
{
	m_frame--;

	if (m_frame <= 0)
	{
		m_updateFunc = &GameOverScene::NormalUpdate;
		m_drawFunc = &GameOverScene::NormalDraw;
	}
}

void GameOverScene::NormalUpdate()
{
	m_btnFrame += m_fadeSpeed;
	if (m_btnFrame > kFadeFrameMax)m_fadeSpeed *= -1;
	if (m_btnFrame < 0)m_fadeSpeed *= -1;

	MV1SetPosition(m_modelHandle,m_handleVelo.VGet());
	MV1SetScale(m_modelHandle, VGet(0.1f, 0.1f, 0.1f));
	MV1SetRotationXYZ(m_modelHandle, VGet(0, DX_PI_F / 2,0));

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_updateFunc = &GameOverScene::FadeOutUpdate;
		m_drawFunc = &GameOverScene::FadeOutDraw;
	}
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_selectNumber++;
	}
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_selectNumber--;
	}
}

void GameOverScene::FadeOutUpdate()
{
	m_frame++;
	m_fadeSoundFrame -= 3;

	if (60 <= m_frame)
	{
		if (m_selectNumber % 2 == 0)
		{
			m_manager.ResetScene(std::make_shared<GamePlayingScene>(m_manager));
			WorldTimer::Reset();
		}
		if (m_selectNumber % 2 == 1|| m_selectNumber % 2 == -1)
		{
			StopSoundMem(SoundManager::GetInstance().GetSoundData("GamePlaying.mp3"));
			WorldTimer::Reset();
			m_manager.ResetScene(std::make_shared<TitleScene>(m_manager));
		}
		//m_manager.PopScene();
	}
}

void GameOverScene::ChangeScene(std::shared_ptr<Scene> nextScene)
{
}

void GameOverScene::FadeDraw()
{
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));

	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xff4444, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(1040, 495, "Continue", 0xffffff);
	DrawString(1040, 595, "To Title", 0xffffff);
	m_angle+=0.05f;
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 / 3);
	
	if (m_selectNumber % 2 == 0)
	{
		DrawRectExtendGraph(950 - static_cast<int>(m_btnFrame) / 6, 470 - static_cast<int>(m_btnFrame) / 6, 1200 + static_cast<int>(m_btnFrame) / 6, 530 + static_cast<int>(m_btnFrame) / 6, 0, 0, 4167, 4167, m_frameHandle, true);
	}
	if (m_selectNumber % 2 == 1 || m_selectNumber % 2 == -1)
	{
		DrawRectExtendGraph(950 - static_cast<int>(m_btnFrame) / 6, 570 - static_cast<int>(m_btnFrame) / 6, 1200 + static_cast<int>(m_btnFrame) / 6, 630 + static_cast<int>(m_btnFrame) / 6, 0, 0, 4167, 4167, m_frameHandle, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatStringToHandle(500, 300, 0xffffff,m_fontHandle, "取得数:%d", m_materialXNum);

	{
		Vec3 zero = m_graphPos[0] + (m_graphVelocity[0].GetNormalized() * 10) * sin(m_angle);
		Vec3 one = m_graphPos[1] + (m_graphVelocity[1].GetNormalized() * 10) * sin(m_angle);
		Vec3 two = m_graphPos[2] + (m_graphVelocity[2].GetNormalized() * 10) * sin(m_angle);
		Vec3 three = m_graphPos[3] + (m_graphVelocity[3].GetNormalized() * 10) * sin(m_angle);

		DrawRectRotaGraph(static_cast<int>(zero.x), static_cast<int>(zero.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(one.x), static_cast<int>(one.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(two.x), static_cast<int>(two.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(three.x), static_cast<int>(three.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
	}

	//UI:3DmaterialX
	Vec3 zero = { 0,0,0 };
	Vec3 offSetVec = GetCameraRightVector();
	offSetVec -= GetCameraUpVector();
	offSetVec *= 9.f / 4.f;
	Quaternion myQ;
	Vec3 front = GetCameraFrontVector();
	Vec3 UIPos = ((Vec3(GetCameraPosition()) + Vec3(GetCameraFrontVector()) * 108) + Vec3(GetCameraLeftVector()) *55 + Vec3(GetCameraUpVector()) * 18);
	for (int i = 0; i < 3; i++)
	{
		myQ.SetQuaternion(offSetVec);
		float angle = DX_PI_F * 2 / 3 * i + m_angle;
		myQ.SetMove(angle, front);
		Vec3 offSet = myQ.Move(offSetVec, zero);
		DrawSphere3D((UIPos + offSet).VGet(), 2.5f, 6, 0xff00ff, 0xff00ff, false);
	}
	DrawSphere3D(UIPos.VGet(), 6, 8, 0x00ff00, 0x00ff00, false);
}

void GameOverScene::FadeOutDraw()
{
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));

	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x444444, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(1040, 495, "Continue", 0xffffff);
	DrawString(1040, 595, "To Title", 0xffffff);
	m_angle += 0.05f;
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 / 3);

	if (m_selectNumber % 2 == 0)
	{
		DrawRectExtendGraph(950 - static_cast<int>(m_btnFrame) / 6, 470 - static_cast<int>(m_btnFrame) / 6, 1200 + static_cast<int>(m_btnFrame) / 6, 530 + static_cast<int>(m_btnFrame) / 6, 0, 0, 4167, 4167, m_frameHandle, true);
	}
	if (m_selectNumber % 2 == 1)
	{
		DrawRectExtendGraph(950 - static_cast<int>(m_btnFrame) / 6, 570 - static_cast<int>(m_btnFrame) / 6, 1200 + static_cast<int>(m_btnFrame) / 6, 630 + static_cast<int>(m_btnFrame) / 6, 0, 0, 4167, 4167, m_frameHandle, true);

	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatStringToHandle(500, 300, 0xffffff, m_fontHandle, "取得数:%d", m_materialXNum);

	{
		Vec3 zero = m_graphPos[0] + (m_graphVelocity[0].GetNormalized() * 10) * sin(m_angle);
		Vec3 one = m_graphPos[1] + (m_graphVelocity[1].GetNormalized() * 10) * sin(m_angle);
		Vec3 two = m_graphPos[2] + (m_graphVelocity[2].GetNormalized() * 10) * sin(m_angle);
		Vec3 three = m_graphPos[3] + (m_graphVelocity[3].GetNormalized() * 10) * sin(m_angle);

		DrawRectRotaGraph(static_cast<int>(zero.x), static_cast<int>(zero.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(one.x), static_cast<int>(one.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(two.x), static_cast<int>(two.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(three.x), static_cast<int>(three.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
	}


	//UI:3DmaterialX
	Vec3 zero = { 0,0,0 };
	Vec3 offSetVec = GetCameraRightVector();
	offSetVec -= GetCameraUpVector();
	offSetVec *= 9.f / 4.f;
	Quaternion myQ;
	Vec3 front = GetCameraFrontVector();
	Vec3 UIPos = ((Vec3(GetCameraPosition()) + Vec3(GetCameraFrontVector()) * 108) + Vec3(GetCameraLeftVector()) * 55 + Vec3(GetCameraUpVector()) * 18);
	for (int i = 0; i < 3; i++)
	{
		myQ.SetQuaternion(offSetVec);
		float angle = DX_PI_F * 2 / 3 * i + m_angle;
		myQ.SetMove(angle, front);
		Vec3 offSet = myQ.Move(offSetVec, zero);
		DrawSphere3D((UIPos + offSet).VGet(), 2.5f, 6, 0xff00ff, 0xff00ff, false);
	}
	DrawSphere3D(UIPos.VGet(), 6, 8, 0x00ff00, 0x00ff00, false);
}

void GameOverScene::NormalDraw()
{
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));

	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x444444, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(1040, 495, "Continue", 0xffffff);
	DrawString(1040, 595, "To Title", 0xffffff);
	m_angle+=0.05f;
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 / 3);

	if (m_selectNumber % 2 == 0)
	{
		
		DrawRectExtendGraph(950 - static_cast<int>(m_btnFrame) / 6, 470 - static_cast<int>(m_btnFrame) / 6, 1200 + static_cast<int>(m_btnFrame) / 6, 530 + static_cast<int>(m_btnFrame) / 6, 0, 0, 4167, 4167, m_frameHandle, true);

	}
	if (m_selectNumber % 2 == 1 || m_selectNumber % 2 == -11)
	{
		DrawRectExtendGraph(950 - static_cast<int>(m_btnFrame) / 6, 570 - static_cast<int>(m_btnFrame) / 6, 1200 + static_cast<int>(m_btnFrame) / 6, 630 + static_cast<int>(m_btnFrame) / 6, 0, 0, 4167, 4167, m_frameHandle, true);

	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	auto& app = Application::GetInstance();
	auto size = app.GetWindowSize();
	int idx = m_btnFrame / 10 % 3;
	constexpr int kButtonSize = 16;
	constexpr float kBtnScale = 3.0f;
	DrawFormatStringToHandle(500, 300, 0xffffff,m_fontHandle, "取得数:%d", m_materialXNum);

	{
		Vec3 zero = m_graphPos[0] + (m_graphVelocity[0].GetNormalized() * 10) * sin(m_angle);
		Vec3 one = m_graphPos[1] + (m_graphVelocity[1].GetNormalized() * 10) * sin(m_angle);
		Vec3 two = m_graphPos[2] + (m_graphVelocity[2].GetNormalized() * 10) * sin(m_angle);
		Vec3 three = m_graphPos[3] + (m_graphVelocity[3].GetNormalized() * 10) * sin(m_angle);

		DrawRectRotaGraph(static_cast<int>(zero.x), static_cast<int>(zero.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(one.x), static_cast<int>(one.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(two.x), static_cast<int>(two.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
		DrawRectRotaGraph(static_cast<int>(three.x), static_cast<int>(three.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_graphHandle, true);
	}

	//UI:3DmaterialX
	Vec3 zero = { 0,0,0 };
	Vec3 offSetVec = GetCameraRightVector();
	offSetVec -= GetCameraUpVector();
	offSetVec *= 9.f / 4.f;
	Quaternion myQ;
	Vec3 front = GetCameraFrontVector();
	Vec3 UIPos = ((Vec3(GetCameraPosition()) + Vec3(GetCameraFrontVector()) * 108) + Vec3(GetCameraLeftVector()) * 55 + Vec3(GetCameraUpVector()) * 18);
	for (int i = 0; i < 3; i++)
	{
		myQ.SetQuaternion(offSetVec);
		float angle = DX_PI_F * 2 / 3 * i + m_angle;
		myQ.SetMove(angle, front);
		Vec3 offSet = myQ.Move(offSetVec, zero);
		DrawSphere3D((UIPos + offSet).VGet(), 2.5f, 6, 0xff00ff, 0xff00ff, false);
	}
	DrawSphere3D(UIPos.VGet(), 6, 8, 0x00ff00, 0x00ff00, false);
}

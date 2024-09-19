#include "Ui.h"
#include"DxLib.h"
#include"Quaternion.h"
#include"WorldTimer.h"
#include"GraphManager.h"
#include"FontManager.h"
#include"Pad.h"

#include <iomanip>
namespace
{
	//UI
	//ミッション
	constexpr int kUiText_SrkX = 150;
	constexpr int kUiText_SrkY = 170;
	constexpr int kUiText_Width = 1050;
	constexpr int kUiText_Height = 450;
	constexpr float kUiText_Exrate = 0.2f;

	//HPバーの枠
	constexpr int kUiHpbarFrame_PosX = 150;
	constexpr int kUiHpbarFrame_PosY = 35;
	constexpr int kUiHpbarFrame_SrkX = 2600;
	constexpr int kUiHpbarFrame_SrkY = 2560;
	constexpr int kUiHpbarFrame_Width = 1000;
	constexpr int kUiHpbarFrame_Height = 144;
	constexpr float kUiHpbarFrame_Exrate = 0.3f;

	//ChargeRemainTimeのバー
	constexpr int kUiCRT_PosX = 20;
	constexpr int kUiCRT_PosY = 70;
	constexpr int kUiCRT_SrkX = 2655;
	constexpr int kUiCRT_SrkY = 2720;
	constexpr int kUiCRT_Width = 30;
	constexpr int kUiCRT_DisX = 9;
	constexpr int kUiCRT_Height = 80;
	constexpr float kUiCRT_Exrate = 0.3f;

	//HPバー
	constexpr int kUiHpbar_PosX = 15;
	constexpr int kUiHpbar_PosY = 25;
	constexpr int kUiHpbar_Height = 23;
	constexpr float kUiHpbar_mag = 5.35f;//HPに掛けたらいい感じになる倍率

	//タイマーの枠
	constexpr int kUiTimeCountFrame_PosX = 1400;
	constexpr int kUiTimeCountFrame_PosY = 100;
	constexpr int kUiTimeCountFrame_SrkX = 1280;
	constexpr int kUiTimeCountFrame_SrkY = 130;
	constexpr int kUiTimeCountFrame_Width = 1040;
	constexpr int kUiTimeCountFrame_Height = 410;
	constexpr float kUiTimeCountFrame_Exrate = 0.3f;

	//タイマー
	constexpr int kUiTimeCount_PosX = 1420;
	constexpr int kUiTimeCount_PosY = 90;

	constexpr float kCameraDistanceFront = 300.f;
	constexpr float kCameraDistanceAddFrontInJump = 200.f;
	constexpr float kCameraDistanceUp = 100.f;

	constexpr int kSelectFrameLeft = 240;
	constexpr int kSelectFrameRight = 340;
	constexpr int kSelectFrameTop = 330;
	constexpr int kSelectFrameBottom = 430;

	constexpr int kSelectFrameWidth = 230;
	constexpr int kSelectFrameHeight = 230;
	constexpr int kSelectFrameDistanceX = 50;
	constexpr int kSelectFrameDistanceY = 50;
	constexpr int kSelectFrameLeftTopSrkX = 820;
	constexpr int kSelectFrameLeftTopSrkY = 800;
	constexpr float kSelectFrameExrate = 0.3f;

	const char* kGraphPath = "Elements_pro.png";
	const char* kMissionName = "mission.png";
	const char* kLeftBottonGraphPath = "Left_Botton.png";
	const char* kRightBottonGraphPath = "Right_Botton.png";
	const char* kLanguageFontPath = "Right_Botton.png";
}


Ui::Ui() :
	m_textureUIHandle(GraphManager::GetInstance().GetGraphData(kGraphPath)),
	m_fadeCount(100),
	m_materialXAngle(0),
	m_isFadeIntroFlag(false),
	m_angle(0),
	m_isFadeEnd(false),
	m_leftBottonHandle(GraphManager::GetInstance().GetGraphData(kLeftBottonGraphPath)),
	m_RightBottonHandle(GraphManager::GetInstance().GetGraphData(kRightBottonGraphPath)),
	m_languageFontHandle(FontManager::GetInstance().GetFontData("SF_font.ttf", "廻想体 ネクスト UP B")),
	m_missionHandle(GraphManager::GetInstance().GetGraphData(kMissionName)),
	m_manualFlag(true),
	m_missionFadeFrame(0),
	m_nextMissionFlag(false),
	m_isBoss(false)
{
	Vec3 centerPos = Vec3(800, 450, 0);

	m_graphVelocity.push_back(centerPos - Vec3(0, 0, 0));
	m_graphVelocity.push_back(centerPos - Vec3(0, 900, 0));
	m_graphVelocity.push_back(centerPos - Vec3(1600, 0, 0));
	m_graphVelocity.push_back(centerPos - Vec3(1600, 900, 0));

	m_graphPos.push_back(Vec3(kSelectFrameLeft, kSelectFrameTop, 0));
	m_graphPos.push_back(Vec3(kSelectFrameLeft, kSelectFrameBottom, 0));
	m_graphPos.push_back(Vec3(kSelectFrameRight, kSelectFrameTop, 0));
	m_graphPos.push_back(Vec3(kSelectFrameRight, kSelectFrameBottom, 0));
}

Ui::~Ui()
{
}

void Ui::FadeUpdate()
{
	m_angle += 0.05f;
	if (m_isFadeIntroFlag)
	{
		
		m_fadeCount++;
	}
	else
	{
		m_fadeCount -= 3;
		if (m_fadeCount < 0)m_fadeCount = 0;
	}
	if (m_fadeCount > 100)m_isFadeEnd = true;;
	if (Pad::IsTrigger(PAD_INPUT_1))m_isFadeIntroFlag = true;
	
}

void Ui::Update()
{
	
}

void Ui::Draw(int fontHandle, float playerHP, int SearchRemainTime)
{
	if (m_manualFlag)
	{
		if (Pad::IsTrigger(PAD_INPUT_7))
		{
			m_manualFlag = false;
		}
		//UI: LeftBotton
		DrawExtendGraph(200, 600, 400, 800, m_leftBottonHandle, true);
		//UI: RightBotton
		DrawExtendGraph(1200, 600, 1400, 800, m_RightBottonHandle, true);

		DrawExtendFormatString(240, 580, 1.2f, 1.2f, 0xffffff, "視点切り替え");
		DrawExtendFormatStringToHandle(1260, 570, 0.5f, 0.5f, 0xffffff, fontHandle, "Search");
		DrawExtendFormatStringToHandle(1390, 700, 0.5f, 0.5f, 0xffffff, fontHandle, "Parry");
		if (!m_isFadeEnd)
		{
			DrawExtendFormatStringToHandle(1330, 760, 0.5f, 0.5f, 0xffffff, m_languageFontHandle, "続ける");
		}
		else
		{
			DrawExtendFormatStringToHandle(1330, 760, 0.5f, 0.5f, 0xffffff, fontHandle, "Jump");
		}
	}
	else
	{
		if (Pad::IsTrigger(PAD_INPUT_7))
		{
			m_manualFlag = true;
		}
	}

	//UI:タイマー
	DrawRectRotaGraph(kUiTimeCountFrame_PosX, kUiTimeCountFrame_PosY, kUiTimeCountFrame_SrkX, kUiTimeCountFrame_SrkY, kUiTimeCountFrame_Width, kUiTimeCountFrame_Height, kUiTimeCountFrame_Exrate, 0, m_textureUIHandle, 1, 1);

	DrawFormatStringToHandle(kUiTimeCount_PosX, kUiTimeCount_PosY, 0xffffff, fontHandle, "%02d:%02d", WorldTimer::GetMinute(), WorldTimer::GetTimer());
	//UI:HPバー
	DrawRectRotaGraph(kUiHpbarFrame_PosX, kUiHpbarFrame_PosY, kUiHpbarFrame_SrkX, kUiHpbarFrame_SrkY, kUiHpbarFrame_Width, kUiHpbarFrame_Height, kUiHpbarFrame_Exrate, 0, m_textureUIHandle, true);
	DrawBox(15, 25, static_cast<int>(15 + playerHP * kUiHpbar_mag), kUiHpbar_PosY + kUiHpbar_Height, 0x00ffff, true);
	//CRTバー
	for (int i = 0; i < SearchRemainTime; i++)
	{
		DrawRectRotaGraph(kUiCRT_PosX + i * kUiCRT_DisX, kUiCRT_PosY, kUiCRT_SrkX, kUiCRT_SrkY, kUiCRT_Width, kUiCRT_Height, kUiHpbarFrame_Exrate, 0, m_textureUIHandle, true);
	}
	
	//UI:ミッション
	DrawRectRotaGraph(kUiText_SrkX + (100 * 7 - m_fadeCount * 7), static_cast<int>(kUiText_SrkY + (100 * 2.2f - m_fadeCount * 2.2f)), kUiText_SrkX, kUiText_SrkY, kUiText_Width, kUiText_Height, kUiText_Exrate * 1 + 1.25f * ((100.f - m_fadeCount) / 100.f), 0, m_textureUIHandle, true);
	if (m_nextMissionFlag)
	{
		m_missionFadeFrame += 5;
		if (m_missionFadeFrame > 150)
		{
			m_missionFadeFrame = 150;
		}
	}
	if (!m_nextMissionFlag && m_missionFadeFrame > -150)
	{
		m_missionFadeFrame -= 5;
	}
	if (m_missionFadeFrame>-150)
	{
		DrawRotaGraph(m_missionFadeFrame/3+95, 270,0.185f,0, m_missionHandle, true);
		DrawExtendFormatStringToHandle(m_missionFadeFrame/3 ,270,0.3f,0.3f,0xffffff, m_languageFontHandle, "出現した物に当たろう");
	}

	//UI:3DmaterialX
	Vec3 zero = { 0,0,0 };
	Vec3 offSetVec = GetCameraRightVector();
	offSetVec -= GetCameraUpVector();
	offSetVec *= 9.f / 5.f / 2.f;
	Quaternion myQ;
	m_materialXAngle += 0.05f;
	Vec3 front = GetCameraFrontVector();
	Vec3 UIPos = ((Vec3(GetCameraPosition()) + Vec3(GetCameraFrontVector()) * 110) + Vec3(GetCameraLeftVector()) * 102 + Vec3(GetCameraUpVector()) * 37);
	for (int i = 0; i < 3; i++)
	{
		myQ.SetQuaternion(offSetVec);
		float angle = DX_PI_F * 2 / 3 * i + m_materialXAngle;
		myQ.SetMove(angle, front);
		Vec3 offSet = myQ.Move(offSetVec, zero);
		DrawSphere3D((UIPos + offSet).VGet(), 1 * 1 + 1.25f * ((100.f - m_fadeCount) / 100.f), 6, 0xff00ff, 0xff00ff, false);
	}
	DrawSphere3D(UIPos.VGet(), 2.5f * 1.f + 1.25f * ((100.f - m_fadeCount) / 100.f), 6, 0x00ff00, 0x00ff00, false);

	if (!m_isFadeEnd)
	{
		{
			Vec3 zero = m_graphPos[0] + (m_graphVelocity[0].GetNormalized() * 10) * sin(m_angle);
			Vec3 one = m_graphPos[1] + (m_graphVelocity[1].GetNormalized() * 10) * sin(m_angle);
			Vec3 two = m_graphPos[2] + (m_graphVelocity[2].GetNormalized() * 10) * sin(m_angle);
			Vec3 three = m_graphPos[3] + (m_graphVelocity[3].GetNormalized() * 10) * sin(m_angle);

			DrawRectRotaGraph(static_cast<int>(zero.x - m_fadeCount * 7), static_cast<int>(zero.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_textureUIHandle, true);
			DrawRectRotaGraph(static_cast<int>(one.x - m_fadeCount * 7), static_cast<int>(one.y), kSelectFrameLeftTopSrkX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_textureUIHandle, true);
			DrawRectRotaGraph(static_cast<int>(two.x - m_fadeCount * 7), static_cast<int>(two.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_textureUIHandle, true);
			DrawRectRotaGraph(static_cast<int>(three.x - m_fadeCount * 7), static_cast<int>(three.y), kSelectFrameLeftTopSrkX + kSelectFrameWidth + kSelectFrameDistanceX, kSelectFrameLeftTopSrkY + kSelectFrameHeight + kSelectFrameDistanceY, kSelectFrameWidth, kSelectFrameHeight, kSelectFrameExrate, 0, m_textureUIHandle, true);
		}

		//UI:3DmaterialX
		Vec3 zero = { 0,0,0 };
		Vec3 offSetVec = GetCameraRightVector();
		offSetVec -= GetCameraUpVector();
		offSetVec *= 9.f / 4.f;
		Quaternion myQ;
		Vec3 front = GetCameraFrontVector();
		Vec3 UIPos = ((Vec3(GetCameraPosition()) + Vec3(GetCameraFrontVector()) * 108) + Vec3(GetCameraLeftVector()) * 70 + Vec3(GetCameraUpVector()) * 10+Vec3(-static_cast<float>(m_fadeCount), 0, 0));
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

	
}

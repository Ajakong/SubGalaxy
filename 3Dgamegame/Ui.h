#pragma once
#include<vector>
#include"MyLib/Vec3.h"
class Ui
{
public:
	Ui();
	~Ui();

	void FadeUpdate();
	void Update();
	void Draw(int fontHandle,float playerHP,int SearchRemainTime);
	int GetFadeCount() { return m_fadeCount; }
	void FadeNextMission() { m_nextMissionFlag = true; }
	void OutNextMission() { m_nextMissionFlag = false; }
	void IsBoss(bool flag) { m_isBoss=flag; }
private:
	/// <summary>
   /// 0:左上 1:左下 2:右上 3:右下
   /// </summary>
	std::vector<Vec3> m_graphVelocity;
	/// <summary>
	/// 0:左上 1:左下 2:右上 3:右下 
	/// </summary>
	std::vector<Vec3> m_graphPos;

	int m_leftBottonHandle;
	int m_RightBottonHandle;

	int m_languageFontHandle;
	int m_textureUIHandle;
	int m_missionHandle;
	int m_fadeCount;
	float m_materialXAngle;
	bool m_isFadeIntroFlag;
	bool m_isFadeEnd;
	bool m_manualFlag;
	bool m_nextMissionFlag;
	bool m_isBoss;
	float m_angle;
	int m_missionFadeFrame;
};


#pragma once
#include"MyLib/Vec3.h"

class Vec3;
class Ui;
class Camera;
class Player;
class Planet;
class BossPlanet;
class Takobo;
class KillerTheSeeker;
class Gorori;
class Item;
class ClearObject;
class WarpGate;

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Init();
	void Update();
	void Draw();
	int GetMaterialXCount() { return static_cast<int>(11 - poworStone.size()); };
	bool GetGameOver() { return m_isGameOverFlag; }
	bool GetClear() { return m_isClearFlag; }

	using managerState_t = void(GameManager::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(GameManager::*)();
	managerState_t m_managerDraw;
private:
	void IntroUpdate();
	void IntroDraw();

	void GamePlayingUpdate();
	void GamePlayingDraw();

private:

	struct UserData
	{
		float dissolveY;	// ディゾルヴしたい高さ
		float minY;
		float maxY;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};
	int cbuffH = CreateShaderConstantBuffer(sizeof(UserData));
	UserData* userData = static_cast<UserData*>(GetBufferShaderConstantBuffer(cbuffH));

	std::shared_ptr<Ui> ui;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Planet>> planet;
	std::shared_ptr<BossPlanet> bossPlanet;
	std::vector<std::shared_ptr<ClearObject>> clearObject;
	std::vector<std::shared_ptr<WarpGate>> warpGate;
	std::vector<std::shared_ptr<Takobo>> takobo;
	std::vector<std::shared_ptr<KillerTheSeeker>> killerTheSeeker;
	std::vector<std::shared_ptr<Gorori>> gorori;
	std::vector<std::shared_ptr<Item>> poworStone;

	Vec3 m_cameraUpVec;

	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;
	bool m_isBossWatch = false;

	int m_skyDomeH;
	int modelH ;
	int textureUIHandle;
	int fontHandle;
	int itemNum;
	int m_warpEffectHandle;
	int m_miniMapScreenHandle;
	
	// 通常のRT
	int RT;
	int RT2;
	// アウトラインのRT
	int outlineRT;
	// 法線情報のRT
	int normRT;
	// ぼかし用RT
	int blurRT;
	int shrinkRT;
	int depthRT;
};


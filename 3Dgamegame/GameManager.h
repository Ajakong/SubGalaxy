#pragma once

class Camera;
class Player;
class Planet;
class Takobo;

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Init();
	void Update();
	void Draw();

	bool GetGameOver() { return m_isGameOverFlag; }
	bool GetClear() { return m_isClearFlag; }
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

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Player> player;
	std::shared_ptr<Planet> planet;
	std::shared_ptr<Planet> planet2;
	std::vector<std::shared_ptr<Takobo>> takobo;

	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;

	int skyDomeH;
	int modelH ;
	
	int sphMapH ;
	
	int roughH;
	
	int metalH;
	
	int toonH;
	
	int psH;
	
	int vsH;
	
	int outlinePsH;
	
	int outlineVsH;
	
	int dissolveH;
	
	int postEffectH;
	
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


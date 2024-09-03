#pragma once
#include "MyLib/Physics/Collidable.h"
#include"MyLib/Vec3.h"

class Camera;

class Player : public MyEngine::Collidable
{
public:
	Player(int modelhandle);
	~Player();

	void Init();

	void Update();
	void SetMatrix();
	void Draw();


	Vec3 GetPos() const { return  m_rigid.GetPos(); }
	int GetHp() { return m_Hp; }
	void SetCameraToPlayer(Vec3 cameraToPlayer);
	Vec3 GetMoveDir() const{ return m_moveDir; }
	float GetRegenerationRange() { return m_regeneRange; }
	virtual void OnCollideEnter(const Collidable& colider);
	int WatchHp()const { return m_Hp; }

	void SetCameraAngle(float cameraAngle);
	void SetSideVec(Vec3 right) { m_sideVec = right; }
	void SetFrontVec(Vec3 front) { m_frontVec = front; }
	int GetPlayerModelhandle() { return m_modelHandle; }

	int& SetReverse() { return m_reverseFlag; }


	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	//メンバ関数ポインタ
	using playerState_t = void(Player::*)();
	playerState_t m_playerUpdate;

	using cameraState_t = void(Player::*)();
	cameraState_t m_cameraUpdate;

private:
	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);
	//アニメーションの変更
	void ChangeAnim(int animIndex);

	//状態別関数(ポインタで呼び出す)
	/*m_playerUpdateで使う*/
	/// <summary>
	/// 開始直後に呼ばれる
	/// </summary>
	void StartUpdate();
	/// <summary>
	/// 通常時
	/// </summary>
	void NeutralUpdate();
	void WalkingUpdate();
	void JumpingUpdate();
	void SpiningUpdate();
	/// <summary>
	/// 衝突関数
	/// </summary>
	void HitUpdate();
	/// <summary>
	/// 回避
	/// </summary>
	void AvoidUpdate();
	/*m_cameraUpdateで使う*/
	void Planet1Update();

	
	Vec3 GetCameraToPlayer()const;

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

	int m_Hp;
	int m_modelHandle = 0;

	int rotRad = 0;

	/// <summary>
	/// 行動のフレームを管理する
	/// </summary>
	int actionFrame = 0;

	float m_regeneRange;
	float m_angle;
	float m_spinAngle;
	float m_radius = 0;

	Vec3 m_cameraToPlayer;
	Vec3 m_cameraPos;
	//std::shared_ptr<Camera> m_camera;
	Vec3 m_moveDir;

	Vec3 m_frontVec;
	Vec3 m_sideVec;
	
	
	int m_currentAnimNo;//現在のアニメーション
	int m_prevAnimNo;//変更前のアニメーション
	float m_animBlendRate;//アニメーションの合成割合
	//0.0f:prevが再生
	//1.0:currentが再生
	int m_reverseFlag=0;

	//アニメーション変数
	int m_anim_nutral = 0;
	int m_anim_move = 0;
	int m_anim_hit = 0;
	int m_anim_jump = 0;
	int m_attach_move = 0;
	int m_attach_hit = 0;
	int m_attach_jump = 0;
	int m_attach_nutral = 0;

	float m_playAnimTime = 0.0f;
	float m_animTime_move = 0.0f;

	float m_playerRotateY = 0;
	float m_cameraAngle = 0;

	bool m_isVisibleFlag = false;
	int m_visibleCount = 0;

	int HitCount = 0;
};


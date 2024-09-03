#include "Player.h"
#include"Pad.h"
#include"MyLib/Physics/ColliderSphere.h"
#include"Camera.h"

namespace
{
	constexpr int kNetralRadius = 20;//通常時の当たり半径

	//アニメーション番号
	constexpr int kIdleAnimIndex = 1;
	//constexpr int kIdleAnimIndex = 2;//待機テスト
	constexpr int kAttackAnimIndex = 30;

	constexpr float kAnimFrameSpeed = 30.0f;//アニメーション進行速度

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 5.0f;//プレイヤーの最大速度
	constexpr float kAnalogRangeMin = 0.1f;//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;//アナログスティックから入力されるベクトルの最大値

	constexpr float kFrameParSecond = 60.0f;

	constexpr int kAvoidFrame = 60;

	constexpr int kJumpPower = 50;
}


Player::Player(int modelhandle) : Collidable(Priority::High,ObjectTag::Player),
	m_modelHandle(MV1DuplicateModel(modelhandle)),
	m_anim_move(),
	m_radius(kNetralRadius),
	m_Hp(50),
	m_playerUpdate(&Player::StartUpdate),
	m_cameraUpdate(&Player::Planet1Update),
	m_regeneRange(0),
	m_angle(0),
	m_spinAngle(0),
	m_animBlendRate(0),
	m_currentAnimNo(0),
	m_prevAnimNo(0)
{
	m_rigid.SetPos(Vec3(0, 0, 0));
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = m_radius;
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	

}

void Player::Update()
{
	m_rigid.SetVelocity(Vec3(0, 0, 0));
	(this->*m_playerUpdate)();

	Pad::Update();
	//(this->*m_cameraUpdate)();
	//m_camera->SetUpVec(GetCameraUpVector());
	//m_camera->Update(m_rigid.GetPos()+Vec3(GetCameraUpVector())*300);

	if (m_visibleCount > 200)
	{
		m_isVisibleFlag = false;
		m_visibleCount = 0;
	}
	if (m_isVisibleFlag)
	{
		m_visibleCount++;
	}
	else
	{
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.front());
		item->radius = m_radius;
	}
}

void Player::SetMatrix()
{
	//カメラのいる角度から
	//コントローラーによる移動方向を決定する
	MATRIX mtx;
	MATRIX scale = MGetScale(VGet(0.1f, 0.1f, 0.1f));
	

	MATRIX moveDir = MGetRotY((m_angle)+DX_PI_F / 2);
	mtx = MMult(scale, moveDir);

	MATRIX moving = MGetTranslate(m_rigid.GetPos().VGet());

	mtx = MMult(mtx, moving);

	MV1SetMatrix(m_modelHandle, mtx);
}

void Player::Draw()
{
	if (m_visibleCount % 5 == 0)
	{
		MV1DrawModel(m_modelHandle);
	}

#if _DEBUG
	DrawSphere3D(m_rigid.GetPos().VGet(), m_radius, 10, 0x000000, 0x00ffff, false);
	
	//printfDx("%d", HitCount);
#endif

}

void Player::SetCameraToPlayer(Vec3 cameraToPlayer)
{
	m_cameraToPlayer = cameraToPlayer;
}

void Player::OnCollideEnter(const Collidable& colider)
{
	if (colider.GetTag() == ObjectTag::Takobo)
	{
		m_Hp -= 10;
	}
}

void Player::SetCameraAngle(float cameraAngle)
{
	m_cameraAngle = cameraAngle;
}

void Player::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::EnemyAttack)
	{
		m_Hp -= 20;
	}
}

Vec3 Player::GetCameraToPlayer() const
{
	return m_cameraToPlayer;
}

bool Player::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);//現在の再生カウント
	now += kAnimFrameSpeed / kFrameParSecond;//アニメーションカウントを進める

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;
	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);

}

void Player::StartUpdate()
{
	m_regeneRange += 0.01f;
	if (m_regeneRange > 2)
	{
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::NeutralUpdate()
{
	//アナログスティックを使って移動

	int analogX = 0, analogY = 0;

	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;
	if (analogX * analogY != 0)
	{
		int a = 0;
	}
	//アナログスティックの入力10%~80%を使用する
	//ベクトルの長さが最大1000になる
	//ベクトルの長さを取得
	Vec3 move;

	float len = move.Length();
	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;
	Vec3 front=GetCameraFrontVector();
	Vec3 right = GetCameraRightVector();
	move=m_frontVec* analogY;//入力が大きいほど利教が大きい,0の時は0
	move+=m_sideVec* analogX;
	

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin / (kAnalogRangeMax - kAnalogRangeMin));
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映
	move = move.GetNormalized();
	float speed = kMaxSpeed;

	//m_angle = fmodf(m_cameraAngle, 360);//mod:余り　
	//MATRIX rotate = MGetRotY((m_angle)-DX_PI_F / 2);//本来はカメラを行列で制御し、その行列でY軸回転
	m_moveDir = move;
	move = move * speed;
	//プレイヤーの最大移動速度は0.01f/frame
	if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	{
		
	}
	if (Pad::IsTrigger(PAD_INPUT_2))//XBoxの
	{
		m_playerUpdate=&Player::SpiningUpdate;
	}
	/*auto v = VTransform(VGet(move.x, 0, move.z), rotate);
	move = Vec3(v);*/
	m_rigid.SetVelocity(move);

	//プレイヤーの最大移動速度は0.01f/frame
	//if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	//{
	//	m_radius = 0;
	//	m_playerUpdate = &Player::AvoidUpdate;
	//}
	
}

void Player::WalkingUpdate()
{
}

void Player::JumpingUpdate()
{
}

void Player::SpiningUpdate()
{
	m_spinAngle+=DX_PI_F/60;
	m_angle += DX_PI_F / 60;
	if (m_spinAngle >= DX_PI_F * 2)
	{
		m_playerUpdate = &Player::NeutralUpdate;
		m_spinAngle = 0;
	}
}

void Player::HitUpdate()
{

	//ダメージアニメーションのみ
	m_playerUpdate = &Player::NeutralUpdate;


}

void Player::AvoidUpdate()
{
	actionFrame++;

	if (actionFrame > kAvoidFrame)
	{
		actionFrame = 0;
		m_radius = kNetralRadius;
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::Planet1Update()
{

	
}

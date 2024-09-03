#include "Camera.h"
#include"Pad.h"
#include <math.h>


namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;

	constexpr float kCameraNear = 1.0f;
	constexpr float kCameraFar = 5000.0f;

	//constexpr VECTOR kFirstPos=VGet(300, 300, -200);

	constexpr float kCameraFOV = 60.0f;

	constexpr float kcameraRotateSpeed = 0.05f;

	constexpr float kOneRapAngle = 360.f;
}

Camera::Camera():
	m_pitchAngle(0),
	m_setCameraPos(0)
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(kCameraNear, kCameraFar);

	// FOV(視野角)を60度に
	SetupCamera_Perspective(kCameraFOV * (static_cast<float>(DX_PI_F) / 180.0f));

	m_pos = { 0.0f,100.0f, -200.0f };
	m_playerToCameraVec = { 0.f,100.f,-200.f };
	m_postLookPointPos = { 0,0,0 };
	m_fowardVec = { 0.f,0.f,0.1f };
}

Camera::~Camera()
{
	// 処理なし.
}

void Camera::Update(Vec3 LookPoint)
{
	Vec3 velocity;
	velocity.x = (m_cameraPoint.x - m_pos.x) / 10.f;
	velocity.y = (m_cameraPoint.y - m_pos.y) / 10.f;
	velocity.z = (m_cameraPoint.z - m_pos.z) / 10.f;
	m_pos += velocity;//イージング
	//m_pos = m_cameraPoint;
	DrawSphere3D(m_pos.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	
	//SetCameraPositionAndTarget_UpVecY(VGet(0, 0.0f, -1000.0f), m_pos.VGet());
	SetCameraPositionAndTargetAndUpVec(m_pos.VGet(),/*Vec3(m_pos.x, LookPoint.y, LookPoint.z)*/Vec3(LookPoint+m_upVec.GetNormalized()*100.0f).VGet(), m_upVec.VGet());
	Pad::Update();
	m_postLookPointPos = LookPoint;

}

void Camera::DebagDraw()
{
	
}

Vec3 Camera::cameraToPlayer(const Vec3& targetPos)
{

	Vec3 cameraToPlayer (sqrt((targetPos.x - m_pos.x) * (targetPos.x - m_pos.x)), 0.0f, sqrt((targetPos.z - m_pos.z) * (targetPos.z - m_pos.z)) );
	return cameraToPlayer;
}

void Camera::SetCameraPos(Vec3 LookPoint)
{
	(this->*m_setCameraPos)(LookPoint);
}

void Camera::SetCameraFirstPersonPos(Vec3 LookPoint)
{
	Vec3 target = LookPoint + m_fowardVec.GetNormalized();
	DrawSphere3D(LookPoint.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	SetCameraPositionAndTargetAndUpVec(LookPoint.VGet(),target.VGet(), m_upVec.VGet());
}

void Camera::SetCameraThirdPersonPos(Vec3 LookPoint)
{

	DrawSphere3D(m_pos.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	SetCameraPositionAndTargetAndUpVec(m_pos.VGet(), LookPoint.VGet(), m_upVec.VGet());
}

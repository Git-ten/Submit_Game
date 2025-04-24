#include"../Utility/VectorHeader.h"
#include"../Manager/InputManager.h"
#include"../Debuger.h"
#include "Camera.h"
#include<cmath>

Camera::Camera(void)
{
	speed_ = CAMERA_SPEED;
	mode_ = MODE::NONE;
	def_mode_ = MODE::FREE;
	pos_ = MyUtility::VECTOR_ZERO;
	targetPos_ = MyUtility::VECTOR_ZERO;
	//デフォルトの回転
	def_rot_ = Quaternion();
	rot_ = def_rot_;
	cameraUp_ = VGet(0.0f, 1.0f, 0.0f);

	angle_ = MyUtility::VECTOR_ZERO;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期設定
	SetDefault();
	ChangeMode(MODE::NONE);
}


void Camera::DrawUpdate(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	//更新処理
	update_();

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::CompCameraInfoUpdate()
{
	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::CompModeUpdate(void)
{
	update_();
}

void Camera::CompSetCameraInfo(const Position3DF& pos, const Position3DF& target, const Quaternion& rot)
{
	MyUtility::RelativeVector(speed_, pos, pos_);
	targetPos_ = target;
	rot_ = rot;
}

Position3DF Camera::GetPos(void) const
{
	return pos_;
}

Position3DF Camera::GetTarget(void) const
{
	return targetPos_;
}

void Camera::SetPos(const Position3DF& pos)
{
	pos_ = pos;
}


void Camera::SetTarget(const Position3DF& target)
{
	targetPos_ = target;
}

Quaternion Camera::GetRot()const
{
	return rot_;
}

void Camera::SetRot(const Quaternion& q)
{
	rot_ = q;
}

Quaternion Camera::GetDefRot(void) const
{
	return def_rot_;
}

void Camera::SetDefaultSpeed(void)
{
	speed_ = CAMERA_SPEED;
}

void Camera::SetSpeed(float speed)
{
	speed_ = speed;
}

float Camera::GetSpeed(void) const
{
	return speed_;
}

Dir3DF Camera::GetForward()const
{
	//return VNorm(VSub(targetPos_, pos_));
	return GetDir(MyUtility::DIR_F);
}

Dir3DF Camera::GetRight()const
{
	return GetDir(MyUtility::DIR_R);
}

Dir3DF Camera::GetLeft()const
{
	return GetDir(MyUtility::DIR_L);
}

Dir3DF Camera::GetBack()const
{
	return GetDir(MyUtility::DIR_B);
}

Dir3DF Camera::GetUp(void) const
{
	return GetDir(MyUtility::DIR_U);
}

Dir3DF Camera::GetDown(void) const
{
	return GetDir(MyUtility::DIR_D);
}

Dir3DF Camera::GetDir(const Dir3DF& vec)const
{
	return rot_.PosAxis(vec);
}

void Camera::UpdateNone(void)
{
}

Camera::MODE Camera::GetCameraMode(void) const
{
	return mode_;
}

void Camera::ChangeMode(MODE mode)
{
	// カメラの初期設定
	//SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case MODE::FREE:
		update_ = [&]() {UpdateFree();};
		break;
	case MODE::FOLLOW:
		update_ = [&]() {UpdateFollow();};
		break;
	case MODE::JUST:
		update_ = [&]() {UpdateJustGurd();};
		break;
	case MODE::FIXED_POINT:
	case MODE::SKILL_MODE:
	case MODE::SPECIAL_ATTACK_MODE:
	case MODE::SYNCHRO:
		update_ = [&]() {UpdateNone(); };
		break;
	default:
		update_ = [&]() {UpdateNone();};
		break;
	}
}

void Camera::SetDefaultMode(MODE mode)
{
	def_mode_ = mode;
}

void Camera::ChangeDefaultMode()
{
	ChangeMode(def_mode_);
}

//SetFollower()でセットされているターゲットは変更される
void Camera::SetCameraTarget(std::weak_ptr<Transform> target)
{
	targetTrans_ = target;
}

void Camera::InputCameraAngle()
{
	InputManager& ins = InputManager::GetInstance();

	//上方向
	if (ins.IsNew(KEY_INPUT_UP))
	{
		angle_.x += MyUtility::Deg2RadF(CAMERA_ANGLE_SPEED);
		if (angle_.x > LIMIT_ANGLE_UP_X)angle_.x = LIMIT_ANGLE_UP_X;
	}

	//下方向
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		angle_.x += MyUtility::Deg2RadF(-CAMERA_ANGLE_SPEED);
		if (angle_.x < -LIMIT_ANGLE_DOWN_X)angle_.x = -LIMIT_ANGLE_DOWN_X;
	}
	//左方向
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		angle_.y += MyUtility::Deg2RadF(-CAMERA_ANGLE_SPEED);
		angle_.y = MyUtility::RadIn2PI(angle_.y);
	}

	//右方向
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		angle_.y += MyUtility::Deg2RadF(CAMERA_ANGLE_SPEED);
		angle_.y = MyUtility::RadIn2PI(angle_.y);
	}
}

void Camera::UpdateFree()
{
	//空の場合は処理しない
	if (targetTrans_.lock() == nullptr)return;

	InputCameraAngle();
		
	targetPos_ = VAdd(targetTrans_.lock()->pos, IDEAL_FREE_TARGET_POS);

	Quaternion angleRot = Quaternion::AngleAxis(angle_.y, MyUtility::AXIS_Y);
	angleRot = angleRot.Mult(Quaternion::AngleAxis(angle_.x, MyUtility::AXIS_X));
	rot_ = std::move(angleRot);

	Position3DF localPos = VAdd(targetTrans_.lock()->pos, rot_.PosAxis(IDEAL_FREE_POS));
	pos_ = localPos;
}


//追従モードの処理----------------------
//追従対象をセット
void Camera::SetFollower(std::weak_ptr<Transform> Tatrans, std::weak_ptr<Transform> Lotrans)
{
	targetTrans_ = Tatrans;
	lockonTrans_ = Lotrans;
}

void Camera::SetSpecialCamera(std::function<void(void)> func)
{
	//特殊カメラモードの時の未処理
	if (mode_ == MODE::SKILL_MODE ||
		mode_ == MODE::SPECIAL_ATTACK_MODE)
	{
		update_ = func;
	}
}

//追従更新
void Camera::UpdateFollow()
{
	//空の場合は処理しない
	if (targetTrans_.lock() == nullptr)return;

	Position3DF target = targetTrans_.lock()->pos;
	target.y = 0.0f;
	Position3DF lockon = lockonTrans_.lock()->pos;
	lockon.y = 0.0f;
	
	Dir3DF vec1 = MyUtility::AXIS_Z;
	Dir3DF vec2 = VSub(lockon, target);

	//ロックオン対象の方向を向く
	Quaternion angle = std::move(Quaternion::FromToRotation(vec1, vec2));
	//89.9598度以上だとz軸以外が反対方向になるので強制的に修正
	if (Quaternion::Angle(def_rot_, angle) >= 89.9598f)
	{
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(180.0f), vec2));
	}

	if (lockonTrans_.lock() == nullptr)
	{
		targetPos_ = VAdd(targetTrans_.lock()->pos, IDEAL_RELATIVE_TARGET_POS);
	}
	//ロックオン対象がいない場合は追従対象の上らへんが視点
	else
	{
		//ロックオン対象の上らへんが視点
		targetPos_ = VAdd(lockonTrans_.lock()->pos, IDEAL_RELATIVE_TARGET_POS);
		rot_ = std::move(angle);
		rot_.Normalize();
	}

	//理想の座標
	Position3DF localPos = VAdd(targetTrans_.lock()->pos, rot_.PosAxis(IDEAL_RELATIVE_POS));
	//距離が一定以上に近い場合
	if (VSize(vec2) < TARGET_LOCKON_DIS)
	{
		localPos = VAdd(targetTrans_.lock()->pos, rot_.PosAxis(IDEAL_RELATIVE_NEAR_POS_L));
	}

	//徐々に理想の座標に近づけていく
	MyUtility::RelativeVector(speed_, localPos, pos_);
}

void Camera::UpdateJustGurd(void)
{
	//空の場合は処理しない
	if (targetTrans_.lock() == nullptr)return;

	if (lockonTrans_.lock() == nullptr)
	{
		targetPos_ = VAdd(targetTrans_.lock()->pos, IDEAL_RELATIVE_TARGET_POS);
	}
	else
	{
		Position3DF target = targetTrans_.lock()->pos;
		target.y = 0.0f;
		Position3DF lockon = lockonTrans_.lock()->pos;
		lockon.y = 0.0f;

		//ロックオン対象の上らへんが視点
		targetPos_ = VAdd(lockonTrans_.lock()->pos, IDEAL_RELATIVE_TARGET_POS);

		Dir3DF vec1 = lockon;
		//正面方向と逆方向なら方向を逆にする
		if (VDot(vec1, MyUtility::DIR_F) < 0.0f)
		{
			vec1 = VScale(vec1, -1.0f);
		}
		Dir3DF vec2 = VSub(lockon, target);

		//ロックオン対象の方向を向く
		Quaternion angle = std::move(Quaternion::FromToRotation(vec1, vec2));
		//89.9598度以上だとz軸以外が反対方向になるので強制的に修正
		if (Quaternion::Angle(def_rot_, angle) >= 89.9598f)
		{
			angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(180.0f), vec2));
		}
		rot_ = std::move(angle);
		rot_.Normalize();
	}

	//理想の座標
	Position3DF localPos = VAdd(targetTrans_.lock()->pos, rot_.PosAxis(IDEAL_JUST_POS));
	pos_ = localPos;
}
//------------------------------

void Camera::SetDefault(void)
{
	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();
}
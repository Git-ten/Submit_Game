#include"../../Utility/VectorHeader.h"
#include"../../Application/Singleton_Manager/SceneManager/SceneManager.h"
#include"../../Application/Singleton_Manager/InputManager/InputManager.h"
#include"../../Application/Singleton_Manager/InputManager/SimpleInput.h"
#include"../../Common/Transform.h"
#include "../../Common/Collider.h"
#include"../../Application/Application.h"
#include "Time.h"
#include "Camera.h"
#include<cmath>

#pragma region �萔�錾

// �J�����̏����J�����ƒ����_�Ƃ̋���()
const float DEFAULT_CAMERA_DIS = 800.0f;

//�J�����̏��������_
const Position3DF DEFAULT_CAMERA_TARGET = { 0.0f, 0.0f, 0.0f };

//�J�����̏����p�x(�I�C���[�p)
const Rot3DF DEFAULT_CAMERA_ANGLE = { 0.0f, 0.0f, 0.0f };

//�J�����̏����
const Dir3DF CAMERA_UP = { 0.0f, 1.0f, 0.0f };

//�J�����̎���p
const float CAMERA_FOV = 1.0472f;

//�J�����̑��x
const float CAMERA_SPEED = 60.0f;


//�J�����̊p�x�ω����x
const float CAMERA_ANGLE_SPEED = 2.65f;


//�J�����ƒ����_�̍ő告�΋���
const float RELATIVE_POS_TARGET_MAX = 3600.0f;

//�Ǐ]���[�h��p::�J�����ƒ����_�̏������΋���
const float FOLLOW_CAMERA_DIS = 1000.0f;
// �Ǐ]���[�h��p::�J�����ƒ����_�̍ő告�΋���
const float FOLLOW_CAMERA_DIS_MAX = 2000.0f;
//�Ǐ]���[�h��p::�J�����ƒ����_�̍ŏ����΋���
const float FOLLOW_CAMERA_DIS_MIN = 250.0f;
//�Ǐ]���[�h��p::�J�����̏����p�x
const Position3DF FOLLOW_CAMERA_ANGLE = { 25.0f, 90.0f, 0.0f };
//�Ǐ]���[�h��p::�����_�̃I�t�Z�b�g(���W�ł͂Ȃ�����::x=x�����̋���,y=y�����̋���,z=z�����̋���)
const Position3DF FOLLOW_RELATIVE_OFFSET = { 0.0f, 200.0f, 50.0f };
//�Ǐ]���[�h��p::�X�N���[���̘g�����p�[�Z���g�g��k��(�g�𒴂��Ȃ��悤�ɂ���)
const float SCREEN_PERCENTAGE = 0.9f;
//�Ǐ]���[�h��p::�J�����̉�]�p�x�𐧌�
const float CAMERA_ANGLE_MAX_UP_X = 50.0f;
const float CAMERA_ANGLE_MAX_DOWN_X = -55.0f;
const float CAMERA_ANGLE_MAX_Y = 360.0f;
//�Ǐ]���[�h��p::�J�����̑��x
const float FOLLOW_CAMERA_SPEED = 20.0f;
//�Ǐ]���[�h��p::���b�N�I�����̉�]�K�p�O����
const float LOCK_ON_ROT_DIS = 500.0f;



// �J�������o::�J�����V�F�C�N�̋���
const float CAMERA_SHAKE_POWER = 80.0f;

// �J�������o::���`�⊮���̏����l
const float LERP_MIN = 0.0f;
// �J�������o::���`�⊮���̍ő�l
const float LERP_MAX = 1.0f;
// �����t�]
const float REVERSE_VALUE = -1.0f;
// �������q
const float INIT_ZERO = 0.0f;

// ��������p
const float DECISION_VALUE = 0.0f;

// ��O�q
const int EXCEPTION_VALUE = -1;
// �l��0.6%���Z�o
const float MINO_SIX = 0.006f;
// �l��1%���Z�o
const float ONE_PERCENT = 0.01f;

#pragma endregion

Camera::Camera(void):
	speed_(CAMERA_SPEED),
	def_mode_(MODE::FREE),
	mode_(MODE::NONE),
	pos_(Utility::VECTOR_ZERO),
	targetPos_(Utility::VECTOR_ZERO),
	relativeTargetPos_(Utility::VECTOR_ZERO),
	lockOnPos_(nullptr),
	relativePosTarget_(Position3DF()),
	def_relativPosTarget_(Position3DF()),
	isColl_(false),
	cameraUp_(Utility::DIR_U),
	update_(nullptr),
	rot_(Quaternion()),
	def_rot_(Quaternion()),
	angle_(Utility::VECTOR_ZERO),
	prevAngle_(Utility::VECTOR_ZERO),
	lerpAngleTime_(INIT_ZERO),
	lockOnAngle_(Utility::VECTOR_ZERO),
	cameraControlIdx_(static_cast<int>(INIT_ZERO)),
	isCanSetUpEventCamera_(true),
	followTargetPos_(nullptr),
	isLockCameraUpdate_(false)
{

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	SetDefault();
	ChangeMode(MODE::DEBUG);

	InitDirectionName();

	UnLockIsCameraUpdate();
}

void Camera::DrawUpdate(void)
{
	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	//�X�V����
	update_();
}

const Position3DF& Camera::GetPos(void) const
{
	return pos_;
}

const Position3DF& Camera::GetPrevPos(void) const
{
	return prevPos_;
}

const Rot3DF& Camera::GetAngle(void) const
{
	return angle_;
}

const Quaternion& Camera::GetRot()const
{
	return rot_;
}

const Quaternion& Camera::GetDefRot(void) const
{
	return def_rot_;
}

Quaternion Camera::GetRotXZ(void)
{
	return Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);
}

const Position3DF& Camera::GetTargetPos(void) const
{
	return targetPos_;
}

float Camera::GetDistanceTarget(void)
{
	return VSize(relativePosTarget_);
}

void Camera::SetDistanceTarget(float distance)
{
	if (distance < DECISION_VALUE)distance *= REVERSE_VALUE;
	relativePosTarget_ = VScale(Utility::DIR_B, distance);

	//����
	if (VSize(relativePosTarget_) < FOLLOW_CAMERA_DIS_MIN)
	{
		relativePosTarget_ = VScale(Utility::DIR_B, FOLLOW_CAMERA_DIS_MIN);
	}
	if (VSize(relativePosTarget_) > FOLLOW_CAMERA_DIS_MAX)
	{
		relativePosTarget_ = VScale(Utility::DIR_B, FOLLOW_CAMERA_DIS_MAX);
	}
}

void Camera::SetDefaultDistanceTarget(void)
{
	relativePosTarget_ = def_relativPosTarget_;
}

float Camera::GetDefaultDistanceTarget(void)
{
	return VSize(def_relativPosTarget_);
}

Dir3DF Camera::GetForward()const
{
	return GetDir(Utility::DIR_F);
}

Dir3DF Camera::GetRight()const
{
	return GetDir(Utility::DIR_R);
}

Dir3DF Camera::GetLeft()const
{
	return GetDir(Utility::DIR_L);
}

Dir3DF Camera::GetBack()const
{
	return GetDir(Utility::DIR_B);
}

Dir3DF Camera::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

Dir3DF Camera::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

Dir3DF Camera::GetDir(const Dir3DF& vec)const
{
	return rot_.PosAxis(vec);
}

void Camera::UpdateNone(void)
{
}

const Camera::MODE& Camera::GetCameraMode(void) const
{
	return mode_;
}

void Camera::ChangeMode(MODE mode)
{
	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case MODE::FREE:
		update_ = [&]() {UpdateFree();};
		break;
	case MODE::FOLLOW:
		update_ = [&]() {UpdateFollow();};
		CancellLockOnTarget();
		angle_ = Utility::Deg2RadV(FOLLOW_CAMERA_ANGLE);
		break;
	case MODE::FIXED_POINT:
		update_ = [&]() {UpdateFixedPoint();};
		break;
	case MODE::SYNCHRO:
		update_ = [&]() {UpdateNone(); };
		break;
	case MODE::DEBUG:
		update_ = [&]() {UpdateDebug(); };
		break;
	default:
		update_ = [&]() {UpdateNone();};
		break;
	}
}

void Camera::SetFollowTargetPos(Position3DF& pos)
{
	followTargetPos_ = &pos;

	targetPos_ = *followTargetPos_;
	/*targetPos_ = VAdd(targetPos_, VScale(trans.lock()->GetRight(), FOLLOW_RELATIVE_OFFSET.x));
	targetPos_ = VAdd(targetPos_, VScale(trans.lock()->GetUp(), FOLLOW_RELATIVE_OFFSET.y));
	targetPos_ = VAdd(targetPos_, VScale(trans.lock()->GetForward(), FOLLOW_RELATIVE_OFFSET.z));*/

	relativePosTarget_ = VScale(Utility::DIR_B, FOLLOW_CAMERA_DIS);
	def_relativPosTarget_ = relativePosTarget_;

	angle_ = Utility::Deg2RadV(FOLLOW_CAMERA_ANGLE);
	lockOnAngle_ = angle_;
}

void Camera::SetTargetTrans(std::weak_ptr<Transform> trans)
{
	targetTrans_ = trans;
	
	targetPos_ = trans.lock()->GetPos();
	targetPos_ = VAdd(targetPos_, VScale(trans.lock()->GetRight(), FOLLOW_RELATIVE_OFFSET.x));
	targetPos_ = VAdd(targetPos_, VScale(trans.lock()->GetUp(), FOLLOW_RELATIVE_OFFSET.y));
	targetPos_ = VAdd(targetPos_, VScale(trans.lock()->GetForward(), FOLLOW_RELATIVE_OFFSET.z));
	
	relativePosTarget_ = VScale(Utility::DIR_B, FOLLOW_CAMERA_DIS);
	def_relativPosTarget_ = relativePosTarget_;
	
	angle_ = Utility::Deg2RadV(FOLLOW_CAMERA_ANGLE);
	lockOnAngle_ = angle_;
}

void Camera::SetLockOnTargetPos(Position3DF& lockon, bool isLerp)
{
	lockOnPos_ = &lockon;

	if(isLerp)
		lerpAngleTime_ = 0.0f;
}

void Camera::CancellLockOnTarget(void)
{
	lockOnPos_ = nullptr;
}

const bool Camera::IsLockOn(void) const
{
	return lockOnPos_ != nullptr;
}

void Camera::SetUpCollModel(const int id)
{
	collModelId_.push_back(id);
}

void Camera::SetUpCameraControlData(const std::vector<DataCameraControl>& dataControl)
{
	if (!isCanSetUpEventCamera_)return;

	cameraControlData_.clear();
	cameraControlIdx_ = static_cast<int>(INIT_ZERO);

	for (const auto& data : dataControl)
	{
		cameraControlData_.push_back(std::make_unique<DataCameraControl>(data));
	}
}

void Camera::ProhibicionCameraControl(void)
{
	isCanSetUpEventCamera_ = false;
}

DataCameraControl& Camera::GetRefelenceCameraContData(void)
{
	return *cameraControlData_[cameraControlIdx_];
}

const bool Camera::NextCameraControlData(void)
{
	if (cameraControlData_.size() <= cameraControlIdx_ + 1)
	{
		return false;
	}

	cameraControlIdx_++;

	return true;
}

const bool Camera::CameraDirectionEvent(const std::string& eventName)const
{
	const auto& ite = directionTable.find(eventName);
	assert(ite != directionTable.end() || !"�J�����œo�^����Ă��Ȃ��C�x���g�����Q�Ƃ��悤�Ƃ��܂���");

	return directionTable.at(eventName)();
}

const bool Camera::CameraEvent(void)
{
	static Time time = Time();

	time.UpdateTime(GetRefelenceCameraContData().speed);

	if (time.GetTotalNowTime() >= GetRefelenceCameraContData().eventTime)
	{
		time.InitTotalNowTime();
		return true;
	}

	return false;
}

const bool Camera::IsCameraDirectionEvent(void) const
{
	return cameraControlData_.size() > 0 ? true : false;
}

void Camera::LockIsCameraUpdate(void)
{
	isLockCameraUpdate_ = true;
}

void Camera::UnLockIsCameraUpdate(void)
{
	isLockCameraUpdate_ = false;
}

bool Camera::IsLockCameraUpdate(void)
{
	return isLockCameraUpdate_;
}

const bool Camera::CameraShake(void)
{
	// �J�E���^�[�̎����v�Z�萔-----
	// �J�E���^�[�T�C�N���v�Z�p�萔
	const int INT_COUNTER_CYCLE = 2;
	// �����p�萔
	const int ODD_DETACTION = 2;
	// �J�E���^�[
	static int counter = 0;
	// �V�F�C�N�̋���
	static float power = CAMERA_SHAKE_POWER;
	// -----------------------------

	static Time time = Time();
	Position3DF shakePos = Position3DF();

	if (time.GetTotalNowTime() < GetRefelenceCameraContData().eventTime)
	{
		if ((counter / INT_COUNTER_CYCLE) % ODD_DETACTION)
		{
			Quaternion q = Quaternion::Euler(angle_);
			Dir3DF dir = Dir3DF();

			const int RANDOM_MAX = 3;
			int rand = GetRand(RANDOM_MAX);
			switch (rand)
			{
			case 0:
				dir = q.GetUp();
				break;
			case RANDOM_MAX - 2:
				dir = q.GetRight();
				break;
			case RANDOM_MAX - 1:
				dir = q.GetDown();
				break;
			case RANDOM_MAX:
				dir = q.GetLeft();
				break;
			default:
				break;
			}

			shakePos = VScale(dir, power);
		}

		time.UpdateTime();
		counter++;
		power -= time.GetTotalNowTime() * GetRefelenceCameraContData().speed;
		if (power < DECISION_VALUE)power = INIT_ZERO;
	}
	else
	{
		time.InitTotalNowTime();
		counter = static_cast<int>(INIT_ZERO);
		power = CAMERA_SHAKE_POWER;
		return true;
	}

	pos_ = VAdd(pos_, shakePos);

	return false;
}

void Camera::InputFreeCamera(void)
{
	SimpleInput& input = SimpleInput::GetInstance();

	if (input.GetInput("angleUp"))
	{
		angle_.x += CAMERA_ANGLE_SPEED;
	}
	if (input.GetInput("angleLeft"))
	{
		angle_.y -= CAMERA_ANGLE_SPEED;
	}
	if (input.GetInput("angleBack"))
	{
		angle_.x -= CAMERA_ANGLE_SPEED;
	}
	if (input.GetInput("angleRight"))
	{
		angle_.y += CAMERA_ANGLE_SPEED;
	}
}

void Camera::FollowTarget(void)
{
	assert((targetTrans_.lock() != nullptr || followTargetPos_ != nullptr) || !"�J����(FollowMode)�ŒǏ]�Ώۂ��ݒ肳��Ă��Ȃ�");

	//�����_�͏�ɒǏ]�Ώۂ̈ʒu
	if (targetTrans_.lock() != nullptr)
	{
		targetPos_ = targetTrans_.lock()->GetPos();
	}
	else
	{
		targetPos_ = *followTargetPos_;
	}
	targetPos_ = VAdd(targetPos_, VScale(targetTrans_.lock()->GetRight(),	FOLLOW_RELATIVE_OFFSET.x));
	targetPos_ = VAdd(targetPos_, VScale(targetTrans_.lock()->GetUp(),		FOLLOW_RELATIVE_OFFSET.y));
	targetPos_ = VAdd(targetPos_, VScale(targetTrans_.lock()->GetForward(), FOLLOW_RELATIVE_OFFSET.z));
	targetPos_ = VAdd(targetPos_, relativeTargetPos_);
	

	//���͏���(���b�N�I�����͖���)
	InputFollowCamera();


	//���b�N�I������
	LockOn();


	UpdateCommon();
}

void Camera::InputFollowCamera(void)
{
	InputManager& input = InputManager::GetInstance();
	Position2D mousePos = input.GetMousePos();
	static Position2D prevMousePos = mousePos;//1�t���[���O�̃}�E�X���W
	
	if (isLockCameraUpdate_)
	{
		prevMousePos = mousePos;
		return;
	}

	if (GetMouseDispFlag())
	{
		prevMousePos = mousePos;
		// �}�E�X���\���ɂ���
		SetMouseDispFlag(false);
		return;
	}

	//�X�N���[���𒴂�����␳����
	Size2D screenSize = Size2D();
	if (Application::GetInstance().IsWindowMode())screenSize = Application::GetInstance().GetScreenSize();
	else										  screenSize = Application::GetInstance().GetScreenDesktopSize();
	
	//���W�̍��ł��ꂼ���]�����Z�o
	int sx = 0;
	int sy = 0;

	if (input.GetJPadNum() <= 0)
	{
		sx = mousePos.x - prevMousePos.x;
		sy = mousePos.y - prevMousePos.y;

		if (mousePos.x <= static_cast<int>(DECISION_VALUE) ||
			mousePos.x > static_cast<float>(screenSize.x) * SCREEN_PERCENTAGE ||
			mousePos.y <= static_cast<int>(DECISION_VALUE) ||
			mousePos.y > static_cast<float>(screenSize.y) * SCREEN_PERCENTAGE)
		{
			Position2D screenCenter = Application::GetInstance().GetScreenCenter();
			SetMousePoint(screenCenter.x, screenCenter.y);
			prevMousePos = screenCenter;
			return;
		}

		prevMousePos = mousePos;
	}
	else
	{
		// �R���g���[������ȏ�Ȃ����Ă���ꍇ
		auto controll = input.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		sx = static_cast<int>(static_cast<float>(controll.aKeyRX) * MINO_SIX);
		sy = static_cast<int>(static_cast<float>(controll.aKeyRY) * MINO_SIX);
	}

	if(sx != 0 || sy != 0)
		CancellLockOnTarget();

	//�����_�𒆐S�ɉ�]
	//Y����]
	angle_.y += static_cast<float>(sx) * MINO_SIX;
	
	//X����]
	angle_.x += static_cast<float>(sy) * MINO_SIX;
	
	//�p�x����
	const float degAngle = Utility::Rad2DegF(angle_.x);
	if (degAngle > CAMERA_ANGLE_MAX_UP_X)
	{
		angle_.x = Utility::Deg2RadF(CAMERA_ANGLE_MAX_UP_X);
	}
	else if (degAngle < CAMERA_ANGLE_MAX_DOWN_X)
	{
		angle_.x = Utility::Deg2RadF(CAMERA_ANGLE_MAX_DOWN_X);
	}
	const float degAngleY = Utility::Rad2DegF(angle_.y);
	if (degAngleY > CAMERA_ANGLE_MAX_Y ||
		degAngleY < -CAMERA_ANGLE_MAX_Y)
	{
		angle_.y = Utility::Deg2RadF(INIT_ZERO);
		prevAngle_ = angle_;
	}
}

void Camera::LockOn(void)
{
	if (!IsLockOn())return;

	LockOnRotation();
}

void Camera::LockOnRotation(void)
{
	// ���b�N�I���Ώۂ�Y���W��Ǐ]�ΏۂƓ����ɂ���
	Position3DF lockOnPos = *lockOnPos_;
	lockOnPos.y = targetTrans_.lock()->GetPos().y;

	Quaternion cameraRotXY = GetRotXZ();
	Dir3DF dir = VNorm(VSub(*lockOnPos_, targetTrans_.lock()->GetPos()));

	Quaternion agRot = Quaternion::FromToRotation(cameraRotXY.GetForward(), dir);
	float angleY = agRot.ToEuler().y;

	agRot = Quaternion::FromToRotation(cameraRotXY.GetForward(),
		VNorm(VSub(*lockOnPos_, targetTrans_.lock()->GetPos())));
	float angleX = agRot.ToEuler().x;

	//�G�Ƃ̍��፷�Ŋp�x�t�]
	if (lockOnPos_->y > targetTrans_.lock()->GetPos().y)
	{
		if (angleX > DECISION_VALUE)angleX *= REVERSE_VALUE;
	}
	else
	{
		if (angleX < DECISION_VALUE)angleX *= REVERSE_VALUE;
	}

	//angle_.x = MyUtility::Deg2RadF(CAMERA_ANGLE_MAX_UP_X / 2.0f);
	angle_.x = angleX;
	//�p�x����(X����]�̂�)
	float radAngle = Utility::Rad2DegF(angle_.x);
	if (radAngle > CAMERA_ANGLE_MAX_UP_X)
	{
		angle_.x = Utility::Deg2RadF(CAMERA_ANGLE_MAX_UP_X);
	}
	else if (radAngle < CAMERA_ANGLE_MAX_DOWN_X)
	{
		angle_.x = Utility::Deg2RadF(CAMERA_ANGLE_MAX_DOWN_X);
	}

	radAngle = Utility::Rad2DegF(angle_.y);
	if (radAngle > CAMERA_ANGLE_MAX_Y)
	{
		angle_.y = INIT_ZERO;
	}

	// ���b�N�I�����ɉ�]��K�p���鋗���ŉ�]
	// ����ȏ�߂��Ɖ�]���Ȃ�
	float dis = VSize(VSub(lockOnPos, targetTrans_.lock()->GetPos()));
	if (dis > LOCK_ON_ROT_DIS)
	{
		angle_.y += angleY;
	}
}

void Camera::UpdateFixedPoint(void)
{
	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::UpdateFree()
{
	InputFreeCamera();
	UpdateCommon();
}

void Camera::InputDebugCamera(void)
{
	static Position2D prevMousePos = Position2D();

	Position2D mousePos = InputManager::GetInstance().GetMousePos();
	//�}�E�X�̓��͏����擾
	bool ClicMidFlg = InputManager::GetInstance().IsClickMouseMiddle();

	int mouseWheel = GetMouseWheelRotVol();

	if (mouseWheel != 0)
	{
		Dir3DF dir = VNorm(VScale(relativePosTarget_, REVERSE_VALUE));
		const int WHEEL_SPEED = 80;
		mouseWheel *= WHEEL_SPEED;

		if (mouseWheel < 0)
		{
			//�k��
			Position3DF relative = VAdd(
				relativePosTarget_, 
				VScale(dir, static_cast<float>(mouseWheel)));
			
			//����
			float dis1 = VSize(relative);
			if (dis1 <= RELATIVE_POS_TARGET_MAX)
			{
				relativePosTarget_ = relative;
			}
		}
		else
		{
			mouseWheel *= static_cast<int>(REVERSE_VALUE);
			dir = VScale(dir, REVERSE_VALUE);
			//�g��
			relativePosTarget_ = VAdd(
				relativePosTarget_,
				VScale(dir, static_cast<float>(mouseWheel)));

			//����
			float dis1 = VSize(def_relativPosTarget_);
			float dis2 = VSize(relativePosTarget_);
			if (dis1 > dis2)
			{
				relativePosTarget_ = def_relativPosTarget_;
			}
		}
	}
	else if (ClicMidFlg)
	{
		//���͂��Ă���ԁA�}�E�X�J�[�\����\�����Ȃ�
		if (GetMouseDispFlag())
		{
			SetMouseDispFlag(false);
			prevMousePos = mousePos;
		}

		int sx = mousePos.x - prevMousePos.x;
		int sy = mousePos.y - prevMousePos.y;
		if (InputManager::GetInstance().IsNew(KEY_INPUT_LSHIFT))
		{
			MovePos(static_cast<float>(sx), static_cast<float>(sy));
		}
		else
		{
			//�����_�𒆐S�ɉ�]
			angle_.x += static_cast<float>(sy) * ONE_PERCENT;
			angle_.y += static_cast<float>(sx) * ONE_PERCENT;
		}

		//�O�̊p�x�Ƃ̍����Ȃ��悤�ɂ���
		prevAngle_ = angle_;
		lerpAngleTime_ = LERP_MAX;

		prevMousePos = mousePos;
	}
	else
	{
		//�}�E�X�J�[�\����\�����Ȃ���
		if (!GetMouseDispFlag())
		{
			SetMouseDispFlag(true);
		}
	}
}

void Camera::MovePos(float valueX, float valueY)
{
	Dir3DF dir = Dir3DF();

	//���W�ړ�
	if (valueX < static_cast<int>(INIT_ZERO))
	{
		valueX *= static_cast<int>(REVERSE_VALUE);
		dir = rot_.GetLeft();
	}
	else if (valueX > static_cast<int>(INIT_ZERO))
	{
		dir = rot_.GetRight();
	}

	targetPos_ = VAdd(targetPos_, VScale(dir, valueX));

	if (valueY < static_cast<int>(INIT_ZERO))
	{
		valueY *= static_cast<int>(REVERSE_VALUE);
		dir = rot_.GetUp();
	}
	else if (valueY > static_cast<int>(INIT_ZERO))
	{
		dir = rot_.GetDown();
	}

	targetPos_ = VAdd(targetPos_, VScale(dir, valueY));
}

//�Ǐ]�X�V
void Camera::UpdateFollow()
{
	FollowTarget();
}

void Camera::UpdateDebug(void)
{
	InputDebugCamera();
	UpdateCommon();
}

void Camera::UpdateCommon(void)
{
 	Rot3DF angle = Utility::Lerp(prevAngle_, angle_, lerpAngleTime_);
	
	lerpAngleTime_ += CAMERA_ANGLE_SPEED * SceneManager::GetInstance().GetDeltaTime();

	//����
	if (lerpAngleTime_ > LERP_MAX)
	{
		lerpAngleTime_ = LERP_MAX;
		prevAngle_ = angle_;
	}

	rot_ = def_rot_.Mult(Quaternion::Euler(angle));
	//rot_ = def_rot_.Mult(Quaternion::Euler(angle_));

	cameraUp_ = rot_.GetUp();

	prevPos_ = pos_;

	pos_ = Quaternion::PosAxis(rot_, relativePosTarget_);
	pos_ = VAdd(pos_, targetPos_);

	// �J��������p�f�[�^�Ɋ�Â�������
	if(!isLockCameraUpdate_)UpdateCameraControl();

	CameraCollision();

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::UpdateCameraControl(void)
{
	// �f�[�^���Ȃ��Ȃ珈�����Ȃ�
	if (cameraControlData_.size() <= 0)return;

	// ���`�⊮�̏I��
	static bool isEndLerp = false;
	// ����J�����C�x���g�̏I��
	static bool isEndEvent = false;

	// ���`�⊮�̕⊮�W���^�C�}�[
	static Time time = Time();

	// �⊮�O�̃J��������
	static float prevDis = GetDistanceTarget();

	// �⊮�O�̒����_
	static Position3DF prevTarget = relativeTargetPos_;
	// �J�������o�O�̃o�b�N�A�b�v
	static Position3DF backUpTarget = relativeTargetPos_;
	// �⊮�O�̃J�����̏����
	static Dir3DF prevDirUp = cameraUp_;
	// �J�������o�O�̃o�b�N�A�b�v
	static Dir3DF backUpDirUp = cameraUp_;

	// �l�̏���������(�^�C�}�[��0�̎�)
	if (!isEndLerp && time.GetTotalNowTime() <= LERP_MIN)
	{
		prevDis = GetDistanceTarget();
		prevTarget = relativeTargetPos_;
		prevDirUp = cameraUp_;

		if (cameraControlIdx_ <= static_cast<int>(INIT_ZERO))
		{
			backUpTarget = relativeTargetPos_;
			backUpDirUp = cameraUp_;
		}
	}



	// �����̐��`�⊮����
	if (!isColl_)
	{
		LerpDistance(prevDis, time.GetTotalNowTime());
		LerpTargetPos(prevTarget, time.GetTotalNowTime());
		LerpCameraUp(prevDirUp, time.GetTotalNowTime());
	}


	// ���`�⊮���x
	float speed = GetRefelenceCameraContData().speed;
	// ����C�x���g�J�������I�����Ă���Ȃ�A���̃J�����f�[�^�ɖ߂������ɐ؂�ւ���
	// �^�C�}�[���t�Ɍ���悤�ɂ��Ă���
	if (isEndEvent)speed *= REVERSE_VALUE;
	time.UpdateTime(speed);



	// ���`�⊮�̊�������(�f�[�^�̕⊮)
	if (time.GetTotalNowTime() >= LERP_MAX)
	{
		isEndLerp = true;
	}
	// ���`�⊮�̊�������(���ɖ߂�����)
	else if (time.GetTotalNowTime() <= LERP_MIN)
	{
		time.InitTotalNowTime();
		isEndLerp = false;
		isEndEvent = false;

		// �C�x���g�f�[�^�̑S����
		for (auto& data : cameraControlData_)
		{
			data.reset();
			data = nullptr;
		}
		cameraControlData_.clear();
		cameraControlIdx_ = static_cast<int>(INIT_ZERO);
		isCanSetUpEventCamera_ = true;
	}



	// ���`�⊮�I�����ɓ���J�����C�x���g�����s����
	if (isEndLerp)
	{
		if (!isEndEvent)
		{
			if (CameraDirectionEvent(GetRefelenceCameraContData().eventName))
			{
				// �C�x���g�̏I������
				isEndEvent = true;
				time.SetTotalNowTime(LERP_MAX);

				if (NextCameraControlData())
				{
					isEndLerp = false;
					isEndEvent = false;
					time.InitTotalNowTime();
				}
				else
				{
					prevDis = GetDefaultDistanceTarget();
					prevTarget = backUpTarget;
					prevDirUp = backUpDirUp;
					isCanSetUpEventCamera_ = false;
				}
			}
		}
	}
}

void Camera::LerpDistance(const float prevDis, const float t)
{
	float dis = GetRefelenceCameraContData().distance;
	if (dis == static_cast<float>(EXCEPTION_VALUE))return;

	SetDistanceTarget(Utility::Lerp(prevDis, dis, t));
}

void Camera::LerpTargetPos(const Position3DF& prevPos, const float t)
{
	relativeTargetPos_ = Utility::Lerp(prevPos,
		VAdd(prevPos, GetRefelenceCameraContData().targetPos), t);
}

void Camera::LerpCameraUp(const Dir3DF& prevDir, const float t)
{
	Quaternion q = Quaternion();
	Quaternion lerpQ = Quaternion::Euler(GetRefelenceCameraContData().angleRot);

	q = Quaternion::Slerp(q, lerpQ, static_cast<double>(t));
	q = rot_.Mult(q);

	cameraUp_ = q.GetUp();
}

void Camera::InitDirectionName(void)
{
	directionTable.clear();
	directionTable.emplace("None", [&]() { return CameraEvent(); });
	directionTable.emplace("CameraShake", [&]() { return CameraShake(); });
}

void Camera::SetDefault(void)
{
	// �����_
	targetPos_ = DEFAULT_CAMERA_TARGET;
	relativeTargetPos_ = DEFAULT_CAMERA_TARGET;

	// �J�����̏����ݒ�
	pos_ = VAdd(targetPos_, VScale(Utility::DIR_B, DEFAULT_CAMERA_DIS));
	prevPos_ = pos_;

	relativePosTarget_ = pos_;
	def_relativPosTarget_ = relativePosTarget_;

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rot_ = Quaternion::Identity();
	def_rot_ = Quaternion::Identity();

	angle_ = Utility::Deg2RadV(DEFAULT_CAMERA_ANGLE);
	lockOnAngle_ = angle_;
	prevAngle_ = angle_;

	isCanSetUpEventCamera_ = true;
	cameraControlIdx_ = static_cast<int>(INIT_ZERO);
	cameraControlData_.clear();

	followTargetPos_ = nullptr;

	CancellLockOnTarget();
}

void Camera::CameraCollision(void)
{
	bool hitSphere = false;

	for (const auto& collModel : collModelId_)
	{
		Line3DF line = { targetPos_ , pos_ };
		auto dim = Collider::CollisionLine2ModelInfo(collModel, Collider::ALL_MODEL_INFO_FRAME_IDX, line);

		isColl_ = dim.HitFlag;

		if (isColl_)
		{
			//���݂̃J��������
			const float NOW_CAMERA_DIS = GetDistanceTarget();

			//�␳���鋗�����v�Z
			float dis = VSize(VSub(dim.HitPosition, pos_));
			dis = NOW_CAMERA_DIS - dis;

			SetDistanceTarget(dis);
		}

		//�J�����̋��Ƃ̓����蔻��
		const float CAMERA_SPHERE = 150;
		SphereF sph = { pos_, CAMERA_SPHERE };
		
		if (!hitSphere)
		{
			hitSphere = Collider::CollisionSphere2Model(
				collModel, Collider::ALL_MODEL_INFO_FRAME_IDX,
				sph);
		}
	}

	if (!hitSphere)
	{
		if (!IsCameraDirectionEvent())
		{
			SetDefaultDistanceTarget();
		}
	}
}
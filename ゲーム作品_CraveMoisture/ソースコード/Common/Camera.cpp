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

#pragma region 定数宣言

// カメラの初期カメラと注視点との距離()
const float DEFAULT_CAMERA_DIS = 800.0f;

//カメラの初期注視点
const Position3DF DEFAULT_CAMERA_TARGET = { 0.0f, 0.0f, 0.0f };

//カメラの初期角度(オイラー角)
const Rot3DF DEFAULT_CAMERA_ANGLE = { 0.0f, 0.0f, 0.0f };

//カメラの上方向
const Dir3DF CAMERA_UP = { 0.0f, 1.0f, 0.0f };

//カメラの視野角
const float CAMERA_FOV = 1.0472f;

//カメラの速度
const float CAMERA_SPEED = 60.0f;


//カメラの角度変化速度
const float CAMERA_ANGLE_SPEED = 2.65f;


//カメラと注視点の最大相対距離
const float RELATIVE_POS_TARGET_MAX = 3600.0f;

//追従モード専用::カメラと注視点の初期相対距離
const float FOLLOW_CAMERA_DIS = 1000.0f;
// 追従モード専用::カメラと注視点の最大相対距離
const float FOLLOW_CAMERA_DIS_MAX = 2000.0f;
//追従モード専用::カメラと注視点の最小相対距離
const float FOLLOW_CAMERA_DIS_MIN = 250.0f;
//追従モード専用::カメラの初期角度
const Position3DF FOLLOW_CAMERA_ANGLE = { 25.0f, 90.0f, 0.0f };
//追従モード専用::注視点のオフセット(座標ではなく距離::x=x方向の距離,y=y方向の距離,z=z方向の距離)
const Position3DF FOLLOW_RELATIVE_OFFSET = { 0.0f, 200.0f, 50.0f };
//追従モード専用::スクリーンの枠を◯パーセント拡大縮小(枠を超えないようにする)
const float SCREEN_PERCENTAGE = 0.9f;
//追従モード専用::カメラの回転角度を制限
const float CAMERA_ANGLE_MAX_UP_X = 50.0f;
const float CAMERA_ANGLE_MAX_DOWN_X = -55.0f;
const float CAMERA_ANGLE_MAX_Y = 360.0f;
//追従モード専用::カメラの速度
const float FOLLOW_CAMERA_SPEED = 20.0f;
//追従モード専用::ロックオン時の回転適用外距離
const float LOCK_ON_ROT_DIS = 500.0f;



// カメラ演出::カメラシェイクの強さ
const float CAMERA_SHAKE_POWER = 80.0f;

// カメラ演出::線形補完時の初期値
const float LERP_MIN = 0.0f;
// カメラ演出::線形補完時の最大値
const float LERP_MAX = 1.0f;
// 符号逆転
const float REVERSE_VALUE = -1.0f;
// 初期化子
const float INIT_ZERO = 0.0f;

// 符号判定用
const float DECISION_VALUE = 0.0f;

// 例外子
const int EXCEPTION_VALUE = -1;
// 値の0.6%を算出
const float MINO_SIX = 0.006f;
// 値の1%を算出
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
	// カメラの初期設定
	SetDefault();
	ChangeMode(MODE::DEBUG);

	InitDirectionName();

	UnLockIsCameraUpdate();
}

void Camera::DrawUpdate(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	//更新処理
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

	//制限
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
	// カメラの初期設定
	SetDefault();

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
	assert(ite != directionTable.end() || !"カメラで登録されていないイベント名を参照しようとしました");

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
	// カウンターの周期計算定数-----
	// カウンターサイクル計算用定数
	const int INT_COUNTER_CYCLE = 2;
	// 奇数判定用定数
	const int ODD_DETACTION = 2;
	// カウンター
	static int counter = 0;
	// シェイクの強さ
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
	assert((targetTrans_.lock() != nullptr || followTargetPos_ != nullptr) || !"カメラ(FollowMode)で追従対象が設定されていない");

	//注視点は常に追従対象の位置
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
	

	//入力処理(ロックオン時は無効)
	InputFollowCamera();


	//ロックオン処理
	LockOn();


	UpdateCommon();
}

void Camera::InputFollowCamera(void)
{
	InputManager& input = InputManager::GetInstance();
	Position2D mousePos = input.GetMousePos();
	static Position2D prevMousePos = mousePos;//1フレーム前のマウス座標
	
	if (isLockCameraUpdate_)
	{
		prevMousePos = mousePos;
		return;
	}

	if (GetMouseDispFlag())
	{
		prevMousePos = mousePos;
		// マウスを非表示にする
		SetMouseDispFlag(false);
		return;
	}

	//スクリーンを超えたら補正する
	Size2D screenSize = Size2D();
	if (Application::GetInstance().IsWindowMode())screenSize = Application::GetInstance().GetScreenSize();
	else										  screenSize = Application::GetInstance().GetScreenDesktopSize();
	
	//座標の差でそれぞれ回転数を算出
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
		// コントローラが一つ以上つながっている場合
		auto controll = input.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		sx = static_cast<int>(static_cast<float>(controll.aKeyRX) * MINO_SIX);
		sy = static_cast<int>(static_cast<float>(controll.aKeyRY) * MINO_SIX);
	}

	if(sx != 0 || sy != 0)
		CancellLockOnTarget();

	//注視点を中心に回転
	//Y軸回転
	angle_.y += static_cast<float>(sx) * MINO_SIX;
	
	//X軸回転
	angle_.x += static_cast<float>(sy) * MINO_SIX;
	
	//角度制限
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
	// ロックオン対象のY座標を追従対象と同じにする
	Position3DF lockOnPos = *lockOnPos_;
	lockOnPos.y = targetTrans_.lock()->GetPos().y;

	Quaternion cameraRotXY = GetRotXZ();
	Dir3DF dir = VNorm(VSub(*lockOnPos_, targetTrans_.lock()->GetPos()));

	Quaternion agRot = Quaternion::FromToRotation(cameraRotXY.GetForward(), dir);
	float angleY = agRot.ToEuler().y;

	agRot = Quaternion::FromToRotation(cameraRotXY.GetForward(),
		VNorm(VSub(*lockOnPos_, targetTrans_.lock()->GetPos())));
	float angleX = agRot.ToEuler().x;

	//敵との高低差で角度逆転
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
	//角度制限(X軸回転のみ)
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

	// ロックオン時に回転を適用する距離で回転
	// それ以上近いと回転しない
	float dis = VSize(VSub(lockOnPos, targetTrans_.lock()->GetPos()));
	if (dis > LOCK_ON_ROT_DIS)
	{
		angle_.y += angleY;
	}
}

void Camera::UpdateFixedPoint(void)
{
	// カメラの設定(位置と注視点による制御)
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
	//マウスの入力情報を取得
	bool ClicMidFlg = InputManager::GetInstance().IsClickMouseMiddle();

	int mouseWheel = GetMouseWheelRotVol();

	if (mouseWheel != 0)
	{
		Dir3DF dir = VNorm(VScale(relativePosTarget_, REVERSE_VALUE));
		const int WHEEL_SPEED = 80;
		mouseWheel *= WHEEL_SPEED;

		if (mouseWheel < 0)
		{
			//縮小
			Position3DF relative = VAdd(
				relativePosTarget_, 
				VScale(dir, static_cast<float>(mouseWheel)));
			
			//制限
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
			//拡大
			relativePosTarget_ = VAdd(
				relativePosTarget_,
				VScale(dir, static_cast<float>(mouseWheel)));

			//制限
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
		//入力している間、マウスカーソルを表示しない
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
			//注視点を中心に回転
			angle_.x += static_cast<float>(sy) * ONE_PERCENT;
			angle_.y += static_cast<float>(sx) * ONE_PERCENT;
		}

		//前の角度との差がないようにする
		prevAngle_ = angle_;
		lerpAngleTime_ = LERP_MAX;

		prevMousePos = mousePos;
	}
	else
	{
		//マウスカーソルを表示しなおす
		if (!GetMouseDispFlag())
		{
			SetMouseDispFlag(true);
		}
	}
}

void Camera::MovePos(float valueX, float valueY)
{
	Dir3DF dir = Dir3DF();

	//座標移動
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

//追従更新
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

	//制限
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

	// カメラ制御用データに基づいた制御
	if(!isLockCameraUpdate_)UpdateCameraControl();

	CameraCollision();

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::UpdateCameraControl(void)
{
	// データがないなら処理しない
	if (cameraControlData_.size() <= 0)return;

	// 線形補完の終了
	static bool isEndLerp = false;
	// 特殊カメライベントの終了
	static bool isEndEvent = false;

	// 線形補完の補完係数タイマー
	static Time time = Time();

	// 補完前のカメラ距離
	static float prevDis = GetDistanceTarget();

	// 補完前の注視点
	static Position3DF prevTarget = relativeTargetPos_;
	// カメラ演出前のバックアップ
	static Position3DF backUpTarget = relativeTargetPos_;
	// 補完前のカメラの上方向
	static Dir3DF prevDirUp = cameraUp_;
	// カメラ演出前のバックアップ
	static Dir3DF backUpDirUp = cameraUp_;

	// 値の初期化処理(タイマーが0の時)
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



	// 距離の線形補完処理
	if (!isColl_)
	{
		LerpDistance(prevDis, time.GetTotalNowTime());
		LerpTargetPos(prevTarget, time.GetTotalNowTime());
		LerpCameraUp(prevDirUp, time.GetTotalNowTime());
	}


	// 線形補完速度
	float speed = GetRefelenceCameraContData().speed;
	// 特殊イベントカメラが終了しているなら、元のカメラデータに戻す処理に切り替える
	// タイマーが逆に減るようにしている
	if (isEndEvent)speed *= REVERSE_VALUE;
	time.UpdateTime(speed);



	// 線形補完の完了処理(データの補完)
	if (time.GetTotalNowTime() >= LERP_MAX)
	{
		isEndLerp = true;
	}
	// 線形補完の完了処理(元に戻す処理)
	else if (time.GetTotalNowTime() <= LERP_MIN)
	{
		time.InitTotalNowTime();
		isEndLerp = false;
		isEndEvent = false;

		// イベントデータの全消去
		for (auto& data : cameraControlData_)
		{
			data.reset();
			data = nullptr;
		}
		cameraControlData_.clear();
		cameraControlIdx_ = static_cast<int>(INIT_ZERO);
		isCanSetUpEventCamera_ = true;
	}



	// 線形補完終了時に特殊カメライベントを実行する
	if (isEndLerp)
	{
		if (!isEndEvent)
		{
			if (CameraDirectionEvent(GetRefelenceCameraContData().eventName))
			{
				// イベントの終了処理
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
	// 注視点
	targetPos_ = DEFAULT_CAMERA_TARGET;
	relativeTargetPos_ = DEFAULT_CAMERA_TARGET;

	// カメラの初期設定
	pos_ = VAdd(targetPos_, VScale(Utility::DIR_B, DEFAULT_CAMERA_DIS));
	prevPos_ = pos_;

	relativePosTarget_ = pos_;
	def_relativPosTarget_ = relativePosTarget_;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
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
			//現在のカメラ距離
			const float NOW_CAMERA_DIS = GetDistanceTarget();

			//補正する距離を計算
			float dis = VSize(VSub(dim.HitPosition, pos_));
			dis = NOW_CAMERA_DIS - dis;

			SetDistanceTarget(dis);
		}

		//カメラの球との当たり判定
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
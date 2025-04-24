#pragma once
#include<memory>
#include<functional>
#include <DxLib.h>
#include "DataLoad/DataCameraControl.h"
#include "Quaternion.h"
#include"../../Utility/Utility.h"
#include "../Application/Singleton_Manager/InputManager/InputManager.h"

class Transform;

class Camera
{

public:

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;
	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 150000.0f;


	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//定点カメラ
		FREE,			//フリーモード
		FOLLOW,			//追従モード
		SYNCHRO,		//他のカメラと同期する
		DEBUG,			//デバッグモード
	};


	//コンストラクタ
	Camera(void);
	//デストラクタ
	~Camera(void);


	//初期化
	void Init(void);
	//描画
	//カメラ関数の都合上、描画と更新は同時に行う
	void DrawUpdate(void);

	//カメラ座標取得
	const Position3DF& GetPos(void) const;
	// 1フレーム前のカメラ座標を取得
	const Position3DF& GetPrevPos(void)const;

	//回転情報の取得
	const Rot3DF& GetAngle(void)const;
	const Quaternion& GetRot(void)const;

	//デフォルト回転情報の取得
	const Quaternion& GetDefRot(void)const;


	//ワールドY方向に垂直なカメラの角度
	Quaternion GetRotXZ(void);

	// 視点座標を取得
	const Position3DF& GetTargetPos(void)const;

	// 視点とカメラ座標の距離
	float GetDistanceTarget(void);
	// 視点とカメラ座標の距離をセット
	void SetDistanceTarget(float distance);
	// 視点とカメラ座標の距離をデフォルトにする
	void SetDefaultDistanceTarget(void);
	float GetDefaultDistanceTarget(void);


	//前方方向取得
	Dir3DF GetForward(void)const;
	//右方向を取得
	Dir3DF GetRight(void)const;
	//左方向を取得
	Dir3DF GetLeft(void)const;
	//後ろ方向を取得
	Dir3DF GetBack(void)const;
	//上方向を取得
	Dir3DF GetUp(void)const;
	//下方向を取得
	Dir3DF GetDown(void)const;
	//方向を取得
	Dir3DF GetDir(const Dir3DF& vec)const;


	//現在のカメラモード
	const MODE& GetCameraMode(void)const;
	// カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従モード専用::追従対象を参照する(座標)
	void SetFollowTargetPos(Position3DF& pos);
	//追従モード専用::追従対象を参照する
	void SetTargetTrans(std::weak_ptr<Transform> trans);
	//追従モード専用::カメラのロックオン対象をセットする(デフォルトでは追従対象と同じになる)
	void SetLockOnTargetPos(Position3DF& lockon, bool isLerp = false);
	//追従モード専用::ロックオン解除
	void CancellLockOnTarget(void);
	// 追従モード専用::ロックオンしているか
	const bool IsLockOn(void)const;

	void SetUpCollModel(const int id);

	// カメラの演出処理用データをセットアップ
	void SetUpCameraControlData(const std::vector<DataCameraControl>& dataControl);
	// カメラの演出追加を禁止
	void ProhibicionCameraControl(void);

	// 線形補完を含めたイベント処理の実行状態
	const bool IsCameraDirectionEvent(void)const;

	void LockIsCameraUpdate(void);
	void UnLockIsCameraUpdate(void);
	bool IsLockCameraUpdate(void);

private:

	using DIRECTION_TABLE = std::map<std::string, std::function<bool(void)>>;

private:

	// 更新をロックするか
	bool isLockCameraUpdate_;

	// 当たり判定を行うモデルのID
	std::vector<int> collModelId_;

	// 演出カメラのセットアップ許可フラグ
	bool isCanSetUpEventCamera_;

	// 演出関数と名称の関連付け
	DIRECTION_TABLE directionTable;

	// 現在の指定するカメラ制御データのインデックス
	int cameraControlIdx_;


	// 現在指定しているカメラ制御データの参照を得る
	DataCameraControl& GetRefelenceCameraContData(void);
	// 次に使用するカメラ制御データ(無ければ、falseを返す)
	const bool NextCameraControlData(void);

	// 指定するイベントの実行及び終了状態の取得
	const bool CameraDirectionEvent(const std::string& eventName)const;
	
	// イベント終了までの時間を計測
	const bool CameraEvent(void);

	// カメラシェイク(trueでカメラシェイク終了の意)
	const bool CameraShake(void);

private:

	//速度
	float speed_;


	//デフォルトカメラモード
	MODE def_mode_;


	// カメラモード
	MODE mode_;


	// カメラの位置
	Position3DF pos_;
	// 1フレーム前のカメラの位置
	Position3DF prevPos_;


	// カメラの注視点
	Position3DF targetPos_;
	// カメラの相対注視点
	Position3DF relativeTargetPos_;

	//カメラのロックオン対象
	Position3DF* lockOnPos_;


	//カメラの座標と注視点の相対座標
	Position3DF relativePosTarget_;
	Position3DF def_relativPosTarget_;

	// カメラの当たり判定の有無
	// ステージとの当たり判定を取得することを想定している
	bool isColl_;


	// カメラの上方向
	Dir3DF cameraUp_;

	// カメラ演出用制御データ
	std::vector<std::unique_ptr<DataCameraControl>> cameraControlData_;


	// 更新関数
	using CameraUpdate = std::function<void(void)>;
	CameraUpdate update_;


	// カメラの回転
	Quaternion rot_;
	//デフォルトの回転
	Quaternion def_rot_;


	//カメラの角度(弧度法)
	Rot3DF angle_;
	//線形補完のために角度変更前の角度を記録
	Rot3DF prevAngle_;
	//線形補完のタイム
	float lerpAngleTime_;


	//ロックオン時のカメラ角度(弧度法)(デフォルトでは追従対象を同じになる)
	Rot3DF lockOnAngle_;


	//追従する際の対象モデルの参照
	//参照を格納する
	std::weak_ptr<Transform> targetTrans_;
	// 追従する際の対象座標変数の参照
	// 参照を格納する
	Position3DF* followTargetPos_;


	//内部でのみ使用する関数
	// 名前と関数の関連付けを初期化
	void InitDirectionName(void);

	// カメラを初期位置に戻す
	void SetDefault(void);

	void CameraCollision(void);

	//何もしない関数
	void UpdateNone(void);
	
	//関数ポインタ用
	void UpdateFixedPoint(void);
	void UpdateFree(void);
	void UpdateFollow(void);
	void UpdateDebug(void);

	//共通で行う処理
	void UpdateCommon(void);
	// カメラ制御用データに基づいた制御処理
	void UpdateCameraControl(void);
	// カメラの距離の線形補完
	void LerpDistance(const float prevDis, const float t);
	// カメラの注視点の線形補完
	void LerpTargetPos(const Position3DF& prevPos, const float t);
	// カメラの上方向の線形補完
	void LerpCameraUp(const Dir3DF& prevDir, const float t);


	//フリーモード処理----------------------
	//操作
	void InputFreeCamera(void);
	//--------------------------------------

	//追従モードの処理----------------------
	//追従処理
	void FollowTarget(void);
	//カメラ操作
	void InputFollowCamera(void);
	//ロックオン処理
	void LockOn(void);
	// ロックオンによる回転処理
	void LockOnRotation(void);
	//--------------------------------------

	//デバッグモードの処理------------------
	//操作
	void InputDebugCamera(void);
	//座標移動処理
	//注視点も同じく移動
	void MovePos(float valueX, float valueY);
	//--------------------------------------

};
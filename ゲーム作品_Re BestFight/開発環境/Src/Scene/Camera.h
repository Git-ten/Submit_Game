#pragma once
#include<memory>
#include<functional>
#include <DxLib.h>
#include "Common/Quaternion.h"
#include"Act/Common/Transform.h"
#include"../Utility/MyUtility.h"

class Camera
{

public:

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//定点カメラ
		FREE,			//フリーモード
		FOLLOW,			//追従モード
		JUST,			//ジャスト判定モード
		SKILL_MODE,		//キャラごとの必殺技、処理はキャラクラスで行う
		SPECIAL_ATTACK_MODE,//キャラごとのフィニッシュ攻撃、処理はキャラクラスで行う
		SYNCHRO,			//他のカメラと同期する
	};

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 50000.0f;

	// カメラの初期座標
	static constexpr Position3DF DEFAULT_CAMERA_POS = { 0.0f, 200.0f, -500.0f };

	// カメラ位置から注視点までの相対座標
	static constexpr Position3DF RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	//カメラの上方向
	static constexpr Dir3DF CAMERA_UP = {0.0f, 1.0f, 0.0f};

	//カメラの視野角
	static constexpr float CAMERA_FOV = 1.0472f;

	//フリー
	static constexpr float LIMIT_ANGLE_UP_X = 0.4444f;
	static constexpr float LIMIT_ANGLE_DOWN_X = 0.1666666f;
	static constexpr Position3DF IDEAL_FREE_POS = {0.0f, 200.0f, -600.0f};
	static constexpr Position3DF IDEAL_FREE_TARGET_POS = {0.0f, 50.0f, 0.0f};
	static constexpr float CAMERA_ANGLE_SPEED = 3.0f;

	//追従
	//ターゲットモデルとカメラの理想相対距離
	static constexpr Position3DF IDEAL_RELATIVE_POS = {0.0f, 300.0f, -600.0f};
	//一定の距離での理想相対距離(右寄り)
	static constexpr Position3DF IDEAL_RELATIVE_NEAR_POS_R = {600.0f, 200.0f, -600.0f};
	//一定の距離での理想相対距離(左寄り)
	static constexpr Position3DF IDEAL_RELATIVE_NEAR_POS_L = { -600.0f, 200.0f, -300.0f };
	//ターゲットモデルとカメラの視点の理想相対距離
	static constexpr Position3DF IDEAL_RELATIVE_TARGET_POS = {0.0f, 100.0f, 50.0f};
	//カメラの移動速度
	static constexpr float CAMERA_SPEED = 70.0f;
	//理想座標を変えるタイミングの距離
	static constexpr float TARGET_LOCKON_DIS = 1000.0f;

	//イベント
	//ジャスト
	static constexpr Position3DF IDEAL_JUST_POS = {450.0f, 100.0f, -400.0f};

	//コンストラクタ
	Camera(void);

	//デストラクタ
	~Camera(void);

	//初期化
	void Init(void);
	//描画
	//カメラ関数の都合上、描画と更新は同時に行う
	void DrawUpdate(void);

	// カメラを初期位置に戻す
	void SetDefault(void);

	//カメラを強制的に更新
	void CompCameraInfoUpdate(void);
	//モードごとのカメラを強制更新
	void CompModeUpdate(void);
	//カメラの情報を強制セット
	void CompSetCameraInfo(const Position3DF& pos, const Position3DF& target, const Quaternion& rot);

	//カメラ座標取得
	Position3DF GetPos(void) const;
	//視点座標の取得
	Position3DF GetTarget(void) const;
	//座標のセット
	void SetPos(const Position3DF& pos);
	//視点座標のセット
	void SetTarget(const Position3DF& target);
	//回転情報の取得
	Quaternion GetRot(void)const;
	//回転情報のセット
	void SetRot(const Quaternion& q);
	//デフォルト回転情報の取得
	Quaternion GetDefRot(void)const;

	//カメラのデフォルト速度をセット
	void SetDefaultSpeed(void);
	//カメラの速度をセット
	void SetSpeed(float speed);
	//カメラの速度を取得
	float GetSpeed(void)const;

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
	
	//現在のカメラモード
	MODE GetCameraMode(void)const;
	// カメラモードの変更
	void ChangeMode(MODE mode);
	//デフォルトカメラモードをセット
	void SetDefaultMode(MODE mode);
	//カメラモードをデフォルトにする
	void ChangeDefaultMode(void);

	//フリーモード処理----------------------
	//ターゲットをセット
	void SetCameraTarget(std::weak_ptr<Transform> target);
	//--------------------------------------

	//追従モードの処理----------------------
	//追従対象をセット
	void SetFollower(std::weak_ptr<Transform> Tatrans, std::weak_ptr<Transform> Lotrans);
	//------------------------------

	//特殊カメラモードの処理セット
	void SetSpecialCamera(std::function<void(void)>func);

private:

	//速度
	float speed_;

	//デフォルトカメラモード
	MODE def_mode_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	Position3DF pos_;

	// カメラの注視点
	Position3DF targetPos_;

	// カメラの上方向
	Dir3DF cameraUp_;

	// 更新関数
	std::function<void(void)>update_;

	// カメラの回転
	Quaternion rot_;
	//デフォルトの回転
	Quaternion def_rot_;

	//カメラの角度
	Rot3DF angle_;

	//追従対象
	std::weak_ptr<Transform> targetTrans_;
	//ロックオン対象
	std::weak_ptr<Transform> lockonTrans_;



	//内部でのみ使用する関数
	Dir3DF GetDir(const Dir3DF& vec)const;
	//何もしない関数
	void UpdateNone(void);

	//フリーモード処理----------------------
	//インプットカメラアングル
	void InputCameraAngle(void);
	//フリー更新
	void UpdateFree(void);
	//--------------------------------------

	//追従モードの処理----------------------
	//追従更新
	void UpdateFollow(void);
	//------------------------------

	//ジャストガードモードの処理-------------
	//更新
	void UpdateJustGurd(void);
	//---------------------------------------

};
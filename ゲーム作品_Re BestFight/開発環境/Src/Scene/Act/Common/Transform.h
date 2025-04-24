#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "../../../Utility/VectorHeader.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>
class Transform
{

public:

	// モデルのハンドルID
	int modelId;

	// 大きさ
	Size3DF scl;
	// 回転
	Rot3DF rot;
	// 位置
	Position3DF pos;
	Position3DF localPos;
	Position3DF prevPos;//前フレームでの位置

	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 回転
	Quaternion quaRot;

	// ローカル回転
	Quaternion quaRotLocal;
	//デフォルトローカル回転
	Quaternion def_quaRotLocal;

	// コンストラクタ
	Transform(void);
	Transform(int model);

	// デストラクタ
	~Transform(void);

	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	//メモリ開放
	void Release(void);

	//モデルハンドルIDのセット
	void SetModel(int modelHId);

	//モデルハンドルIDを取得
	int GetModel()const;

	// 前方方向を取得
	Dir3DF GetForward(void) const;

	// 後方方向を取得
	Dir3DF GetBack(void) const;

	// 右方向を取得
	Dir3DF GetRight(void) const;

	// 左方向を取得
	Dir3DF GetLeft(void) const;

	// 上方向を取得
	Dir3DF GetUp(void) const;

	// 下方向を取得
	Dir3DF GetDown(void) const;

	//ローカルな前方向
	Dir3DF GetForwardLocal(void)const;

	//ローカルな右方向
	Dir3DF GetRightLocal(void)const;

	//ローカルな左方向
	Dir3DF GetLeftLocal(void)const;

	//ローカルな上方向
	Dir3DF GetUpLocal(void)const;

	//ローカルな後ろ方向
	Dir3DF GetBackLocal(void)const;

private:

	// 対象方向を取得
	Dir3DF GetDir(const Dir3DF& vec) const;

	//ローカルな対象方向を取得
	Dir3DF GetDirLocal(const Dir3DF& vec)const;
};


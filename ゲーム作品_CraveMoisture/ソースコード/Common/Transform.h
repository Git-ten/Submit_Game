#pragma once
#include <string>
#include "DataLoad/DataModel.h"
#include "DataLoad/LoadData.h"
#include "../RENDERER/ModelRenderer/ModelRenderer.h"
#include "../APPLICATION/SINGLETON_MANAGER/ResourceManager/ResourceManager.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>
class Transform
{
public:

	// モデルタイプ
	enum class TRANSFORM_TYPE
	{
		NONE,
		SKIN_MODEL,
		NORMAL_MODEL,
		NOISE_MODEL,
		METAL_MODEL,
	};


	// 演出タイプ
	enum class EVENT_TYPE
	{
		NONE,
		RIMLIGHT,
		RIMLIGHT_SKIN,
	};

	// コンストラクタ
	Transform(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	// デストラクタ
	~Transform(void);

	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	/// <param name=""></param>
	
	// 更新前処理(しなくても問題なく動くが、prevPositionが正常でなくなる)
	void PrevUpdate(void);

	void Update(void);
	void Update(const int modelId, const std::string& frameName);
	void UpdateVertexPS(const int index, const FLOAT4& param);

	void Draw(void);

	// モデルIDの取得
	const int& GetModelId(void)const;

	// モデルIDの参照を取得
	int& GetModelIdReference(void);

	// 1フレーム前の座標を取得
	const Position3DF& GetPrevPos(void);

	// 座標を取得
	const Position3DF& GetPos(void)const;

	// 1フレーム前の座標の参照を取得
	Position3DF& GetPrevPosReference(void);

	// 座標を取得(参照)
	Position3DF& GetPosReference(void);

	// スケールを取得
	const VECTOR& GetScl(void)const;

	// 角度を取得(rad)
	const Rot3DF& GetRot(void)const;
	Rot3DF& GetRotReference(void);

	// 座標をセットする
	// 余程のことでないと使用しない
	void SetPos(const Position3DF& pos);

	// 座標を引数分動かす
	void MoveModelPos(const VECTOR& moveVec);
	// 角度を指定の方向に引数分回転
	void RotationModelQuaRot(const Dir3DF& dir);
	// 角度を指定の軸で引数分回転
	void RotationModelAngleAxisLerp(const Rot3DF& angleRot);
	void RotationModelAngleAxis(const Rot3DF& angleRot);

	
	// 頂点シェーダの初期化
	void InitVertexRenderer(const TRANSFORM_TYPE& type);


	// 指定したイベント系頂点シェーダの起動
	void StartEventVertex(const EVENT_TYPE& type);
	// イベント系シェーダの終了処理
	void EndEventVertex(void);

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

	// ライティングの色を調節
	void SetLightColor(const Color3DF& color);
	// ライティングの強さを調節
	void SetLightPower(const float power);

private:

	// 現在のイベントタイプ
	EVENT_TYPE nowType_;

	// 頂点シェーダ
	std::unique_ptr<ModelRenderer>vertexRenderer_;

	// イベント系シェーダ
	std::unique_ptr<ModelRenderer>eventVertexRenderer_;

	// モデルデータ
	DataModel modInfo_;

	//前フレームでの位置
	Position3DF prevPosition;

	//デフォルトローカル回転
	Quaternion def_quaRotLocal;

	//重力処理を行う
	//Dir3DF gravDir_;

	// 計算処理に使用
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 現在のイベント系頂点シェーダの更新
	void EventVertexUpdate(void);
	
	// 対象方向を取得
	Dir3DF GetDir(const Dir3DF& vec) const;
	//ローカルな対象方向を取得
	Dir3DF GetDirLocal(const Dir3DF& vec)const;

	// スキンメッシュのシェーダ
	void InitVertexSkin(void);
	// 通常モデルのシェーダ
	void InitVertexNormal(void);
	// ノイズモデルのシェーダ
	void InitVertexNoize(void);
	// メタルモデルのシェーダ
	void InitVertexMetal(void);

	// リムライトの初期化
	void InitRimLight(const ModelRenderer::SHADER_NAME& name);

	// リムライトの更新
	void UpdateRimLight(void);

	// メタルの更新
	void UpdateMetal(void);

};
#pragma once
#include "../../RENDERER/ModelRenderer/PoligonRenderer.h"
#include "PlayerWeaponBase.h"

class PlayerWeaponSword : public PlayerWeaponBase
{
public:

	// コンストラクタ
	PlayerWeaponSword(int& modelId);
	// デストラクタ
	~PlayerWeaponSword(void)override;

	void InitSpecial(void)override;

	// 軌跡の描画をオンにする
	void OnDrawLocus(void);
	// 軌跡の描画をオフにする
	void OffDrawLocus(void);

	//軌跡の更新
	void UpdateLocus(float maxAnimStep, int attachIdx);

protected:

	// 根本と先端
	struct PosBuf
	{
		Position3DF headPos;		//先端
		Position3DF tailPos;		//根本
		int alpha;					//透明度
	};

	//剣の根元と先端の座標
	std::vector<PosBuf>posBuf_;
	// シェーダ
	std::unique_ptr<PoligonRenderer>locusRenderer_;
	
	//軌跡のポリゴン
	std::vector<VERTEX3DSHADER>vertex_;
	//ポリゴンインデックス
	std::vector<WORD>indexes_;

	// 剣の軌跡を生成描画するか
	bool isSetLocus_;

	void InitActorTransform(void)override;

	// 現フレームの剣の位置を設定
	void SetLocusPos(const PosBuf& buf);

	// 剣の根本の座標
	const Position3DF GetTailPos(void)const;
	// 剣の先端の座標
	const Position3DF GetHeadPos(void)const;

	void UpdateSpecial(void)override;
	void DrawSpecial(void)override;

};
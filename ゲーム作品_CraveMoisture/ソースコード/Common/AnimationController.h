#pragma once
#include<map>
#include<string>
#include "../Common/DataLoad/DataAnimation.h"


class AnimationController
{
public:

	//コンストラクタ
	AnimationController(const int& model);
	//デストラクタ
	~AnimationController(void);

	//更新
	void Update(bool isSlow = false);


	//アニメーションの追加
	//使用するアニメーションをあらかじめ登録しておく
	void Add(int type, const DataAnimation& data);


	//最初に再生するアニメーション
	void Start(int type);


	//アニメーションの再生
	void Play(int type);
	//停止中のアニメーションの再生
	void RePlay(void);
	//再生中のアニメーションの停止
	void Stop(void);


	//登録したすべてのアニメーション連想配列をクリアする
	void ClearAnimMap(void);


	//アニメーションを再生しているか取得
	const bool IsPlayAnim(void)const;


	//現在再生しているアニメーションのステップタイム
	const float GetStepAnimTime(void)const;
	// 現在再生しているアニメーションのステップタイムの参照
	float& GetReferenceStepAnimTime(void);
	//アニメーションステップを引数のところまでスキップ
	void SkipAnimTime(float skipStep);
	// 現在再生しているアニメーションの終了ステップを取得
	const float GetEndStepAnimTime(void)const;
	// 現在再生しているアニメーションのアタッチ番号
	const int GetAttachAnimIndex(void)const;


	//ブレンドしているかの取得
	const bool IsBlendFlag(void)const;
	//ブレンド処理を強制終了
	void EndBlend(void);


	//再生速度をセット
	void SetPlaySpeed(float speed);
	// 再生速度を引数倍する
	void MultPlaySpeed(const float mul);
	//再生速度をデフォルトにする
	void DefaultPlaySpeed(void);

private:

	//アニメーションの情報
	struct ANIMATION_INFO
	{
		int nowType;
		std::string animationPath;	//アニメーションのパス
		int modelId;				//モデルID
		float speed;				//アニメーション速度
		int attachNo;				//アニメーションのアタッチ番号
		float totalTime;			//アニメーションの終了時間
		float step;					//現在のアニメーションステップ
		bool isLoop;				//アニメーションをループするか否か
		float start;				//アニメーションのスタートステップ
		float end;					//アニメーションの終了ステップ
	};

	//アニメーションデータ
	using AnimationsData = std::map<int, ANIMATION_INFO>;

private:

	//アニメーションを登録する
	AnimationsData animation_;


	//現在再生しているアニメーション
	ANIMATION_INFO nowAnimation_;
	//切り替え前に再生していたアニメーション
	ANIMATION_INFO prevAnimation_;


	//ブレンド処理中か
	bool blendFlag_;
	//ブレンド率
	float blendRate_;


	//アニメーションを再生しているか
	bool playAnimationFlag_;


	//アタッチする対象モデル
	const int& modelId_;


	//再生速度
	float animSpeed_;


	// ブレンド処理
	void BlendAnimation(void);
	// アニメーション処理
	void Animation(void);

	// アニメーション再生のセットアップ
	void SetUpPlayAnimation(int type);

};


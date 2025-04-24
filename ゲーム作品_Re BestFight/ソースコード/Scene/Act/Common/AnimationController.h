#pragma once
#include<map>
#include<string>

class AnimationController
{
public:

	//アニメーションの情報
	struct ANIM_INFO
	{
		int modelId = -1;
		float speed = 0.0f;
		int attachNo = -1;
		float totalTime = 0.0f;
		float step = 0.0f;
		bool loopFlag = false;

		float start = 0.0f;
		float end = 0.0f;
	};

	//ブレンドの進行速度
	static constexpr float BLEND_SPEED = 0.15f;
	
	//コンストラクタ
	AnimationController(int& model);

	//デストラクタ
	~AnimationController(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);

	//アニメーションの追加
	//使用するアニメーションをあらかじめ登録しておく
	void Add(int type, const std::string& line, float speed, float start, float end, bool loop);

	//最初に再生するアニメーション
	void Start(int type);

	//アニメーションの再生
	void Play(int type);
	//再生中のアニメーションの停止
	void Stop(void);

	void ClearAnimMap(void);

	//アニメーションを再生しているか取得
	bool IsPlayAnim(void)const;

	//現在再生しているアニメーションのステップタイム
	float GetStepAnimTime()const;

	//ブレンドしているかの取得
	bool IsBlendFlag(void)const;

private:

	using AnimationsData = std::map<int, ANIM_INFO>;

	//アニメーションを登録する
	AnimationsData animation_;

	//現在再生しているアニメーション
	ANIM_INFO nowPlayAnim_;
	//切り替え前に再生していたアニメーション
	ANIM_INFO prevPlayAnim_;

	//ブレンド処理中か
	bool blendFlag_;
	//ブレンド率
	float blendRate_;

	//アニメーションを再生しているか
	bool playAnimFlag_;

	//アタッチする対象モデル
	int& modelId_;

};


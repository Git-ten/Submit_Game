#pragma once
#include<memory>
#include "SceneBase.h"
#include"../Manager/SceneManager.h"

class Transform;
class AnimationController;
class ResultMenyu;
class Sound;

class ResultScene :public SceneBase
{
public:

	//現在のゲームシーンの状態
	enum class SCENE_STATE
	{
		RESULT,
		MENYU,
	};


	//コンストラクタ
	ResultScene(SceneManager::CHARA_ID winner, SceneManager::CHARA_ID loser);

	//デストラクタ
	~ResultScene(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//メモリ開放
	void Release(void)override;

private:

	//ピクセルシェーダの数
//最初が必ずメインスクリーンが入るので2以上にする
	static constexpr int PIXEL_NUM = 2;

	//リザルト画像のハンドル番号を格納
	int imgResultHandle_;

	//勝者
	SceneManager::CHARA_ID winnerId_;
	std::unique_ptr<Transform>winner_;
	std::unique_ptr<AnimationController>winnerAnim_;

	//敗者
	SceneManager::CHARA_ID loserId_;
	std::unique_ptr<Transform>loser_;
	std::unique_ptr<AnimationController>loserAnim_;

	std::unique_ptr<Camera>camera_;

	//メニューシーン
	std::unique_ptr<ResultMenyu>menyu_;

	using UpdateFunc = std::function<void(void)>;
	using DrawFunc = std::function<void(void)>;
	//更新関数ポインタ
	UpdateFunc update_;
	//描画関数ポインタ
	DrawFunc draw_;
	//現在の状態
	SCENE_STATE nowState_;
	//決定音
	std::weak_ptr<Sound> decisionSod_;
	//リザルト音(一度しか鳴らない)
	std::weak_ptr<Sound>resultSod_;



	//内部でしか使わない関数
	//キャラモデルの初期化
	void InitCharactor(void);
	//キャラアニメの初期化
	void InitCharaAnim(void);
	//状態の変化
	void ChangeState(const SCENE_STATE& stat);
	//リザルトの更新
	void UpdateResult(void);
	//リザルトメニューの更新
	void UpdateMenyu(void);
	//リザルト画像を描画
	void DrawResult(void);
	//シーン全体の描画
	void DrawResultScene(void);
	//リザルトメニューの描画
	void DrawMenyu(void);

};
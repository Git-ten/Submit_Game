#pragma once
#include <map>
#include <chrono>
#include<memory>

class SceneBase;
class Fade;
class Camera;
class Transform;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,//タイトル
		GAME,//ゲーム
		SELECT,//キャラセレクト
		RESULT,//リザルト
		DEMO,//デモ
		ABOUT,//ゲーム説明
	};
	
	//キャラクターID
	enum class CHARA_ID
	{
		NONE,
		FIGHTER_ID,//格闘家ID
		SORDMAN_ID,//剣士ID
		MAGICIAN_ID,//魔導士ID
		MAX,
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	//初期化
	void Init(void);
	void Init3D(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(const SCENE_ID& nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void)const;

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	//プレイヤーと敵のセット
	void SetPlayerAndEnemy(CHARA_ID player, CHARA_ID enemy);

	//CPUモードのセット
	void SetCpuFlag(bool flg);

	//CPUモードの取得
	bool GetCpuFlag(void)const;

	//スクリーンの取得
	int GetScreen(void)const;

	//画面の振動数をセット
	void SetVibrationCnt(int count);

private:

	// 静的インスタンス
	static SceneManager* instance_;

	//現在のシーンID
	SCENE_ID sceneId_;

	// 各種シーン
	std::unique_ptr<SceneBase>scene_;

	//フェイド
	std::unique_ptr<Fade>fader_;

	//プレイヤー
	CHARA_ID playerID_;
	//敵
	CHARA_ID enemyID_;

	// シーン遷移中判定
	bool isSceneChanging_;

	//CPUと戦うか
	bool cpuFlag_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//スクリーンの振動数
	int vibrationCnt_;

	//メインスクリーン
	int mainScreen_;



	//内部でのみ使用する関数
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager(void);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);
	// シーン遷移
	void DoChangeScene(const SCENE_ID& sceneId);
	//フェード処理(手動)
	void FadeManual(void);
	//キャラを2体をランダムで選ぶ
	void RandomChara(CHARA_ID* chara_1, CHARA_ID* chara_2);
	//メインスクリーンの描画
	void DrawMainScreen(void);

};
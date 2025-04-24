#pragma once
#include<string>

#define DEBUG_STRING(line) (Debuger::GetInstance().DrawDebugString(line))
#define DEBUG_FVALUE_1(line, v1) (Debuger::GetInstance().DrawDebugValueStringF(line, v1))
#define DEBUG_FVALUE_2(line, v1, v2) (Debuger::GetInstance().DrawDebugValueStringF(line, v1, v2))
#define DEBUG_FVALUE_3(line, v1, v2, v3) (Debuger::GetInstance().DrawDebugValueStringF(line, v1, v2, v3))
#define DEBUG_DVALUE_1(line, v1) (Debuger::GetInstance().DrawDebugValueStringD(line, v1))
#define DEBUG_DVALUE_2(line, v1, v2) (Debuger::GetInstance().DrawDebugValueStringD(line, v1, v2))
#define DEBUG_DVALUE_3(line, v1, v2, v3) (Debuger::GetInstance().DrawDebugValueStringD(line, v1, v2, v3))

class Debuger
{
public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Debuger& GetInstance(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	// リソースの破棄
	void Destroy(void);

	//デバッグ用の文字列表示
	void DrawDebugString(std::string line);
	//デバッグ用の値表示
	void DrawDebugValueStringF(std::string line, float v1);
	void DrawDebugValueStringD(std::string line, int v1);
	void DrawDebugValueStringF(std::string line, float v1, float v2);
	void DrawDebugValueStringD(std::string line, int v1, int v2);
	void DrawDebugValueStringF(std::string line, float v1, float v2, float v3);
	void DrawDebugValueStringD(std::string line, int v1, int v2, int v3);

private:

	//シングルトン参照
	static Debuger* instance_;

	//文字列の数を記録する
	int lineCounter;

	//内部的に宣言することで他クラスからアクセスさせない
	//コンストラクタ
	Debuger(void);
	//コピーコンストラクタも同様
	Debuger(const Debuger&);
	//デストラクタ
	~Debuger(void) = default;

};


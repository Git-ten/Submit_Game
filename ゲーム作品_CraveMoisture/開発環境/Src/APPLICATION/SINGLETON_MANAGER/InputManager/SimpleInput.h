#pragma once
#include <map>
#include <functional>
#include <string>

class SimpleInput
{
public:

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SimpleInput& GetInstance(void);

	// リソースの破棄
	void Destroy(void);

	// 判定を行うキーを追加
	void Add(const std::string& keyName, const std::function<bool(void)> keyFunc);

	// 指定した名称のキーの入力状態を取得
	const bool GetInput(const std::string& keyName)const;

private:

	using SIMPLE_KEY = std::map<std::string, std::function<bool(void)>>;

private:

	// シングルトン用インスタンス
	static SimpleInput* instance_;

	// 登録キー
	SIMPLE_KEY input_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SimpleInput(void);
	// コピーコンストラクタも同様
	SimpleInput(const SimpleInput&);
	//デストラクタ
	~SimpleInput(void) = default;

};
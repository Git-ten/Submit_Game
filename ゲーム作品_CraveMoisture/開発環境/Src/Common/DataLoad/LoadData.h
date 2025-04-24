#pragma once
#include <string>
#include <fstream>
#include "../../Common/Quaternion.h"
#include "../../Utility/VectorHeader.h"
#include "../../Utility/JsonLib/nlohmann/json.hpp"

class LoadData
{
public:

	// ロード用の情報
	struct LoadDataInfo
	{
		std::string fileName;
		std::string dataName;
	};

	//コンストラクタ
	LoadData(void);
	//デストラクタ
	~LoadData(void);

	// ロードのスタート処理
	void LoadStart(const std::string& dataPathName);
	// ロードの終了処理
	void LoadEnd(void);

	// 文字列のロード
	void LoadedString(const std::string& readDataName,
		const std::string& name, std::string& line);

	// 複数文字列動的配列のロード
	void LoadedStringVector(const std::string& readDataName,
		const std::string& name, std::vector<std::string>& lines);

	// Vector2型パラメータのロード
	void LoadedVector2(const std::string& readDataName,
		const std::string& name, Vector2& vec2);

	// Vector3型のパラメータのロード
	void LoadedVector3(const std::string& readDataName,
		const std::string& name, Vector3D& vec3);

	// Vector3F型のパラメータのロード
	void LoadedVector3F(const std::string& readDataName,
		const std::string& name, VECTOR& vec3);

	// double型パラメータのロード
	void LoadedDouble(const std::string& readDataName,
		const std::string& name, double& param);

	// float型パラメータのロード
	void LoadedFloat(const std::string& readDataName,
		const std::string& name, float& param);
	// 複数float型動的配列のロード
	void LoadedFloatVector(const std::string& readDataName,
		const std::string& name, std::vector<float>& params);

	// int型パラメータのロード
	void LoadedInt(const std::string& readDataName,
		const std::string& name, int& param);
	// 複数int型パラメータのロード
	void LoadedIntVector(const std::string& readDataName,
		const std::string& name, std::vector<int>& params);

	// bool型パラメータのロード
	void LoadedBool(const std::string& readDataName,
		const std::string& name, bool& flag);

	// Quaternion型パラメータのロード
	void LoadedQuaternion(const std::string& readDataName, const std::string& name, Quaternion& quaternion);


private:

	std::ifstream dataStream_;
	// ファイルストリームからjsonオブジェクトに変換
	nlohmann::json readData_;


	//データ名が存在するかチェック
	void CheckReadData(const std::string& name);

	void ReadData(const std::string& readDataName, const std::string& name, std::vector<std::string>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<int>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<double>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<bool>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<float>& readVector);

};

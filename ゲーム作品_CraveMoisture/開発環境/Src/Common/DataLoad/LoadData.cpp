#include <cassert>
#include "LoadData.h"

LoadData::LoadData(void)
{
}

LoadData::~LoadData()
{
}

void LoadData::LoadStart(const std::string& dataPathName)
{
	dataStream_.open(dataPathName);
	assert(dataStream_ || !"ファイルの読み込みに失敗");

	// ファイルストリームからjsonオブジェクトに変換
	readData_ = nlohmann::json::parse(dataStream_);
}

void LoadData::LoadEnd(void)
{
	dataStream_.close();
}

void LoadData::LoadedString(const std::string& readDataName,
	const std::string& name, std::string& line)
{
	std::vector<std::string>rPath;
	ReadData(readDataName, name, rPath);
	line = rPath[0];
}

void LoadData::LoadedStringVector(const std::string& readDataName,
	const std::string& name, std::vector<std::string>& lines)
{
	ReadData(readDataName, name, lines);
}

void LoadData::LoadedVector2(const std::string& readDataName,
	const std::string& name, Vector2& vec2)
{
	std::vector<int>rInt;
	ReadData(readDataName, name, rInt);
	vec2.x = rInt[0];
	vec2.y = rInt[1];
}

void LoadData::LoadedVector3(const std::string& readDataName,
	const std::string& name, Vector3D& vec3)
{
	std::vector<int>rInt;
	ReadData(readDataName, name, rInt);
	vec3.x = rInt[0];
	vec3.y = rInt[1];
	vec3.z = rInt[2];
}

void LoadData::LoadedVector3F(const std::string& readDataName,
	const std::string& name, VECTOR& vec3)
{
	std::vector<float>rFloat;
	ReadData(readDataName, name, rFloat);
	vec3.x = rFloat[0];
	vec3.y = rFloat[1];
	vec3.z = rFloat[2];
}

void LoadData::LoadedDouble(const std::string& readDataName,
	const std::string& name, double& param)
{
	std::vector<double>rParam;
	ReadData(readDataName, name, rParam);
	param = rParam[0];
}

void LoadData::LoadedFloat(const std::string& readDataName,
	const std::string& name, float& param)
{
	std::vector<float>rParam;
	ReadData(readDataName, name, rParam);
	param = rParam[0];
}

void LoadData::LoadedFloatVector(const std::string& readDataName,
	const std::string& name, std::vector<float>& params)
{
	ReadData(readDataName, name, params);
}

void LoadData::LoadedInt(const std::string& readDataName, const std::string& name, int& param)
{
	std::vector<int>rParam;
	ReadData(readDataName, name, rParam);
	param = rParam[0];
}

void LoadData::LoadedIntVector(const std::string& readDataName,
	const std::string& name, std::vector<int>& params)
{
	ReadData(readDataName, name, params);
}

void LoadData::LoadedBool(const std::string& readDataName,
	const std::string& name, bool& flag)
{
	std::vector<bool>rBool;
	ReadData(readDataName, name, rBool);
	flag = rBool[0];
}

void LoadData::LoadedQuaternion(const std::string& readDataName,
	const std::string& name, Quaternion& quaternion)
{
	std::vector<float>rFloat;
	ReadData(readDataName, name, rFloat);
	quaternion.w = rFloat[0];
	quaternion.x = rFloat[1];
	quaternion.y = rFloat[2];
	quaternion.z = rFloat[3];
}

void LoadData::CheckReadData(const std::string& name)
{
	const auto& ite = readData_.find(name);
	assert(ite != readData_.end() || !"ロードしたデータファイルに名前がありません");
}

void LoadData::ReadData(const std::string& readDataName, const std::string& name, std::vector<std::string>& readVector)
{
	CheckReadData(readDataName);

	const auto& readData = readData_[readDataName];
	const auto& readLine = readData[name];

	for (const auto& r : readLine)
	{
		readVector.push_back(r);
	}
}

void LoadData::ReadData(const std::string& readDataName, const std::string& name, std::vector<int>& readVector)
{
	CheckReadData(readDataName);

	const auto& readData = readData_[readDataName];
	const auto& readLine = readData[name];

	for (const auto& r : readLine)
	{
		readVector.push_back(r);
	}
}

void LoadData::ReadData(const std::string& readDataName, const std::string& name, std::vector<double>& readVector)
{
	CheckReadData(readDataName);

	const auto& readData = readData_[readDataName];
	const auto& readLine = readData[name];

	for (const auto& r : readLine)
	{
		readVector.push_back(r);
	}
}

void LoadData::ReadData(const std::string& readDataName, const std::string& name, std::vector<bool>& readVector)
{
	CheckReadData(readDataName);

	const auto& readData = readData_[readDataName];
	const auto& readLine = readData[name];

	for (const auto& r : readLine)
	{
		readVector.push_back(r);
	}
}

void LoadData::ReadData(const std::string& readDataName, const std::string& name, std::vector<float>& readVector)
{
	CheckReadData(readDataName);

	const auto& readData = readData_[readDataName];
	const auto& readLine = readData[name];

	for (const auto& r : readLine)
	{
		readVector.push_back(r);
	}
}
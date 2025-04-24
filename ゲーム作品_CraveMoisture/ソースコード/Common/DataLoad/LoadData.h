#pragma once
#include <string>
#include <fstream>
#include "../../Common/Quaternion.h"
#include "../../Utility/VectorHeader.h"
#include "../../Utility/JsonLib/nlohmann/json.hpp"

class LoadData
{
public:

	// ���[�h�p�̏��
	struct LoadDataInfo
	{
		std::string fileName;
		std::string dataName;
	};

	//�R���X�g���N�^
	LoadData(void);
	//�f�X�g���N�^
	~LoadData(void);

	// ���[�h�̃X�^�[�g����
	void LoadStart(const std::string& dataPathName);
	// ���[�h�̏I������
	void LoadEnd(void);

	// ������̃��[�h
	void LoadedString(const std::string& readDataName,
		const std::string& name, std::string& line);

	// ���������񓮓I�z��̃��[�h
	void LoadedStringVector(const std::string& readDataName,
		const std::string& name, std::vector<std::string>& lines);

	// Vector2�^�p�����[�^�̃��[�h
	void LoadedVector2(const std::string& readDataName,
		const std::string& name, Vector2& vec2);

	// Vector3�^�̃p�����[�^�̃��[�h
	void LoadedVector3(const std::string& readDataName,
		const std::string& name, Vector3D& vec3);

	// Vector3F�^�̃p�����[�^�̃��[�h
	void LoadedVector3F(const std::string& readDataName,
		const std::string& name, VECTOR& vec3);

	// double�^�p�����[�^�̃��[�h
	void LoadedDouble(const std::string& readDataName,
		const std::string& name, double& param);

	// float�^�p�����[�^�̃��[�h
	void LoadedFloat(const std::string& readDataName,
		const std::string& name, float& param);
	// ����float�^���I�z��̃��[�h
	void LoadedFloatVector(const std::string& readDataName,
		const std::string& name, std::vector<float>& params);

	// int�^�p�����[�^�̃��[�h
	void LoadedInt(const std::string& readDataName,
		const std::string& name, int& param);
	// ����int�^�p�����[�^�̃��[�h
	void LoadedIntVector(const std::string& readDataName,
		const std::string& name, std::vector<int>& params);

	// bool�^�p�����[�^�̃��[�h
	void LoadedBool(const std::string& readDataName,
		const std::string& name, bool& flag);

	// Quaternion�^�p�����[�^�̃��[�h
	void LoadedQuaternion(const std::string& readDataName, const std::string& name, Quaternion& quaternion);


private:

	std::ifstream dataStream_;
	// �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
	nlohmann::json readData_;


	//�f�[�^�������݂��邩�`�F�b�N
	void CheckReadData(const std::string& name);

	void ReadData(const std::string& readDataName, const std::string& name, std::vector<std::string>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<int>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<double>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<bool>& readVector);
	void ReadData(const std::string& readDataName, const std::string& name, std::vector<float>& readVector);

};

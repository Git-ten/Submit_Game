#pragma once
#include <string>

class DataPaths
{
public:

	//�R���X�g���N�^
	DataPaths(
		const std::string& path_Json,
		const std::string& path_Image,
		const std::string& path_Model,
		const std::string& path_Movie,
		const std::string& path_Effect,
		const std::string& path_Music,
		const std::string& path_Wave,
		const std::string& path_ShaderVertex) :

		PATH_JSON(path_Json),
		PATH_IMAGE(path_Image),
		PATH_MOVIE(path_Movie),
		PATH_MODEL(path_Model),
		PATH_EFFECT(path_Effect),
		PATH_MUSIC(path_Music),
		PATH_WAVE(path_Wave),
		PATH_SHADER_VERTEX(path_ShaderVertex)
	{
	}

	//�f�X�g���N�^
	~DataPaths(void) = default;

	// �f�[�^�p�X��-----------------
	const std::string PATH_JSON;			// JSON�t�@�C���p�X
	const std::string PATH_IMAGE;			// Image�t�@�C���p�X
	const std::string PATH_MOVIE;			// Movie�t�@�C���p�X
	const std::string PATH_MODEL;			// Model�t�@�C���p�X
	const std::string PATH_EFFECT;			// Effect�t�@�C���p�X
	const std::string PATH_MUSIC;			// Music�t�@�C���p�X
	const std::string PATH_WAVE;			// Wave�t�@�C���p�X
	const std::string PATH_SHADER_VERTEX;	// Shader�t�@�C���p�X
	// -----------------------------

};
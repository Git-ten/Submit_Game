#pragma once
#include <string>

class DataPaths
{
public:

	//コンストラクタ
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

	//デストラクタ
	~DataPaths(void) = default;

	// データパス名-----------------
	const std::string PATH_JSON;			// JSONファイルパス
	const std::string PATH_IMAGE;			// Imageファイルパス
	const std::string PATH_MOVIE;			// Movieファイルパス
	const std::string PATH_MODEL;			// Modelファイルパス
	const std::string PATH_EFFECT;			// Effectファイルパス
	const std::string PATH_MUSIC;			// Musicファイルパス
	const std::string PATH_WAVE;			// Waveファイルパス
	const std::string PATH_SHADER_VERTEX;	// Shaderファイルパス
	// -----------------------------

};
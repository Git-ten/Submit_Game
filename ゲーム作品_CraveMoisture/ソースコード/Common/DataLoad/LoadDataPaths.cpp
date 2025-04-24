#include "DataPaths.h"
#include "LoadData.h"
#include "LoadDataPaths.h"

DataPaths LoadDataPaths::LoadedDataPaths(void)
{
	LoadData loadData;

	const std::string PATH_JSON_NAME = "Json";
	const std::string DATA_PATH_NAME = "Data/" + PATH_JSON_NAME + "/DataPath.json";
	const std::string PATH_IMAGE_NAME = "Image";
	const std::string PATH_MODEL_NAME = "Model";
	const std::string PATH_MOVIE_NAME = "Movie";
	const std::string PATH_EFFECT_NAME = "Effect";
	const std::string PATH_MUSIC_NAME = "SoundBgm";
	const std::string PATH_WAVE_NAME = "SoundWave";
	const std::string PATH_SHADER_VERTEX_NAME = "ShaderVertex";

	const std::string PATH_NAME = "path";

	loadData.LoadStart(DATA_PATH_NAME);

	std::string jsonLine;
	std::string imgLine;
	std::string modLine;
	std::string movieLine;
	std::string effLine;
	std::string musicLine;
	std::string waveLine;
	std::string resourceLine;
	std::string shaderVertexLine;

	loadData.LoadedString(PATH_JSON_NAME, PATH_NAME, jsonLine);
	loadData.LoadedString(PATH_IMAGE_NAME, PATH_NAME, imgLine);
	loadData.LoadedString(PATH_MODEL_NAME, PATH_NAME, modLine);
	loadData.LoadedString(PATH_MOVIE_NAME, PATH_NAME, movieLine);
	loadData.LoadedString(PATH_EFFECT_NAME, PATH_NAME, effLine);
	loadData.LoadedString(PATH_MUSIC_NAME, PATH_NAME, musicLine);
	loadData.LoadedString(PATH_WAVE_NAME, PATH_NAME, waveLine);
	loadData.LoadedString(PATH_SHADER_VERTEX_NAME, PATH_NAME, shaderVertexLine);

	DataPaths paths = DataPaths(
							jsonLine,
							imgLine,
							modLine,
							movieLine,
							effLine,
							musicLine,
							waveLine,
							shaderVertexLine);

	loadData.LoadEnd();

	return paths;
}
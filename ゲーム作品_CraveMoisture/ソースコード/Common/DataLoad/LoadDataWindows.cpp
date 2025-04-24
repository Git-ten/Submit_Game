#include <cassert>
#include "DataWindows.h"
#include "DataPaths.h"
#include "LoadData.h"
#include "LoadDataWindows.h"

DataWindows LoadDataWindows::LoadedDataWindows(const DataPaths& dataPath)
{
	LoadData loadData;
	DataWindows windows = DataWindows();

	const std::string WINDOWS_DATA_NAME = dataPath.PATH_JSON + "WindowsInfo.json";
	const std::string WINDOWS_SCREEN_SIZE_NAME = "screenSize";
	const std::string WINDOWS_SCREEN_COLOR_NAME = "screenColor";
	const std::string WINDOWS_FPS_NAME = "FPS";
	const std::string WINDOWS_MODE_NAME = "windowMode";
	const std::string WINDOWS_MOUSE_MODEE_NAME = "mouseMode";

	const std::string SIZE_NAME = "size";
	const std::string COLOR_NAME = "color";
	const std::string FPS_FRAME_RATE_NAME = "frameRate";
	const std::string FLAG_NAME = "flag";

	loadData.LoadStart(WINDOWS_DATA_NAME);

	loadData.LoadedVector2(WINDOWS_SCREEN_SIZE_NAME, SIZE_NAME,windows.screenSize);
	loadData.LoadedVector3(WINDOWS_SCREEN_COLOR_NAME, COLOR_NAME, windows.screenColor);
	loadData.LoadedFloat(WINDOWS_FPS_NAME, FPS_FRAME_RATE_NAME, windows.frameFps);
	loadData.LoadedBool(WINDOWS_MODE_NAME, FLAG_NAME, windows.isWindowMode);
	loadData.LoadedBool(WINDOWS_MOUSE_MODEE_NAME, FLAG_NAME, windows.isMouseShow);

	loadData.LoadEnd();

	return windows;
}
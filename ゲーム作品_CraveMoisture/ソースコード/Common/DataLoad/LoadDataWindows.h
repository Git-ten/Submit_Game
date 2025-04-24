#pragma once

class DataWindows;
class DataPaths;

class LoadDataWindows
{
public:

	//Windowsデータのロード
	static DataWindows LoadedDataWindows(const DataPaths& dataPath);

};
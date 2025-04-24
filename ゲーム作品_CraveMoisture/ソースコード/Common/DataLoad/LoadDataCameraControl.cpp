#include <string>
#include "DataPaths.h"
#include "DataCameraControl.h"
#include "LoadDataCameraControl.h"

DataCameraControl LoadDataCameraControl::LoadedDataCameraControl(const DataPaths& dataPath, const LoadData::LoadDataInfo& info)
{
	LoadData loadData;
	DataCameraControl controlData;

	// ƒpƒX
	const std::string CAMERA_CONTROL_DATA_NAME = dataPath.PATH_JSON + info.fileName;

	const std::string CAMERA_CONTROL_DISNTANCE = "distance";
	const std::string CAMERA_CONTROL_SPEED = "speed";
	const std::string CAMERA_CONTROL_TARGETPOS = "relativeTarget";
	const std::string CAMERA_CONTROL_CAMERA_ANGLE = "cameraAngle";
	const std::string CAMERA_CONTROL_EVENT_NAME = "eventName";
	const std::string CAMERA_CONTROL_EVENT_TIME = "eventTime";

	loadData.LoadStart(CAMERA_CONTROL_DATA_NAME);

	loadData.LoadedFloat(info.dataName, CAMERA_CONTROL_DISNTANCE, controlData.distance);
	loadData.LoadedFloat(info.dataName, CAMERA_CONTROL_SPEED, controlData.speed);
	loadData.LoadedVector3F(info.dataName, CAMERA_CONTROL_TARGETPOS, controlData.targetPos);
	loadData.LoadedVector3F(info.dataName, CAMERA_CONTROL_CAMERA_ANGLE, controlData.angleRot);
	loadData.LoadedString(info.dataName, CAMERA_CONTROL_EVENT_NAME, controlData.eventName);
	loadData.LoadedFloat(info.dataName, CAMERA_CONTROL_EVENT_TIME, controlData.eventTime);

	loadData.LoadEnd();

	return controlData;
}
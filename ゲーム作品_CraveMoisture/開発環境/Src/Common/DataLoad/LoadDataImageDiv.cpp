#include <string>
#include "DataImageDiv.h"
#include "DataPaths.h"
#include "LoadDataImageDiv.h"

DataImageDiv LoadDataImageDiv::LoadedDataImageDiv(const DataPaths& dataPath, const ResourceManager::RESOURCE_ID& id, const LoadData::LoadDataInfo& info)
{
    DataImageDiv imageDiv;

    imageDiv.imgDataId = ResourceManager::GetInstance().Load(id).handleIds_;
    imageDiv.imgPlane = Plane2D();

    LoadDataImageInfo(dataPath, imageDiv, info);

    return imageDiv;
}

DataImageDiv LoadDataImageDiv::LoadedDataImageDiv(const DataPaths& dataPath, const EffectResourceManager::EFFECT_RESOURCE_ID& id, const LoadData::LoadDataInfo& info)
{
    DataImageDiv imageDiv;

    imageDiv.imgDataId = EffectResourceManager::GetInstance().Load(id).handleIds_;
    imageDiv.imgPlane = Plane2D();

    LoadDataImageInfo(dataPath, imageDiv, info);

    return imageDiv;
}

void LoadDataImageDiv::LoadDataImageInfo(const DataPaths& dataPath, DataImageDiv& imageDiv, const LoadData::LoadDataInfo& info)
{
    LoadData loadData;

    // ƒpƒX
    const std::string IMAGE_DATA_NAME = dataPath.PATH_JSON + info.fileName;

    const std::string IMAGE_POS_LEFT_UP_NAME = "posLeftUp";
    const std::string IMAGE_POS_RIGHT_DOWN_NAME = "posRightDown";
    const std::string IMAGE_CENTER_NAME = "center";
    const std::string IMAGE_SCL_RATE_NAME = "exRate";
    const std::string IMAGE_ANGLE_NAME = "angle";

    loadData.LoadStart(IMAGE_DATA_NAME);
    loadData.LoadedVector2(info.dataName, IMAGE_POS_LEFT_UP_NAME, imageDiv.posLeftUp);
    loadData.LoadedVector2(info.dataName, IMAGE_POS_RIGHT_DOWN_NAME, imageDiv.posRightDown);
    loadData.LoadedVector2(info.dataName, IMAGE_CENTER_NAME, imageDiv.imgCenter);
    loadData.LoadedDouble(info.dataName, IMAGE_SCL_RATE_NAME, imageDiv.imgScl);
    loadData.LoadedDouble(info.dataName, IMAGE_ANGLE_NAME, imageDiv.imgAngle);

    loadData.LoadEnd();

    imageDiv.imgPlane.leftUpPos = imageDiv.posLeftUp;
    imageDiv.imgPlane.rightDownPos = imageDiv.posRightDown;

}

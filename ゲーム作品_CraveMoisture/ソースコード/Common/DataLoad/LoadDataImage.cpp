#include <string>
#include "DataImage.h"
#include "DataPaths.h"
#include "LoadDataImage.h"

DataImage LoadDataImage::LoadedDataImage(const DataPaths& dataPath,
    const ResourceManager::RESOURCE_ID& id, 
    const LoadData::LoadDataInfo& info)
{
    LoadData loadData;
    DataImage image = DataImage();

    if (id != ResourceManager::RESOURCE_ID::NONE)
    {
        image.imgDataId = ResourceManager::GetInstance().Load(id).handleId_;
    }
    else
    {
        image.imgDataId = -1;
    }

    image.imgPlane = Plane2D();

    // ƒpƒX
    const std::string IMAGE_DATA_NAME = dataPath.PATH_JSON + info.fileName;

    const std::string IMAGE_POS_LEFT_UP_NAME = "posLeftUp";
    const std::string IMAGE_POS_RIGHT_DOWN_NAME = "posRightDown";
    const std::string IMAGE_CENTER_NAME = "center";
    const std::string IMAGE_SCL_RATE_NAME = "exRate";
    const std::string IMAGE_ANGLE_NAME = "angle";

    loadData.LoadStart(IMAGE_DATA_NAME);
    loadData.LoadedVector2(info.dataName, IMAGE_POS_LEFT_UP_NAME, image.posLeftUp);
    loadData.LoadedVector2(info.dataName, IMAGE_POS_RIGHT_DOWN_NAME, image.posRightDown);
    loadData.LoadedVector2(info.dataName, IMAGE_CENTER_NAME, image.imgCenter);
    loadData.LoadedDouble(info.dataName, IMAGE_SCL_RATE_NAME, image.imgScl);
    loadData.LoadedDouble(info.dataName, IMAGE_ANGLE_NAME, image.imgAngle);

    loadData.LoadEnd();

    image.imgPlane.leftUpPos = image.posLeftUp;
    image.imgPlane.rightDownPos = image.posRightDown;

    return image;
}

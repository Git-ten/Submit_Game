#include <string>
#include "DataAnimation.h"
#include "DataPaths.h"
#include "LoadDataAnimation.h"

DataAnimation LoadDataAnimation::LoadedDataAnimation(const DataPaths& dataPath, const LoadData::LoadDataInfo& info)
{
    LoadData loadData;
    DataAnimation animation;

    // ƒpƒX
    const std::string ANIMATION_DATA_NAME = dataPath.PATH_JSON + info.fileName;

    const std::string ANIMATION_PATH_NAME = "path";
    const std::string ANIMATION_SPEED_NAME = "animationSpeed";
    const std::string ANIMATION_START_NAME = "startStep";
    const std::string ANIMATION_END_NAME = "endStep";
    const std::string ANIMATION_LOOP_NAME = "isLoop";


    loadData.LoadStart(ANIMATION_DATA_NAME);
    loadData.LoadedString(info.dataName, ANIMATION_PATH_NAME, animation.path);
    animation.path = dataPath.PATH_MODEL + animation.path;


    loadData.LoadedFloat(info.dataName, ANIMATION_SPEED_NAME, animation.speed);
    loadData.LoadedFloat(info.dataName, ANIMATION_START_NAME, animation.start);
    loadData.LoadedFloat(info.dataName, ANIMATION_END_NAME, animation.end);
    loadData.LoadedBool(info.dataName, ANIMATION_LOOP_NAME, animation.isLoop);

    loadData.LoadEnd();

    return animation;
}

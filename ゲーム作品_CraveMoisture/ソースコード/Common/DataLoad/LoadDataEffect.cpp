#include <string>
#include "DataEffect.h"
#include "DataPaths.h"
#include "LoadDataEffect.h"

DataEffect LoadDataEffect::LoadedDataEffect(const DataPaths& dataPath, const LoadData::LoadDataInfo& info)
{
    LoadData loadData;
    DataEffect effect;

    // ƒpƒX
    const std::string ANIMATION_DATA_NAME = dataPath.PATH_JSON + info.fileName;

    const std::string EFFECT_SIZE_NAME = "scl";
    const std::string EFFECT_SPEED_NAME = "speed";


    loadData.LoadStart(ANIMATION_DATA_NAME);
    loadData.LoadedVector3F(info.dataName, EFFECT_SIZE_NAME, effect.scl);
    loadData.LoadedFloat(info.dataName, EFFECT_SPEED_NAME, effect.speed);

    loadData.LoadEnd();

    return effect;
}

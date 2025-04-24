#include <string>
#include "DataModel.h"
#include "DataPaths.h"
#include "LoadDataModel.h"

DataModel LoadDataModel::LoadedDataModel(const DataPaths& dataPath,
	const ResourceManager::RESOURCE_ID& id,
	const LoadData::LoadDataInfo& info)
{
	LoadData loadData;
	DataModel model = DataModel();

    if (id != ResourceManager::RESOURCE_ID::NONE)
    {
        // ���O��SetUseASyncLoadFlag(true);���Ăяo���Ă���ꍇ�̂ݔ񓯊�
        while (CheckHandleASyncLoad(model.modelId))
        {
            model.modelId = ResourceManager::GetInstance().LoadModelDuplicate(id);
        }
    }
    else
    {
        model.modelId = -1;
    }
   
    // ���̃f�[�^�������Ă��Ȃ��Ȃ炱��ȏ㏈�����Ȃ�
    if (info.fileName == "" &&
        info.dataName == "")
    {
        return model;
    }

    // �p�X
    const std::string IMAGE_DATA_NAME = dataPath.PATH_JSON + info.fileName;

    const std::string MODEL_POS_NAME = "pos";
    const std::string MODEL_QUAROT_NAME = "quaRot";
    const std::string MODEL_QUAROT_LOCAL_NAME = "quaRotLocal";
    const std::string MODEL_ROT_NAME = "rot";
    const std::string MODEL_SCL_NAME = "scl";

    loadData.LoadStart(IMAGE_DATA_NAME);
    loadData.LoadedVector3F(info.dataName, MODEL_POS_NAME, model.position);
    loadData.LoadedQuaternion(info.dataName, MODEL_QUAROT_NAME, model.quaRot);
    loadData.LoadedQuaternion(info.dataName, MODEL_QUAROT_LOCAL_NAME, model.quaRotLocal);
    loadData.LoadedVector3F(info.dataName, MODEL_ROT_NAME, model.rot);
    loadData.LoadedVector3F(info.dataName, MODEL_SCL_NAME, model.scl);

    loadData.LoadEnd();

	return model;
}

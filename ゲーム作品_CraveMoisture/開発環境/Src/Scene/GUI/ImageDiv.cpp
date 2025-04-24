#include <cassert>
#include "../../Common/DataLoad/LoadDataImageDiv.h"
#include "../../Application/Application.h"
#include "ImageDiv.h"

ImageDiv::ImageDiv(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id)
{
	imgInfos_ = LoadDataImageDiv::LoadedDataImageDiv(Application::GetInstance().GetDataPaths(), id, info);
}

ImageDiv::ImageDiv(const LoadData::LoadDataInfo& info, const EffectResourceManager::EFFECT_RESOURCE_ID& id)
{
	imgInfos_ = LoadDataImageDiv::LoadedDataImageDiv(Application::GetInstance().GetDataPaths(), id, info);
}

ImageDiv::~ImageDiv(void)
{
}

void ImageDiv::DrawImageDiv(const int number)
{
	assert(imgInfos_.imgDataId.size() > number || !"DrawImageDiv�Ŕ͈͊O�̃n���h��ID���w�肵�悤�Ƃ��܂���");

	DrawRotaGraph(imgInfos_.imgCenter.x, imgInfos_.imgCenter.y,
		imgInfos_.imgScl, imgInfos_.imgAngle, imgInfos_.imgDataId[number], true);
}

void ImageDiv::DrawImageDiv(const int number, const Position2D& pos)
{
	assert(imgInfos_.imgDataId.size() > number || !"DrawImageDiv�Ŕ͈͊O�̃n���h��ID���w�肵�悤�Ƃ��܂���");

	DrawRotaGraph(pos.x, pos.y,
		imgInfos_.imgScl, imgInfos_.imgAngle, imgInfos_.imgDataId[number], true);
}

void ImageDiv::SetPos(const Position2D& pos)
{
	// �����蔻��p�̃f�[�^������
	Position2D vec = { pos.x - imgInfos_.imgCenter.x, pos.y - imgInfos_.imgCenter.y };
	imgInfos_.imgPlane.leftUpPos += vec;
	imgInfos_.imgPlane.rightDownPos += vec;

	imgInfos_.imgCenter = pos;
}

const Position2D& ImageDiv::GetPos(void) const
{
	return imgInfos_.imgCenter;
}

Position2D& ImageDiv::GetPosReference(void)
{
	return imgInfos_.imgCenter;
}

Plane2D& ImageDiv::GetPlaneReference(void)
{
	return imgInfos_.imgPlane;
}

Size2D ImageDiv::GetSize(void) const
{
	return { imgInfos_.posRightDown.x - imgInfos_.posLeftUp.x,
			 imgInfos_.posRightDown.y - imgInfos_.posLeftUp.y };
}

int ImageDiv::GetImageDataSize(void)
{
	return static_cast<int>(imgInfos_.imgDataId.size());
}

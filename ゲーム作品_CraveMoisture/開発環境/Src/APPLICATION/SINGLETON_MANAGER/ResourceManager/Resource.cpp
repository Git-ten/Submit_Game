#include <DxLib.h>
#include<algorithm>
#include "Resource.h"

#pragma region íËêîêÈåæ

// ó·äOéq
const int EXCEPTION_VALUE = -1;

#pragma endregion

Resource::Resource(void):
	resType_(RESOURCE_TYPE::NONE),
	path_(""),
	handleId_(EXCEPTION_VALUE),
	numX_(EXCEPTION_VALUE),
	numY_(EXCEPTION_VALUE),
	sizeX_(EXCEPTION_VALUE),
	sizeY_(EXCEPTION_VALUE)
{
	
}

Resource::Resource(const RESOURCE_TYPE& type, const std::string& path) :
	resType_(type),
	path_(path),
	handleId_(EXCEPTION_VALUE),
	numX_(EXCEPTION_VALUE),
	numY_(EXCEPTION_VALUE),
	sizeX_(EXCEPTION_VALUE),
	sizeY_(EXCEPTION_VALUE)
{
	
}

Resource::Resource(const RESOURCE_TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
{
	resType_ = type;
	path_ = path;
	numX_ = numX;
	numY_ = numY;
	sizeX_ = sizeX;
	sizeY_ = sizeY;

	handleId_ = EXCEPTION_VALUE;
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{

	switch (resType_)
	{
	case Resource::RESOURCE_TYPE::IMG:
		// âÊëú
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::RESOURCE_TYPE::NONE:
		handleId_ = LoadBlendGraph(path_.c_str());
		break;

	case Resource::RESOURCE_TYPE::IMGS:
		// ï°êîâÊëú
		handleIds_.resize(numX_ * numY_);
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::RESOURCE_TYPE::MODEL:
		// ÉÇÉfÉã
		handleId_ = MV1LoadModel(path_.c_str());
		break;
	case Resource::RESOURCE_TYPE::MOVIE:
		// ìÆâÊ
		handleId_ = LoadGraph(path_.c_str());
		break;
	default:
		break;
	}

}

void Resource::Release(void)
{

	switch (resType_)
	{
	case Resource::RESOURCE_TYPE::IMG:
	case Resource::RESOURCE_TYPE::NONE:
	{
		DeleteGraph(handleId_);
	}
	break;

	case Resource::RESOURCE_TYPE::IMGS:
	{
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		handleIds_.clear();
	}
	break;

	case Resource::RESOURCE_TYPE::MODEL:
	{
		MV1DeleteModel(handleId_);
		for (auto id : duplicateModelIds_)
		{
			MV1DeleteModel(id);
		}
	}
	break;
	}

}

void Resource::Release(int handleId)
{
	switch (resType_)
	{
	case Resource::RESOURCE_TYPE::IMG:
		break;

	case Resource::RESOURCE_TYPE::IMGS:
		break;

	case Resource::RESOURCE_TYPE::MODEL:
	{
		MV1DeleteModel(handleId);
		
		int c = 0;
		for (auto id : duplicateModelIds_)
		{
			if (handleId == id) {
				//MV1DeleteModel(id);
				duplicateModelIds_.erase(duplicateModelIds_.begin() + c);
				break;
			}
			c++;
		}
	}
	break;
	}
}

void Resource::CopyHandle(int* imgs)
{

	if (handleIds_.size() <= 0)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}

}

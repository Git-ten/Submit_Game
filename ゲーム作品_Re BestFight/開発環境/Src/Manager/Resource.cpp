#include <DxLib.h>
#include<algorithm>
#include "Resource.h"

Resource::Resource(void)
{
	resType_ = TYPE::NONE;
	path_ = "";

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(const TYPE& type, const std::string& path)
{
	resType_ = type;
	path_ = path;

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(const TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
{
	resType_ = type;
	path_ = path;
	numX_ = numX;
	numY_ = numY;
	sizeX_ = sizeX;
	sizeY_ = sizeY;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
		// âÊëú
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::NONE:
		handleId_ = LoadBlendGraph(path_.c_str());
		break;

	case Resource::TYPE::IMGS:
		// ï°êîâÊëú
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::TYPE::MODEL:
		// ÉÇÉfÉã
		handleId_ = MV1LoadModel(path_.c_str());
		break;
	}

}

void Resource::Release(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
	case Resource::TYPE::NONE:
	{
		DeleteGraph(handleId_);
	}
	break;

	case Resource::TYPE::IMGS:
	{
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		delete[] handleIds_;
	}
	break;

	case Resource::TYPE::MODEL:
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
	case Resource::TYPE::IMG:
		break;

	case Resource::TYPE::IMGS:
		break;

	case Resource::TYPE::MODEL:
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

	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}

}

#include <DxLib.h>
#include <cassert>
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Application/Application.h"
#include "Movie.h"

Movie::Movie(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id)
{
	movieInfo_ = LoadDataImage::LoadedDataImage(Application::GetInstance().GetDataPaths(), id, info);
}

Movie::~Movie(void)
{
}

void Movie::Play(int type)
{
	if (type != DX_PLAYTYPE_NORMAL &&
		type != DX_PLAYTYPE_BACK &&
		type != DX_PLAYTYPE_LOOP)
	{
		assert(!"ñ≥å¯Ç»ìÆâÊçƒê∂É^ÉCÉvÇ≈Ç∑");
	}

	PlayMovieToGraph(movieInfo_.imgDataId, type);
}

void Movie::Stop(void)
{
	PauseMovieToGraph(movieInfo_.imgDataId);
}

void Movie::DrawMovie(void)
{
	DrawRotaGraph(movieInfo_.imgCenter.x, movieInfo_.imgCenter.y,
		movieInfo_.imgScl, movieInfo_.imgAngle, movieInfo_.imgDataId, true);
}
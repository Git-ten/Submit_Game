#include"../Application.h"
#include "SceneBase.h"

//コンストラクタ
SceneBase::SceneBase()
{

}

//デストラクタ
SceneBase::~SceneBase()
{

}

std::weak_ptr<Camera> SceneBase::GetCamera(void) const
{
	return camera_;
}

//マップ座標に変換
Position2D SceneBase::World2MapPos(const Position2D& worldPos)
{
	Position2D ans;
	Position2D mapSize = Application::GetInstance().GetMapSize();

	int x = worldPos.x / mapSize.x;
	int y = worldPos.y / mapSize.y;

	ans = { x, y };
	return ans;
}

void SceneBase::DrawMap(void)
{
	Position2D mapSize = Application::GetInstance().GetMapSize();
	Position2D screenSize = Application::GetInstance().GetScreenSize();

	for (int ii = 0; ii < mapSize.y; ii++)
	{
		DrawLine(ii * mapSize.x, 0, ii * mapSize.x, screenSize.y, 0x00ff00);
		DrawFormatString(ii * mapSize.x, 0, 0xffffff, "%d", ii + 1);
	}

	for (int jj = 0; jj < mapSize.x; jj++)
	{
		DrawLine(0, jj * mapSize.y, screenSize.x, jj * mapSize.y, 0x0000ff);
		DrawFormatString(0, jj * mapSize.y, 0xffffff, "%d", jj + 1);
	}
}
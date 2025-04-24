#pragma once
#include "../../Common/Time.h"
#include "../ActorBase.h"
class EntranceObject : public ActorBase
{
public:

	EntranceObject(void);
	~EntranceObject(void)override;

	void InitSpecial(void)override;

	void OpenEntrance(void);
	void CloseEntrance(void);

	bool IsOpenEntrance(void);

private:

	ActorData actorData_;

	bool isOpen_;

	Time mirageTime_;
	Time dessolveTime_;

	void InitActorTransform(void)override;
	void UpdateSpecial(void)override;

	void DrawSpecial(void)override;

	void CollisionedHorizontalActor(void)override;
};
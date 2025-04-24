#include "Edge.h"

Edge::Edge(int from, int to, float cost)
{
	idxFrom_ = from;
	idxTo_ = to;
	passCost_ = cost;
}

Edge::~Edge()
{
}

float Edge::GetCost() const
{
	return passCost_;
}

int Edge::GetPrevIndex() const
{
	return idxFrom_;
}

int Edge::GetNextIndex() const
{
	return idxTo_;
}

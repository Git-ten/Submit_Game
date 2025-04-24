#pragma once
#include <string>
#include <vector>

class DataAIEdge
{
public:

	std::string fromIndexName;					// 前ノードの名前
	std::vector<std::string>endIndexNames;		// 次ノードの名前
	std::vector<std::string>conditionNames;		// それぞれの遷移条件名
	std::vector<float>costs;					// それぞれの遷移コスト

};
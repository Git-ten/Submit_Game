#pragma once
#include <string>
#include <vector>

class DataAIEdge
{
public:

	std::string fromIndexName;					// �O�m�[�h�̖��O
	std::vector<std::string>endIndexNames;		// ���m�[�h�̖��O
	std::vector<std::string>conditionNames;		// ���ꂼ��̑J�ڏ�����
	std::vector<float>costs;					// ���ꂼ��̑J�ڃR�X�g

};
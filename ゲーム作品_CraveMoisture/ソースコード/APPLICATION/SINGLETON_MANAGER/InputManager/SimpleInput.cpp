#include <cassert>
#include "SimpleInput.h"

SimpleInput* SimpleInput::instance_ = nullptr;

void SimpleInput::CreateInstance(void)
{
	assert(instance_ == nullptr || !"SimpleInput���쐬����Ă���̂ɍĂѐ������悤�Ƃ��Ă��܂�");

	instance_ = new SimpleInput();
}

SimpleInput& SimpleInput::GetInstance(void)
{
	return *instance_;
}

void SimpleInput::Destroy(void)
{
	input_.clear();
	delete instance_;
	instance_ = nullptr;
}

void SimpleInput::Add(const std::string& keyName, const std::function<bool(void)> keyFunc)
{
	input_.emplace(keyName, keyFunc);
}

const bool SimpleInput::GetInput(const std::string& keyName)const
{
	const auto& ite = input_.find(keyName);
	assert(ite != input_.end() || !"SimpleInput�œo�^����Ă��Ȃ����̂̃L�[���Q�Ƃ��悤�Ƃ��܂����B");

	return input_.at(keyName)();
}

SimpleInput::SimpleInput(void)
{

}
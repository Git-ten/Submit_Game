#include<DxLib.h>
#include "Debuger.h"

Debuger* Debuger::instance_ = nullptr;

void Debuger::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Debuger();
	}
	instance_->Init();
}

Debuger& Debuger::GetInstance()
{
	return *instance_;
}

Debuger::Debuger()
{
	lineCounter = 0;
}

void Debuger::Init()
{

}

void Debuger::Update()
{
	//ƒJƒEƒ“ƒ^[‚Ì‰Šú‰»
	lineCounter = 0;
}

void Debuger::Draw()
{

}

void Debuger::Destroy(void)
{
	delete instance_;
}

void Debuger::DrawDebugString(std::string line)
{
	DrawFormatString(0, 30 * lineCounter, 0x000000, line.c_str());
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}

void Debuger::DrawDebugValueStringF(std::string line, float v1)
{
	DrawFormatString(0, 30 * lineCounter, 0x000000, line.c_str(), v1);
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}

void Debuger::DrawDebugValueStringD(std::string line, int v1)
{
	DrawFormatString(0, 30 * lineCounter, 0xffffff, line.c_str(), v1);
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}

void Debuger::DrawDebugValueStringF(std::string line, float v1, float v2)
{
	DrawFormatString(0, 30 * lineCounter, 0x000000, line.c_str(), v1, v2);
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}

void Debuger::DrawDebugValueStringD(std::string line, int v1, int v2)
{
	DrawFormatString(0, 30 * lineCounter, 0xffffff, line.c_str(), v1, v2);
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}

void Debuger::DrawDebugValueStringF(std::string line, float v1, float v2, float v3)
{
	DrawFormatString(0, 30 * lineCounter, 0x000000, line.c_str(), v1, v2, v3);
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}

void Debuger::DrawDebugValueStringD(std::string line, int v1, int v2, int v3)
{
	DrawFormatString(0, 30 * lineCounter, 0xffffff, line.c_str(), v1, v2, v3);
	//ŠÖ”‚ªŒÄ‚Ño‚³‚ê‚é‚½‚Ñ‚É‰ÁZ
	lineCounter++;
}
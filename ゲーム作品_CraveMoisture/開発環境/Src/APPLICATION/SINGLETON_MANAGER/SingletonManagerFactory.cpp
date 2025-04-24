#include <cassert>
#include "SceneManager/SceneManager.h"
#include "ResourceManager/ResourceManager.h"
#include "EffectResourceManager/EffectResourceManager.h"
#include "InputManager/InputManager.h"
#include "SoundResourceManager/SoundResourceManager.h"
#include "SingletonManagerFactory.h"

void SingletonManagerFactory::InstanceFactory(const SINGLETON_NAME& name)
{
	switch (name)
	{
	case SINGLETON_NAME::SINGLETON_SCENE_MANAGER:
		SceneManager::CreateInstance();
		break;
	case SINGLETON_NAME::SINGLETON_RESOURCE_MANAGER:
		ResourceManager::CreateInstance();
		break;
	case SINGLETON_NAME::SINGLETON_EFFECT_RESOURCE_MANAGER:
		EffectResourceManager::CreateInstance();
		break;
	case SINGLETON_NAME::SINGLETON_INPUT_RESOURCE_MANAGER:
		InputManager::CreateInstance();
		break;
	case SINGLETON_NAME::SINGLETON_SOUND_RESOURCE_MANAGER:
		SoundResourceManager::CreateInstance();
		break;
	default:
		break;
	}
}

void SingletonManagerFactory::DestroyInstance(void)
{
	Destroy(SINGLETON_NAME::SINGLETON_SCENE_MANAGER);
	Destroy(SINGLETON_NAME::SINGLETON_RESOURCE_MANAGER);
	Destroy(SINGLETON_NAME::SINGLETON_EFFECT_RESOURCE_MANAGER);
	Destroy(SINGLETON_NAME::SINGLETON_INPUT_RESOURCE_MANAGER);
	Destroy(SINGLETON_NAME::SINGLETON_SOUND_RESOURCE_MANAGER);

	// ���ׂč폜���ꂽ���m�F
	CheckDestroyInstance();
}

void SingletonManagerFactory::Destroy(const SINGLETON_NAME& name)
{
	switch (name)
	{
	case SINGLETON_NAME::SINGLETON_SCENE_MANAGER:
		if(!SceneManager::IsNullptr())SceneManager::GetInstance().Destroy();
		break;
	case SINGLETON_NAME::SINGLETON_RESOURCE_MANAGER:
		if (!ResourceManager::IsNullptr())ResourceManager::GetInstance().Destroy();
		break;
	case SINGLETON_NAME::SINGLETON_EFFECT_RESOURCE_MANAGER:
		if (!EffectResourceManager::IsNullptr())EffectResourceManager::GetInstance().Destroy();
		break;
	case SINGLETON_NAME::SINGLETON_INPUT_RESOURCE_MANAGER:
		if (!InputManager::IsNullptr())InputManager::GetInstance().Destroy();
		break;
	case SINGLETON_NAME::SINGLETON_SOUND_RESOURCE_MANAGER:
		if (!SoundResourceManager::IsNullptr())SoundResourceManager::GetInstance().Destroy();
		break;
	default:
		break;
	}
}

void SingletonManagerFactory::CheckDestroyInstance(void)
{
	if (!SceneManager::IsNullptr())				assert(!"SceneManager���J������Ă��܂���");
	if (!ResourceManager::IsNullptr())			assert(!"ResourceManager���J������Ă��܂���");
	if (!EffectResourceManager::IsNullptr())	assert(!"EffectResourceManager���J������Ă��܂���");
	if (!InputManager::IsNullptr())				assert(!"InputManager���J������Ă��܂���");
	if (!SoundResourceManager::IsNullptr())		assert(!"SoundResourceManager���J������Ă��܂���");
}
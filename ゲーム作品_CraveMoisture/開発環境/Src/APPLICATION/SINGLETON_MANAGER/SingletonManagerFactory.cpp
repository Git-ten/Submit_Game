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

	// すべて削除されたか確認
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
	if (!SceneManager::IsNullptr())				assert(!"SceneManagerが開放されていません");
	if (!ResourceManager::IsNullptr())			assert(!"ResourceManagerが開放されていません");
	if (!EffectResourceManager::IsNullptr())	assert(!"EffectResourceManagerが開放されていません");
	if (!InputManager::IsNullptr())				assert(!"InputManagerが開放されていません");
	if (!SoundResourceManager::IsNullptr())		assert(!"SoundResourceManagerが開放されていません");
}
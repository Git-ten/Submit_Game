#pragma once

#define SINGLETON_MANAGER_FACTORY(a) SingletonManagerFactory::InstanceFactory(a);

#define SCENE_MANAGER SingletonManagerFactory::SINGLETON_NAME::SINGLETON_SCENE_MANAGER
#define RESOURCE_MANAGER SingletonManagerFactory::SINGLETON_NAME::SINGLETON_RESOURCE_MANAGER
#define EFFECT_RESOURCE_MANAGER SingletonManagerFactory::SINGLETON_NAME::SINGLETON_EFFECT_RESOURCE_MANAGER
#define INPUT_MANAGER SingletonManagerFactory::SINGLETON_NAME::SINGLETON_INPUT_RESOURCE_MANAGER
#define SOUND_RESOURCE_MANAGER SingletonManagerFactory::SINGLETON_NAME::SINGLETON_SOUND_RESOURCE_MANAGER

class SingletonManagerFactory
{
public:

	// シングルトン
	enum class SINGLETON_NAME
	{
		SINGLETON_SCENE_MANAGER,
		SINGLETON_RESOURCE_MANAGER,
		SINGLETON_EFFECT_RESOURCE_MANAGER,
		SINGLETON_INPUT_RESOURCE_MANAGER,
		SINGLETON_SOUND_RESOURCE_MANAGER
	};

	// インスタンス生成
	static void InstanceFactory(const SINGLETON_NAME& name);

	// すべてのインスタンスを削除
	static void DestroyInstance(void);
	
private:

	// インスタンスの削除処理
	static void Destroy(const SINGLETON_NAME& name);

	// 生成されたインスタンスがすべて削除されたかチェック
	static void CheckDestroyInstance(void);

};
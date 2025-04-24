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

	// �V���O���g��
	enum class SINGLETON_NAME
	{
		SINGLETON_SCENE_MANAGER,
		SINGLETON_RESOURCE_MANAGER,
		SINGLETON_EFFECT_RESOURCE_MANAGER,
		SINGLETON_INPUT_RESOURCE_MANAGER,
		SINGLETON_SOUND_RESOURCE_MANAGER
	};

	// �C���X�^���X����
	static void InstanceFactory(const SINGLETON_NAME& name);

	// ���ׂẴC���X�^���X���폜
	static void DestroyInstance(void);
	
private:

	// �C���X�^���X�̍폜����
	static void Destroy(const SINGLETON_NAME& name);

	// �������ꂽ�C���X�^���X�����ׂč폜���ꂽ���`�F�b�N
	static void CheckDestroyInstance(void);

};
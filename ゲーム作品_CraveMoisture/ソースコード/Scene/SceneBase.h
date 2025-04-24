#pragma once
#include <functional>
#include <memory>
#include "GUI/ImageGUI.h"
#include "GUI/ImageDiv.h"
#include "../Common/SoundPlayLaunch.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/ModelRenderer/ModelShadowRenderer.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneID.h"

class SceneBase
{
public:

	// �R���X�g���N�^
	SceneBase(void);
	// �f�X�g���N�^
	virtual ~SceneBase(void);


	// ������
	void Init(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);

	virtual void Release(void);

	// �f�[�^�̃��[�h
	virtual void LoadSceneData(void);

protected:

	std::vector<std::unique_ptr<ImageGUI>>guiImages_;

	// PixcelRenderer
	std::unique_ptr<PixelRenderer>pixcelRenderer_;

	// SoundLauncher
	std::unique_ptr<SoundPlayLaunch>soundPlayLaunch_;

	// �����摜
	std::unique_ptr<ImageDiv> imgNumber_;
	// �L���摜
	std::unique_ptr<ImageDiv> imgResultSymbol_;

	// �_���[�W�����摜�p�̍��W���
	Position2D damageNumberPos_;


	// �A�N�^�[�S�̂ɋy�Ԓ��_�V�F�[�_
	std::unique_ptr<ModelShadowRenderer>shadowVertex_;



	// GUI�̕\�����
	bool isGuiDraw_;

	// �J�ڐ�V�[��ID
	SceneID::SCENE_ID nextSceneId_;

	void InitNumbersImage(LoadData::LoadDataInfo info);

	// ���ʐ��̍�������������
	virtual void InitSpecial(void);

	// ���ʐ��̍����X�V
	virtual void UpdateSpecial(void);

	// �T�E���h�̃����`���[��������
	virtual void InitSoundPlayLaunch(void);
	// BGM�̍Đ�����
	virtual void PlayBgm(void);

	void SetUpGUI(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id, 
		const std::function<bool(void)>& conditionFunc, const std::function<void(void)>& runFunc);

	void AddGUICondition(const int number, const std::function<bool(void)>& conditionFunc);

	// PixcelRenderer�̏�����
	virtual void InitPixcelRenderer(void);
	// PixcelRenderer�̍X�V
	virtual void UpdatePixcelRenderer(void);
	void DrawPixcelRenderer(void);

	// �e�V�F�[�_�̏�����
	virtual void InitShadowShader(void);
	void UpdateShadowShader(void);

	// �w�i�摜�̕`��
	virtual void DrawBack(void);

	// ���ʐ��̍����`��
	virtual void DrawSpecial(void);

	// ���݂̃^�C����`��
	void DrawTime(const float param);
	// �_���[�W�ʂ�`��
	void DrawDamage(const float param);

private:

	bool SceneTransition(void);

};
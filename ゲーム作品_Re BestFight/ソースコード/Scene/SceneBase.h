#pragma once
#include<memory>
#include"../Utility/VectorHeader.h"

#define INT_CAST(a)(static_cast<int>(a))

class Camera;

class SceneBase
{
public:

	//�R���X�g���N�^
	SceneBase(void);

	//�f�X�g���N�^
	virtual ~SceneBase(void);

	//���z������
	virtual void Init(void) = 0;
	//���z�X�V
	virtual void Update(void) = 0;
	//���z�`��
	virtual void Draw(void) = 0;
	//���z�������J��
	virtual void Release(void) = 0;
	//�J�����擾
	std::weak_ptr<Camera>GetCamera(void)const;

protected:

	//�f�o�b�O�p�̃J�����C���X�^���X
	std::shared_ptr<Camera>camera_;



	//�����ł̂ݎg�p����֐�
	//�}�b�v���W�ɕϊ�
	//�}�b�v���W��Application�N���X�Œ�`�����X�N���[���̑傫������v�Z����
	Position2D World2MapPos(const Position2D& worldPos);
	//�}�b�v�̕`��
	void DrawMap(void);

};


#pragma once
#include<memory>
#include<functional>
#include <DxLib.h>
#include "Common/Quaternion.h"
#include"Act/Common/Transform.h"
#include"../Utility/MyUtility.h"

class Camera
{

public:

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//��_�J����
		FREE,			//�t���[���[�h
		FOLLOW,			//�Ǐ]���[�h
		JUST,			//�W���X�g���胂�[�h
		SKILL_MODE,		//�L�������Ƃ̕K�E�Z�A�����̓L�����N���X�ōs��
		SPECIAL_ATTACK_MODE,//�L�������Ƃ̃t�B�j�b�V���U���A�����̓L�����N���X�ōs��
		SYNCHRO,			//���̃J�����Ɠ�������
	};

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FFAR
	static constexpr float CAMERA_FAR = 50000.0f;

	// �J�����̏������W
	static constexpr Position3DF DEFAULT_CAMERA_POS = { 0.0f, 200.0f, -500.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr Position3DF RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	//�J�����̏����
	static constexpr Dir3DF CAMERA_UP = {0.0f, 1.0f, 0.0f};

	//�J�����̎���p
	static constexpr float CAMERA_FOV = 1.0472f;

	//�t���[
	static constexpr float LIMIT_ANGLE_UP_X = 0.4444f;
	static constexpr float LIMIT_ANGLE_DOWN_X = 0.1666666f;
	static constexpr Position3DF IDEAL_FREE_POS = {0.0f, 200.0f, -600.0f};
	static constexpr Position3DF IDEAL_FREE_TARGET_POS = {0.0f, 50.0f, 0.0f};
	static constexpr float CAMERA_ANGLE_SPEED = 3.0f;

	//�Ǐ]
	//�^�[�Q�b�g���f���ƃJ�����̗��z���΋���
	static constexpr Position3DF IDEAL_RELATIVE_POS = {0.0f, 300.0f, -600.0f};
	//���̋����ł̗��z���΋���(�E���)
	static constexpr Position3DF IDEAL_RELATIVE_NEAR_POS_R = {600.0f, 200.0f, -600.0f};
	//���̋����ł̗��z���΋���(�����)
	static constexpr Position3DF IDEAL_RELATIVE_NEAR_POS_L = { -600.0f, 200.0f, -300.0f };
	//�^�[�Q�b�g���f���ƃJ�����̎��_�̗��z���΋���
	static constexpr Position3DF IDEAL_RELATIVE_TARGET_POS = {0.0f, 100.0f, 50.0f};
	//�J�����̈ړ����x
	static constexpr float CAMERA_SPEED = 70.0f;
	//���z���W��ς���^�C�~���O�̋���
	static constexpr float TARGET_LOCKON_DIS = 1000.0f;

	//�C�x���g
	//�W���X�g
	static constexpr Position3DF IDEAL_JUST_POS = {450.0f, 100.0f, -400.0f};

	//�R���X�g���N�^
	Camera(void);

	//�f�X�g���N�^
	~Camera(void);

	//������
	void Init(void);
	//�`��
	//�J�����֐��̓s����A�`��ƍX�V�͓����ɍs��
	void DrawUpdate(void);

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	//�J�����������I�ɍX�V
	void CompCameraInfoUpdate(void);
	//���[�h���Ƃ̃J�����������X�V
	void CompModeUpdate(void);
	//�J�����̏��������Z�b�g
	void CompSetCameraInfo(const Position3DF& pos, const Position3DF& target, const Quaternion& rot);

	//�J�������W�擾
	Position3DF GetPos(void) const;
	//���_���W�̎擾
	Position3DF GetTarget(void) const;
	//���W�̃Z�b�g
	void SetPos(const Position3DF& pos);
	//���_���W�̃Z�b�g
	void SetTarget(const Position3DF& target);
	//��]���̎擾
	Quaternion GetRot(void)const;
	//��]���̃Z�b�g
	void SetRot(const Quaternion& q);
	//�f�t�H���g��]���̎擾
	Quaternion GetDefRot(void)const;

	//�J�����̃f�t�H���g���x���Z�b�g
	void SetDefaultSpeed(void);
	//�J�����̑��x���Z�b�g
	void SetSpeed(float speed);
	//�J�����̑��x���擾
	float GetSpeed(void)const;

	//�O�������擾
	Dir3DF GetForward(void)const;
	//�E�������擾
	Dir3DF GetRight(void)const;
	//���������擾
	Dir3DF GetLeft(void)const;
	//���������擾
	Dir3DF GetBack(void)const;
	//��������擾
	Dir3DF GetUp(void)const;
	//���������擾
	Dir3DF GetDown(void)const;
	
	//���݂̃J�������[�h
	MODE GetCameraMode(void)const;
	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);
	//�f�t�H���g�J�������[�h���Z�b�g
	void SetDefaultMode(MODE mode);
	//�J�������[�h���f�t�H���g�ɂ���
	void ChangeDefaultMode(void);

	//�t���[���[�h����----------------------
	//�^�[�Q�b�g���Z�b�g
	void SetCameraTarget(std::weak_ptr<Transform> target);
	//--------------------------------------

	//�Ǐ]���[�h�̏���----------------------
	//�Ǐ]�Ώۂ��Z�b�g
	void SetFollower(std::weak_ptr<Transform> Tatrans, std::weak_ptr<Transform> Lotrans);
	//------------------------------

	//����J�������[�h�̏����Z�b�g
	void SetSpecialCamera(std::function<void(void)>func);

private:

	//���x
	float speed_;

	//�f�t�H���g�J�������[�h
	MODE def_mode_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	Position3DF pos_;

	// �J�����̒����_
	Position3DF targetPos_;

	// �J�����̏����
	Dir3DF cameraUp_;

	// �X�V�֐�
	std::function<void(void)>update_;

	// �J�����̉�]
	Quaternion rot_;
	//�f�t�H���g�̉�]
	Quaternion def_rot_;

	//�J�����̊p�x
	Rot3DF angle_;

	//�Ǐ]�Ώ�
	std::weak_ptr<Transform> targetTrans_;
	//���b�N�I���Ώ�
	std::weak_ptr<Transform> lockonTrans_;



	//�����ł̂ݎg�p����֐�
	Dir3DF GetDir(const Dir3DF& vec)const;
	//�������Ȃ��֐�
	void UpdateNone(void);

	//�t���[���[�h����----------------------
	//�C���v�b�g�J�����A���O��
	void InputCameraAngle(void);
	//�t���[�X�V
	void UpdateFree(void);
	//--------------------------------------

	//�Ǐ]���[�h�̏���----------------------
	//�Ǐ]�X�V
	void UpdateFollow(void);
	//------------------------------

	//�W���X�g�K�[�h���[�h�̏���-------------
	//�X�V
	void UpdateJustGurd(void);
	//---------------------------------------

};
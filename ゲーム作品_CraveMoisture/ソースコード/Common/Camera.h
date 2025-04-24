#pragma once
#include<memory>
#include<functional>
#include <DxLib.h>
#include "DataLoad/DataCameraControl.h"
#include "Quaternion.h"
#include"../../Utility/Utility.h"
#include "../Application/Singleton_Manager/InputManager/InputManager.h"

class Transform;

class Camera
{

public:

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;
	// �J�����N���b�v�FFAR
	static constexpr float CAMERA_FAR = 150000.0f;


	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//��_�J����
		FREE,			//�t���[���[�h
		FOLLOW,			//�Ǐ]���[�h
		SYNCHRO,		//���̃J�����Ɠ�������
		DEBUG,			//�f�o�b�O���[�h
	};


	//�R���X�g���N�^
	Camera(void);
	//�f�X�g���N�^
	~Camera(void);


	//������
	void Init(void);
	//�`��
	//�J�����֐��̓s����A�`��ƍX�V�͓����ɍs��
	void DrawUpdate(void);

	//�J�������W�擾
	const Position3DF& GetPos(void) const;
	// 1�t���[���O�̃J�������W���擾
	const Position3DF& GetPrevPos(void)const;

	//��]���̎擾
	const Rot3DF& GetAngle(void)const;
	const Quaternion& GetRot(void)const;

	//�f�t�H���g��]���̎擾
	const Quaternion& GetDefRot(void)const;


	//���[���hY�����ɐ����ȃJ�����̊p�x
	Quaternion GetRotXZ(void);

	// ���_���W���擾
	const Position3DF& GetTargetPos(void)const;

	// ���_�ƃJ�������W�̋���
	float GetDistanceTarget(void);
	// ���_�ƃJ�������W�̋������Z�b�g
	void SetDistanceTarget(float distance);
	// ���_�ƃJ�������W�̋������f�t�H���g�ɂ���
	void SetDefaultDistanceTarget(void);
	float GetDefaultDistanceTarget(void);


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
	//�������擾
	Dir3DF GetDir(const Dir3DF& vec)const;


	//���݂̃J�������[�h
	const MODE& GetCameraMode(void)const;
	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]���[�h��p::�Ǐ]�Ώۂ��Q�Ƃ���(���W)
	void SetFollowTargetPos(Position3DF& pos);
	//�Ǐ]���[�h��p::�Ǐ]�Ώۂ��Q�Ƃ���
	void SetTargetTrans(std::weak_ptr<Transform> trans);
	//�Ǐ]���[�h��p::�J�����̃��b�N�I���Ώۂ��Z�b�g����(�f�t�H���g�ł͒Ǐ]�ΏۂƓ����ɂȂ�)
	void SetLockOnTargetPos(Position3DF& lockon, bool isLerp = false);
	//�Ǐ]���[�h��p::���b�N�I������
	void CancellLockOnTarget(void);
	// �Ǐ]���[�h��p::���b�N�I�����Ă��邩
	const bool IsLockOn(void)const;

	void SetUpCollModel(const int id);

	// �J�����̉��o�����p�f�[�^���Z�b�g�A�b�v
	void SetUpCameraControlData(const std::vector<DataCameraControl>& dataControl);
	// �J�����̉��o�ǉ����֎~
	void ProhibicionCameraControl(void);

	// ���`�⊮���܂߂��C�x���g�����̎��s���
	const bool IsCameraDirectionEvent(void)const;

	void LockIsCameraUpdate(void);
	void UnLockIsCameraUpdate(void);
	bool IsLockCameraUpdate(void);

private:

	using DIRECTION_TABLE = std::map<std::string, std::function<bool(void)>>;

private:

	// �X�V�����b�N���邩
	bool isLockCameraUpdate_;

	// �����蔻����s�����f����ID
	std::vector<int> collModelId_;

	// ���o�J�����̃Z�b�g�A�b�v���t���O
	bool isCanSetUpEventCamera_;

	// ���o�֐��Ɩ��̂̊֘A�t��
	DIRECTION_TABLE directionTable;

	// ���݂̎w�肷��J��������f�[�^�̃C���f�b�N�X
	int cameraControlIdx_;


	// ���ݎw�肵�Ă���J��������f�[�^�̎Q�Ƃ𓾂�
	DataCameraControl& GetRefelenceCameraContData(void);
	// ���Ɏg�p����J��������f�[�^(������΁Afalse��Ԃ�)
	const bool NextCameraControlData(void);

	// �w�肷��C�x���g�̎��s�y�яI����Ԃ̎擾
	const bool CameraDirectionEvent(const std::string& eventName)const;
	
	// �C�x���g�I���܂ł̎��Ԃ��v��
	const bool CameraEvent(void);

	// �J�����V�F�C�N(true�ŃJ�����V�F�C�N�I���̈�)
	const bool CameraShake(void);

private:

	//���x
	float speed_;


	//�f�t�H���g�J�������[�h
	MODE def_mode_;


	// �J�������[�h
	MODE mode_;


	// �J�����̈ʒu
	Position3DF pos_;
	// 1�t���[���O�̃J�����̈ʒu
	Position3DF prevPos_;


	// �J�����̒����_
	Position3DF targetPos_;
	// �J�����̑��Β����_
	Position3DF relativeTargetPos_;

	//�J�����̃��b�N�I���Ώ�
	Position3DF* lockOnPos_;


	//�J�����̍��W�ƒ����_�̑��΍��W
	Position3DF relativePosTarget_;
	Position3DF def_relativPosTarget_;

	// �J�����̓����蔻��̗L��
	// �X�e�[�W�Ƃ̓����蔻����擾���邱�Ƃ�z�肵�Ă���
	bool isColl_;


	// �J�����̏����
	Dir3DF cameraUp_;

	// �J�������o�p����f�[�^
	std::vector<std::unique_ptr<DataCameraControl>> cameraControlData_;


	// �X�V�֐�
	using CameraUpdate = std::function<void(void)>;
	CameraUpdate update_;


	// �J�����̉�]
	Quaternion rot_;
	//�f�t�H���g�̉�]
	Quaternion def_rot_;


	//�J�����̊p�x(�ʓx�@)
	Rot3DF angle_;
	//���`�⊮�̂��߂Ɋp�x�ύX�O�̊p�x���L�^
	Rot3DF prevAngle_;
	//���`�⊮�̃^�C��
	float lerpAngleTime_;


	//���b�N�I�����̃J�����p�x(�ʓx�@)(�f�t�H���g�ł͒Ǐ]�Ώۂ𓯂��ɂȂ�)
	Rot3DF lockOnAngle_;


	//�Ǐ]����ۂ̑Ώۃ��f���̎Q��
	//�Q�Ƃ��i�[����
	std::weak_ptr<Transform> targetTrans_;
	// �Ǐ]����ۂ̑Ώۍ��W�ϐ��̎Q��
	// �Q�Ƃ��i�[����
	Position3DF* followTargetPos_;


	//�����ł̂ݎg�p����֐�
	// ���O�Ɗ֐��̊֘A�t����������
	void InitDirectionName(void);

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	void CameraCollision(void);

	//�������Ȃ��֐�
	void UpdateNone(void);
	
	//�֐��|�C���^�p
	void UpdateFixedPoint(void);
	void UpdateFree(void);
	void UpdateFollow(void);
	void UpdateDebug(void);

	//���ʂōs������
	void UpdateCommon(void);
	// �J��������p�f�[�^�Ɋ�Â������䏈��
	void UpdateCameraControl(void);
	// �J�����̋����̐��`�⊮
	void LerpDistance(const float prevDis, const float t);
	// �J�����̒����_�̐��`�⊮
	void LerpTargetPos(const Position3DF& prevPos, const float t);
	// �J�����̏�����̐��`�⊮
	void LerpCameraUp(const Dir3DF& prevDir, const float t);


	//�t���[���[�h����----------------------
	//����
	void InputFreeCamera(void);
	//--------------------------------------

	//�Ǐ]���[�h�̏���----------------------
	//�Ǐ]����
	void FollowTarget(void);
	//�J��������
	void InputFollowCamera(void);
	//���b�N�I������
	void LockOn(void);
	// ���b�N�I���ɂ���]����
	void LockOnRotation(void);
	//--------------------------------------

	//�f�o�b�O���[�h�̏���------------------
	//����
	void InputDebugCamera(void);
	//���W�ړ�����
	//�����_���������ړ�
	void MovePos(float valueX, float valueY);
	//--------------------------------------

};
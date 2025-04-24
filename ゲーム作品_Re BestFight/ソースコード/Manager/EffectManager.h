#pragma once
#include<memory>
#include<vector>
#include<map>

class Effect;

class EffectManager
{
public:

	//�G�t�F�N�g�̎��
	enum class EFF
	{
		CARSOL_EFF,//�����G�t�F�N�g
		JUMP,//�W�����v�̓y��
		SWING,//��U��G�t�F�N�g
		STRONG,//���G�t�F�N�g
		ATTACK_WIND,//�U����
		FLASH,//;��U���G�t�F�N�g
		SORD,//�a���G�t�F�N�g
		FALL_ATTACK,//�����U��
		BARIA,//�h��G�t�F�N�g
		QUICK_MOVE,//�����ړ�
		JUST,//�W���X�g�G�t�F�N�g
		SKILL_MAX,//�K�E�ZMAX�G�t�F�N�g
		SKILL_FLAME,//���K�E�Z
		SKILL_SORD,//�\�[�h�K�E�r�[��
		LIGHTNING,//����
		LASER_FIRE,//�����[�U�[
		BLUR,//�u���[
		FIRE_CIRCLE,//�퓬�J�n�G�t�F�N�g
		LASER_MAGIC,//���p���[�U�[
		LASER_VIOLENCE,//���p���[�U�[(��)
		AURA,//���[�v�I�[���L���[�u
		BIG_BANG,//�r�b�N�o��
		TRNADE,//����
		FHANTASM,//���e�G�t�F�N�g
		SOGEKI,//�_�������w
		METEO,//覐�
		MAHOUJIN,//覐Ζ����w
		GURD_BREAK,//�K�[�h�u���[�N
		EFF_MAX,//�G�t�F�N�gID�̍ő�l
	};


	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static EffectManager& GetInstance(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//2D�G�t�F�N�g�̕`��
	void Draw2DEffect(void);
	//3D�G�t�F�N�g�̕`��
	void Draw3DEffect(void);
	//�������J��
	void Release(void);
	//���\�[�X�̊��S�J��
	void Destroy(void);

	//�ǂݍ���
	std::weak_ptr<Effect> Load(const EFF& eff);

private:

	using  ResourcesData = std::map<EFF, Effect>;
	using  LoadedData = std::map<EFF, std::vector<std::shared_ptr<Effect>>>;

	//�V���O���g���C���X�^���X
	static EffectManager* instance_;

	//���\�[�X�A�z�z��
	ResourcesData resourcesMap_;
	//���[�h
	LoadedData loadedMap_;



	//�����ł̂ݎg�p����֐�
	//�O������C���X�g���N�^�𐶐��ł��Ȃ��悤��
	//�����ŃR���X�g���N�^��錾����
	EffectManager(void);
	// �R�s�[�R���X�g���N�^�����l
	EffectManager(const EffectManager&);
	//�f�X�g���N�^
	~EffectManager(void) = default;

	//�����I���[�h
	std::weak_ptr<Effect> _Load(const EFF& eff);

};
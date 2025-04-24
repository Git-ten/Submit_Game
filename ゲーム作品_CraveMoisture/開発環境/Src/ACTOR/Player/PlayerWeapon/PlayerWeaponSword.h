#pragma once
#include "../../RENDERER/ModelRenderer/PoligonRenderer.h"
#include "PlayerWeaponBase.h"

class PlayerWeaponSword : public PlayerWeaponBase
{
public:

	// �R���X�g���N�^
	PlayerWeaponSword(int& modelId);
	// �f�X�g���N�^
	~PlayerWeaponSword(void)override;

	void InitSpecial(void)override;

	// �O�Ղ̕`����I���ɂ���
	void OnDrawLocus(void);
	// �O�Ղ̕`����I�t�ɂ���
	void OffDrawLocus(void);

	//�O�Ղ̍X�V
	void UpdateLocus(float maxAnimStep, int attachIdx);

protected:

	// ���{�Ɛ�[
	struct PosBuf
	{
		Position3DF headPos;		//��[
		Position3DF tailPos;		//���{
		int alpha;					//�����x
	};

	//���̍����Ɛ�[�̍��W
	std::vector<PosBuf>posBuf_;
	// �V�F�[�_
	std::unique_ptr<PoligonRenderer>locusRenderer_;
	
	//�O�Ղ̃|���S��
	std::vector<VERTEX3DSHADER>vertex_;
	//�|���S���C���f�b�N�X
	std::vector<WORD>indexes_;

	// ���̋O�Ղ𐶐��`�悷�邩
	bool isSetLocus_;

	void InitActorTransform(void)override;

	// ���t���[���̌��̈ʒu��ݒ�
	void SetLocusPos(const PosBuf& buf);

	// ���̍��{�̍��W
	const Position3DF GetTailPos(void)const;
	// ���̐�[�̍��W
	const Position3DF GetHeadPos(void)const;

	void UpdateSpecial(void)override;
	void DrawSpecial(void)override;

};
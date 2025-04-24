#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "PlayerWeaponSword.h"

#pragma region �萔�錾

// �������q
const float INIT_ZERO = 0.0f;
// �����t�]
const float REVERSE_VALUE = -1.0f;
// �����x�̍ő�l
const int ALPHA_MAX = 255;
// �����x�̍ŏ��l
const int ALPHA_MIN = 0;
// �V�F�[�_�ɃZ�b�g����F
const int LOCUS_SHADER_COLOR = 255;

// �l�̔���������o��
const int HALF_PARAM = 2;

// �l��2�{���Z�o
const int TWO_MULT = 2;

// �O�Ղ̑傫��
const float LOCUS_SIZE = 20.0f;
// �O�Ճ|���S���̍Œ�ŕK�v�Ȑ�
const int LOCUS_POLIGON_MIN_NUM = 3;
// �|���S���e�N�X�`����V�̍ŏ��l
const float LOCUS_POLIGON_V_MIN = 0.0f;
// �|���S���e�N�X�`����V�̍ő�l
const float LOCUS_POLIGON_V_MAX = 1.0f;


// �O�Ղ������鑬�x
const int LOCUS_DELETE_SPEED = 15;
// �O�Ղ��L���鑬�x
const float LOCUS_SPREAD = 20.0f;

// ���̍��W���獪���܂ł̑傫��
const float POS_TO_TAILE = 210.0f;
// ���̍��W�����[�܂ł̑傫��
const float POS_TO_HEAD = 250.0f;

#pragma endregion

PlayerWeaponSword::PlayerWeaponSword(int& modelId):
	PlayerWeaponBase(modelId),
	isSetLocus_(false)
{
}

PlayerWeaponSword::~PlayerWeaponSword(void)
{
}

void PlayerWeaponSword::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "PlayerInfo.json", "Sword"};

	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::HERO_SWORD);

	actorData_.pos_ = &actorTransform_->GetPosReference();

	myActorCollision_.collisiionAttackModelId_ = &actorTransform_->GetModelIdReference();
	myActorCollision_.isCollAttack_ = false;
}

void PlayerWeaponSword::SetLocusPos(const PosBuf& buf)
{
	posBuf_.push_back(buf);
}

const Position3DF PlayerWeaponSword::GetTailPos(void) const
{
	Position3DF pos = actorTransform_->GetPos();
	pos = VAdd(pos, VScale(actorTransform_->GetForward(), POS_TO_TAILE));

	return pos;
}

const Position3DF PlayerWeaponSword::GetHeadPos(void) const
{
	Position3DF pos = actorTransform_->GetPos();
	pos = VAdd(pos, VScale(actorTransform_->GetForward(), POS_TO_HEAD));

	return pos;
}

void PlayerWeaponSword::InitSpecial(void)
{
	locusRenderer_ = std::make_unique<PoligonRenderer>();

	locusRenderer_->SetTexture(
		ResourceManager::GetInstance().Load(
			ResourceManager::RESOURCE_ID::SWORD_LOCUS_TEXTURE).handleId_
	);
	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::METAL_MODEL);

}

void PlayerWeaponSword::OnDrawLocus(void)
{
	isSetLocus_ = true;
}

void PlayerWeaponSword::OffDrawLocus(void)
{
	isSetLocus_ = false;
}

void PlayerWeaponSword::UpdateLocus(float maxAnimStep, int attachIdx)
{
	static float prevMaxAnimStep = INIT_ZERO;

	if (isSetLocus_)
	{
		if (prevMaxAnimStep != maxAnimStep)
		{
			for (float ii = prevMaxAnimStep; ii <= maxAnimStep; ii += 0.1f)
			{
				MV1SetAttachAnimTime(modelId_, attachIdx, ii);
				Update();

				PosBuf buf = { GetHeadPos(), GetTailPos(), ALPHA_MAX };

				Dir3DF dir = VNorm(VSub(buf.headPos, buf.tailPos));

				if (static_cast<int>(ii) % HALF_PARAM)
				{
					buf.headPos = VAdd(buf.headPos, VScale(dir, LOCUS_SIZE));
					buf.tailPos = VAdd(buf.tailPos, VScale(dir, LOCUS_SIZE));
				}
				else
				{
					dir = VScale(dir, REVERSE_VALUE);
					buf.headPos = VAdd(buf.headPos, VScale(dir, LOCUS_SIZE));
					buf.tailPos = VAdd(buf.tailPos, VScale(dir, LOCUS_SIZE));
				}

				SetLocusPos(buf);
			}

			prevMaxAnimStep = maxAnimStep;
		}
	}
	else
	{
		// �����s�Ȃ�펞������
		prevMaxAnimStep = INIT_ZERO;
		posBuf_.clear();
	}

	//�����x��0�ɂȂ����炻�̒n�_���牺�̂��ׂĂ̗v�f����������
	Position3DF headPos;
	Position3DF tailPos;
	Dir3DF dir;

	for (int ii = static_cast<int>(posBuf_.size() - 1); ii >= 0; ii--)
	{
		posBuf_[ii].alpha -= LOCUS_DELETE_SPEED;
		if (posBuf_[ii].alpha < ALPHA_MIN)
		{
			// �z��̍ŏ����瓧���x��0�̒n�_�܂ŏ���
			posBuf_.erase(posBuf_.begin(), posBuf_.begin() + ii);
			break;
		}
		else
		{
			// ����V�����Z�b�e�B���O
			headPos = posBuf_[ii].headPos;
			tailPos = posBuf_[ii].tailPos;
			dir = VNorm(VSub(headPos, tailPos));
			posBuf_[ii].headPos = VAdd(posBuf_[ii].headPos, VScale(dir, LOCUS_SPREAD));
			posBuf_[ii].tailPos = VAdd(posBuf_[ii].tailPos, VScale(dir, LOCUS_SPREAD));
		}
	}


	//���_�f�[�^���X�V����
	const float amount = 1.0f / static_cast<float>(posBuf_.size());
	float x = INIT_ZERO;
	vertex_.clear();
	vertex_.resize(posBuf_.size() * TWO_MULT);
	if (vertex_.size() <= LOCUS_POLIGON_MIN_NUM)return;


	indexes_.clear();
	indexes_.resize((posBuf_.size() - 1) * LOCUS_POLIGON_MIN_NUM * TWO_MULT);

	int kk = static_cast<int>(INIT_ZERO);
	for (int ii = 0, jj = 0; ii < vertex_.size(); ii += 2, jj++)
	{
		// ���_����
		vertex_[ii].pos = posBuf_[jj].tailPos;
		vertex_[ii].dif = GetColorU8(
			LOCUS_SHADER_COLOR, LOCUS_SHADER_COLOR, LOCUS_SHADER_COLOR, posBuf_[jj].alpha);
		vertex_[ii].u = x;
		vertex_[ii].v = LOCUS_POLIGON_V_MIN;
		indexes_[kk] = ii;// ���{�ɓ�����C���f�b�N�X
		vertex_[ii + 1].pos = posBuf_[jj].headPos;
		vertex_[ii + 1].dif = GetColorU8(
			LOCUS_SHADER_COLOR, LOCUS_SHADER_COLOR, LOCUS_SHADER_COLOR, posBuf_[jj].alpha);
		vertex_[ii + 1].u = x;
		vertex_[ii + 1].v = LOCUS_POLIGON_V_MAX;
		indexes_[kk + 1] = ii + 1;// ��[�ɓ�����C���f�b�N�X

		// �C���f�b�N�X����
		if (kk <= 0)
		{
			indexes_[kk + 2] = ii + 2;
			kk += LOCUS_POLIGON_MIN_NUM;
		}
		else
		{
			indexes_[kk + 2] = ii - 1;
			kk += LOCUS_POLIGON_MIN_NUM;

			if (kk < indexes_.size())
			{
				indexes_[kk] = ii;
				indexes_[kk + 1] = ii + 1;
				indexes_[kk + 2] = ii + 2;
				kk += LOCUS_POLIGON_MIN_NUM;
			}
		}

		x += amount;
	}
}

void PlayerWeaponSword::UpdateSpecial(void)
{
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	actorTransform_->UpdateVertexPS(1, FLOAT4(
		ModelRenderer::SHADER_NONE_VALUE,
		cameraPos.x,
		cameraPos.y,
		cameraPos.z));
}

void PlayerWeaponSword::DrawSpecial(void)
{
	// �o�b�N�J�����O�𖳌��ɂ���
	SetUseBackCulling(false);

	locusRenderer_->Draw(vertex_, indexes_);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);
}

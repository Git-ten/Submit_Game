#include <fstream>
#include <sstream>
#include <math.h>
#include "Utility.h"

#pragma region �萔�錾
const float kEpsilonNormalSqrt = 1e-15F;
#pragma endregion

int Utility::Round(float v)
{
    return static_cast<int>(roundf(v));
}

std::vector<std::string> Utility::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double Utility::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float Utility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int Utility::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double Utility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float Utility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int Utility::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

//�Ǝ��ǉ��@�\
// �x(deg)���烉�W�A��(rad)
Rot3DF Utility::Deg2RadV(const Rot3DF& deg)
{
    float h = DX_PI_F / 180.0f;
    return {deg.x * h, deg.y * h, deg.z * h};
}

Rot3DF Utility::Rad2DegV(const Rot3DF& rad)
{
    float h = 180.0f / DX_PI_F;
    return { rad.x * h, rad.y * h, rad.z * h };
}

double Utility::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double Utility::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

float Utility::RadIn2PI(float rad)
{
    rad = fmodf(rad, DX_TWO_PI_F);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI_F;
    }
    return rad;
}

int Utility::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }

    }
    else
    {

        // ��r�����������v���Ɉʒu����

        if (diff < -DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int Utility::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > 180.0f)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }

    }
    else
    {

        // ��r�����������v���Ɉʒu����

        if (diff < -180.0f)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int Utility::Lerp(int start, int end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    int ret = start;
    ret += Round(t * static_cast<float>(end - start));
    return ret;
}

float Utility::Lerp(float start, float end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    float ret = start;
    ret += t * (end - start);
    return ret;
}

double Utility::Lerp(double start, double end, double t)
{
    // ���`���
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

Position2D Utility::Lerp(const Position2D& start, const Position2D& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    Position2D ret = start;
    ret.x += Round(t * static_cast<float>((end.x - start.x)));
    ret.y += Round(t * static_cast<float>((end.y - start.y)));
    return ret;
}

Position3DF Utility::Lerp(const Position3DF& start, const Position3DF& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    Position3DF ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double Utility::LerpDeg(double start, double end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

COLOR_F Utility::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;
}

Position2D Utility::Bezier(const Position2D& p1, const Position2D& p2, const Position2D& p3, float t)
{
    Position2D a = Lerp(p1, p2, t);
    Position2D b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

Position3DF Utility::Bezier(const Position3DF& p1, const Position3DF& p2, const Position3DF& p3, float t)
{
    Position3DF a = Lerp(p1, p2, t);
    Position3DF b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

Position3DF Utility::RotXZPos(const Position3DF& centerPos, const Position3DF& radiusPos, float rad)
{
    float x = ((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = ((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return VGet(centerPos.x + x, radiusPos.y, centerPos.z + z);
}

double Utility::Magnitude(const Vector2& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

double Utility::Magnitude(const VECTOR& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Utility::MagnitudeF(const VECTOR& v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

int Utility::SqrMagnitude(const Vector2& v)
{
    return v.x * v.x + v.y * v.y;
}

float Utility::SqrMagnitudeF(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility::SqrMagnitude(const VECTOR& v)
{
    return static_cast<double>(v.x * v.x + v.y * v.y + v.z * v.z);
}

double Utility::SqrMagnitude(const VECTOR& v1, const VECTOR& v2)
{
    return pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2);
}

double Utility::Distance(const Position2D& v1, const Position2D& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

double Utility::Distance(const Position3DF& v1, const Position3DF& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

Vector2 Utility::GetHalfVector2(const Vector2& v)
{
    return {v.x / 2, v.y / 2};
}

bool Utility::IsHitSpheres(const Position3DF& pos1, float radius1, const Position3DF& pos2, float radius2)
{
    // ���̓��m�̏Փ˔���
    bool ret = false;

    // ���݂��̔��a�̍��v
    float radius = radius1 + radius2;

    // ���W�̍����炨�݂��̋��������
    VECTOR diff = VSub(pos2, pos1);

    // �O�����̒藝�Ŕ�r(SqrMagnitude�Ɠ���)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool Utility::IsHitSphereCapsule(
    const Position3DF& sphPos, float sphRadius,
    const Position3DF& capPos1, const Position3DF& capPos2, float capRadius)
{

    bool ret = false;

    // �J�v�Z�����̂̒��S���q���x�N�g��
    VECTOR cap1to2 = VSub(capPos2, capPos1);

    // �x�N�g���𐳋K��
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // �J�v�Z���q���̒P�ʃx�N�g���ƁA
    // ���̃x�N�g�������狅�̂ւ̃x�N�g���̓��ς����
    float dot = VDot(cap1to2ENor, VSub(sphPos, capPos1));

    // ���ςŋ��߂��ˉe�������g���āA�J�v�Z���q����̍��W�����
    Position3DF capRidePos = VAdd(capPos1, VScale(cap1to2ENor, dot));

    // �J�v�Z���q���̃x�N�g���̒��������
    float len = Utility::MagnitudeF(cap1to2);

    // ���̂��J�v�Z���q����ɂ��邩���ʂ��邽�߁A�䗦�����
    float rate = dot / len;

    Position3DF centerPos;

    // ���̂̈ʒu���R�G���A�ɕ������ꂽ�J�v�Z���`��̂ǂ��ɂ��邩����
    if (rate > 0.0f && rate <= 1.0f)
    {
        // �@���̂��J�v�Z���q����ɂ���
        centerPos = VAdd(capPos1, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // �A���̂��J�v�Z���̏I�_���ɂ���
        centerPos = capPos2;
    }
    else if (rate < 0.0f)
    {
        // �B���̂��J�v�Z���̎n�_���ɂ���
        centerPos = capPos1;
    }
    else
    {
        // �����ɂ��Ă͂����Ȃ�
    }

    // ���̓��m�̓����蔻��
    if (Utility::IsHitSpheres(centerPos, capRadius, sphPos, sphRadius))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;

}

bool Utility::Equals(const VECTOR& v1, const VECTOR& v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool Utility::EqualsVZero(const VECTOR& v1)
{
    const VECTOR& v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR Utility::Normalize(const Vector2& v)
{
    VECTOR ret = VGet(
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        0.0f
    );
    float len = static_cast<float>(Magnitude(v));
    ret.x /= len;
    ret.y /= len;
    ret.z /= len;
    return ret;
}

VECTOR Utility::VNormalize(const VECTOR& v)
{
    if (Utility::EqualsVZero(v))
    {
        // Quaternion�v�Z�Ń[����n���āA
        // �G���[(-1, -1, -1)���Ԃ��Ă���ƍ���
        return v;
    }
    return VNorm(v);
}

double Utility::AngleDeg(const VECTOR& from, const VECTOR& to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    auto fLen = SqrMagnitude(from);
    auto tLen = SqrMagnitude(to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(from, to) / denominator;
    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

//�O�p�`�̏d�S
VECTOR Utility::CentVector(const VECTOR& v1, const VECTOR& v2, const VECTOR& v3)
{
    VECTOR ans = VECTOR_ZERO;

    float x = (v1.x + v2.x + v3.x) / 3.0f;
    float y = (v1.y + v2.y + v3.y) / 3.0f;
    float z = (v1.z + v2.z + v3.z) / 3.0f;
    ans = {x, y, z};

    return ans;
}

VECTOR Utility::CentVector(const VECTOR& v1, const VECTOR& v2)
{
    VECTOR ans = VECTOR_ZERO;

    float x = (v1.x + v2.x) / 3.0f;
    float y = (v1.y + v2.y) / 3.0f;
    float z = (v1.z + v2.z) / 3.0f;
    ans = { x, y, z };

    return ans;
}

void Utility::GageRateX(Image* image, const float rate, Position2D& rightPos, Size2D& size)
{
    // ��`�p�ɉ摜�̊g��O�̃T�C�Y��ݒ�-----------------------
    size = image->GetSize();

    Size2DF sizeF = { static_cast<float>(size.x), static_cast<float>(size.y) };
    sizeF = sizeF / static_cast<float>(image->GetScl());
    sizeF.x *= rate;

    size = { static_cast<int>(sizeF.x), static_cast<int>(sizeF.y) };
    // -------------------------------------------------------

    // rate�ɂ��킹�āA�E�����W��ݒ�--------------
    Size2D imgSize = image->GetSize();

    rightPos = image->GetRightDownPos();
    rightPos.x = image->GetLeftUpPos().x + 
        static_cast<int>(static_cast<float>(imgSize.x) * rate);
    // --------------------------------------------

}

void Utility::DrawLineDir(const Position3DF& pos, const Dir3DF& dir, int color, float len)
{
    auto nDir = Utility::VNormalize(dir);
    auto sPos = VAdd(pos, VScale(nDir, -len));
    auto ePos = VAdd(pos, VScale(nDir, len));
    DrawLine3D(sPos, ePos, color);
    DrawSphere3D(ePos, 5.0f, 5, color, color, true);
}

void Utility::DrawLineXYZ(const Position3DF& pos, const MATRIX& rot, float len)
{

    VECTOR dir;

    // X
    dir = VTransform(Utility::DIR_R, rot);
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = VTransform(Utility::DIR_U, rot);
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = VTransform(Utility::DIR_F, rot);
    DrawLineDir(pos, dir, 0x0000ff, len);

}

void Utility::DrawLineXYZ(const Position3DF& pos, const Quaternion& rot, float len)
{

    VECTOR dir;

    // X
    dir = rot.GetRight();
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = rot.GetUp();
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = rot.GetForward();
    DrawLineDir(pos, dir, 0x0000ff, len);

}


void Utility::RelativeQuaternion(double speed, const Quaternion& relativeQua, Quaternion& nowQua)
{
    double angle = Quaternion::Angle(relativeQua, nowQua);

    if (angle < speed)
    {
        nowQua = relativeQua;
    }
    else
    {
        nowQua = Quaternion::AngleAxis(angle, AXIS_Y);
    }
}

//�����n
VECTOR Utility::RandomVector(const int max)
{
    VECTOR ans = VECTOR_ZERO;
    int random = 0;

    random = GetRand(max);
    ans.x = static_cast<float>(random);

    random = GetRand(max);
    ans.y = static_cast<float>(random);

    random = GetRand(max);
    ans.z = static_cast<float>(random);

    return ans;
}

void Utility::RelativeVector(float speed, const VECTOR& relativeVec, VECTOR& nowVec)
{
    //�J�����̌��ݍ��W
    VECTOR NowPos = nowVec;

    //���X�ɗ��z�̍��W�ɋ߂Â��Ă���---------------
    //x�̒l
    //���z�̍��W�ƌ��݂̍��W�̍����J�����̑��x��菬�����Ȃ�
    //���z�̍��W�ɃX�i�b�v
    if (fabs(relativeVec.x - NowPos.x) < speed)
    {
        NowPos.x = relativeVec.x;
    }
    //�����J�����̑��x�ȏ�
    else
    {
        if ((relativeVec.x - NowPos.x) > 0)
        {
            NowPos.x += speed;
        }
        else
        {
            NowPos.x -= speed;
        }
    }
    //y�̒l
    //���z�̍��W�ƌ��݂̍��W�̍����J�����̑��x��菬�����Ȃ�
    //���z�̍��W�ɃX�i�b�v
    if (fabs(relativeVec.y - NowPos.y) < speed)
    {
        NowPos.y = relativeVec.y;
    }
    //�����J�����̑��x�ȏ�
    else
    {
        if ((relativeVec.y - NowPos.y) > 0)
        {
            NowPos.y += speed;
        }
        else
        {
            NowPos.y -= speed;
        }
    }
    //z�̒l
    //���z�̍��W�ƌ��݂̍��W�̍����J�����̑��x��菬�����Ȃ�
    //���z�̍��W�ɃX�i�b�v
    if (fabs(relativeVec.z - NowPos.z) < speed)
    {
        NowPos.z = relativeVec.z;
    }
    //�����J�����̑��x�ȏ�
    else
    {
        if ((relativeVec.z - NowPos.z) > 0)
        {
            NowPos.z += speed;
        }
        else
        {
            NowPos.z -= speed;
        }
    }
    //--------------------------------------------

    //���X�ɗ��z�̍��W�ɂȂ�
    nowVec = NowPos;
}

int Utility::Second2Minute(float second)
{
    int minute = 0;

    minute = static_cast<int>(second / 60.0f);

    return minute;
}

#pragma once
#include <map>
#include "SoundResource.h"

class SoundResourceManager
{
public:

	// サウンドリソース
	enum class SOUND_RESOURCE_ID
	{
		// BGM
		TITLE,
		GAME,

		// 効果音
		WALK,
		RUN,
		SWING,
		SWORD_ATTACK,
		DAMAGE,
		PARY,
		JUST_AVOID,
		DASH_SONIC,
		AVOID,
		JUMP,
		ELECTRO,
		THUNDER,
		SPARK,
		SLASH_FINISH,
		REPAIR,
		CHARGE,
		EVIL_GOA_SLASH,
		EVIL_GOA_BOM,
		EVIL_GOA_ROAR,
		EVIL_GOA_AWAKE_1,
		EVIL_GOA_AWAKE_2,
		METEO,

		ENEMY_FLINCH,
		ENEMY_DOWN,

		GAME_OVER,
	};

	//インスタンスの生成
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SoundResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);
	// リソースの完全破棄
	void Destroy(void);

	// ロードして、ハンドルIDを取得
	SoundResource Load(const SOUND_RESOURCE_ID& id);

	// NULLであるか
	static const bool IsNullptr(void);

private:

	// リソースデータ
	using ResourcesData = std::map<SOUND_RESOURCE_ID, SoundResource>;
	// ロード済みデータ
	using LoadedData = std::map<SOUND_RESOURCE_ID, SoundResource>;

private:

	// 静的インスタンス
	static SoundResourceManager* instance_;

	// リソース管理の対象
	ResourcesData resourcesMap_;

	// 読み込み済みリソース
	LoadedData loadedMap_;

	// privateでコンストラクタを生成する
	SoundResourceManager(void);
	// コピーコンストラクタも同様
	SoundResourceManager(const SoundResourceManager&);
	//デストラクタ
	~SoundResourceManager(void) = default;

	// 内部ロード
	SoundResource _Load(const SOUND_RESOURCE_ID& id);

};
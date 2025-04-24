#include <DxLib.h>
#include <thread>
#include "../../Application/Application.h"
#include "../../Application/Singleton_Manager/EffectResourceManager/EffectResourceManager.h"
#include "../../Application/Singleton_Manager/InputManager/InputManager.h"
#include "../../Application/Singleton_Manager/InputManager/SimpleInput.h"
#include "../../ACTOR/Player/Player.h"
#include "../../ACTOR/Enemy/EnemyDemon.h"
#include "../../ACTOR/Stage/StageRuins.h"
#include "../../ACTOR/SkyDome/SkyNight.h"
#include "GameScene.h"

#pragma region 定数宣言

// シーン遷移までの時間
const float SCENE_TRANSITION_TIME = 5.3f;

// ゲームオーバー画像を表示するまでの時間
const float GAME_OVER_IMAGE_TIME = 3.0f;

// PixcelRendererの枚数
const int PIXCELRENDERER_NUM = 1;

// 初期化子
const float INIT_ZERO = 0.0f;

// BGMの音量
const int BGM_VOLUME = 180;

// マルチスレッドの個数
const int INIT_COUNTER_MAX = 4;

// レートの最大値
const float LOAD_RATE_MAX = 1.0f;

// ロードゲージが溜まる速度
const float LOAD_GAGE_SPEED = 0.005f;

#pragma endregion

GameScene::GameScene(void):
	actorCollisionId_(ACTOR_ID::NONE),
	battleStep_(BATTLE_STEP::NONE_BATTLE),
	isLoadData_(true),
	loadGageRate_(0.0f),
	loadGageRateMax_(0.0f),
	isSceneStop_(false)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Release(void)
{
	for (const auto& actor : actors_)
		actor->Release();
}

void GameScene::LoadSceneData(void)
{
	const Color3D color = { 0, 0, 0};

	LoadData::LoadDataInfo info = { "GameSceneInfo.json", "GameOverBack"};

	imgGameOver_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::GAMEOVER_BACK);

	info.dataName = "LoadBack";
	imgLoadBack_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::LOAD_BACK);

	info.dataName = "LoadGage";
	imgLoadGage_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::LOAD_GAGE, Image::IMAGE_TYPE::GAGE);
	imgLoadGage_->SetUpImageGageRate(loadGageRate_, LOAD_RATE_MAX, color);
	
	imgLoadGageCase_ = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::LOAD_GAGE_CASE);

	info.dataName = "LoadNumbers";
	imgLoadPercent_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS);

	info.dataName = "LoadSymbols";
	imgLoadSymbol_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::SYMBOLS);
}

void GameScene::StopGameScene(void)
{
	isSceneStop_ = true;
}

void GameScene::PlayGameScene(void)
{
	isSceneStop_ = false;
}

void GameScene::AddGagePercent(const float rate)
{
	static int timeLoop = 0;

	// 10000000回に一度ゲージを増やす
	if (!(timeLoop % 10000000))
	{
		loadGageRate_ += rate;
		timeLoop = 0;
	}

	timeLoop++;

	if (loadGageRate_ > loadGageRateMax_)
		loadGageRate_ = loadGageRateMax_;
}

void GameScene::AddGagePercentMax(const float rateMax)
{
	loadGageRateMax_ += rateMax;
	if (loadGageRateMax_ > LOAD_RATE_MAX)
		loadGageRateMax_ = LOAD_RATE_MAX;
}

void GameScene::SetUpActors(void)
{
	// 必ず、ACTOR_IDの順番
	actors_.reserve(ALL_ACTOR_NUM);
	actors_.push_back(std::make_unique<Player>());
	actors_.push_back(std::make_unique<EnemyDemon>());
	actors_.push_back(std::make_unique<StageRuins>());
	actors_.push_back(std::make_unique<SkyNight>());
}

void GameScene::InitSpecial(void)
{
	fader_ = std::make_unique<Fader>();

	pause_ = std::make_unique<PauseMenyu>();
	pause_->Init();

	battleManager_ = std::make_unique<BattleManager>();


	SetUpActors();
	InitBattleManager();

	InitAddSpecial();

	// マルチスレッド
	// 事前に呼ばないと、内部的にマルチスレッドにならない
	SetUseASyncLoadFlag(true);

	// アクター初期化
	std::thread initTh([&]()
		{
			for (const auto& actor : actors_)
			{
				float param = LOAD_RATE_MAX / static_cast<float>(INIT_COUNTER_MAX);
				AddGagePercentMax(param);
				actor->Init();
			}
		}
	);

	// ロードゲージの増加
	std::thread addGage([&]()
		{
			while (loadGageRate_ < LOAD_RATE_MAX)
				AddGagePercent(LOAD_GAGE_SPEED);
		}
	);

	initTh.detach();
	addGage.detach();
}

void GameScene::InitAddSpecial(void)
{
}

void GameScene::InitShadowShader(void)
{
	if (actors_.size() <= 0)return;

	// ステージアクターのIDをセット
	shadowVertex_ = std::make_unique<ModelShadowRenderer>(actors_[2]->GetModelId());
	shadowVertex_->SetShaderInfo(ModelRenderer::SHADER_NAME::NORM_SHADOW,
		ModelShadowRenderer::SHADOW_BUF_SIZE_PS,
		ModelShadowRenderer::SHADOW_BUF_SIZE_VS,
		ModelRenderer::BUF_SIZE_ZERO);

	// ライトの方向
	Dir3DF lightDir = SceneManager::GetInstance().GetLightDir();
	shadowVertex_->AddConstBufPS(FLOAT4(lightDir.x, lightDir.y, lightDir.z,
		ModelRenderer::SHADER_NONE_VALUE));
	// ライトの色
	shadowVertex_->AddConstBufPS(FLOAT4(
		ModelRenderer::FEW_BLACK_COLOR.x,
		ModelRenderer::FEW_BLACK_COLOR.y,
		ModelRenderer::FEW_BLACK_COLOR.z,
		ModelRenderer::SHADER_NONE_VALUE));
	// 拡散光
	shadowVertex_->AddConstBufPS(FLOAT4(
		ModelRenderer::DEFAULT_DIFFUSE_COLOR.x,
		ModelRenderer::DEFAULT_DIFFUSE_COLOR.y,
		ModelRenderer::DEFAULT_DIFFUSE_COLOR.z,
		ModelRenderer::SHADER_NONE_VALUE));
	// 環境光
	shadowVertex_->AddConstBufPS(FLOAT4(
		ModelRenderer::DEFAULT_SPECULAR_COLOR.x,
		ModelRenderer::DEFAULT_SPECULAR_COLOR.y,
		ModelRenderer::DEFAULT_SPECULAR_COLOR.z,
		ModelRenderer::SHADER_NONE_VALUE));

	// ライトの強さ
	// カメラ
	Position3DF cameraPos = SceneManager::GetInstance().GetCamera().lock()->GetPos();
	shadowVertex_->AddConstBufPS(FLOAT4(
		ModelRenderer::DIRECTION_LIGHT_POWER_MEDIUM,
		cameraPos.x,
		cameraPos.y,
		cameraPos.z));


	// それぞれのタイプに合わせて登録
	shadowVertex_->AddSkinDepthModel(actors_[0]->GetModelId());
	shadowVertex_->AddSkinDepthModel(actors_[1]->GetModelId());
	shadowVertex_->AddDepthModel(actors_[2]->GetModelId());

	// ラフネステクスチャを追加
	shadowVertex_->SetTextureBuf(11, ResourceManager::GetInstance().Load(
		ResourceManager::RESOURCE_ID::RUINS_ROUGHNESS).handleId_);

}

void GameScene::InitBattleManager(void)
{
	// バトルマネージャーに条件式を追加
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::NONE_BATTLE),
		[&]() { return actors_[2]->BattleStepCondition(); });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE_START),
		[&]() { return actors_[1]->BattleStepCondition(); });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE),
		[&]() { return actors_[0]->BattleStepCondition() || actors_[1]->BattleStepCondition(); });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE_WIN),
		[&]() { return actors_[0]->BattleStepCondition(); });
	battleManager_->AddCondition(static_cast<int>(BATTLE_STEP::BATTLE_GAMEOVER),
		[]() { return false; });

}

void GameScene::InitActors(void)
{
	// 初期化処理
	for (int ii = 0; ii < ALL_ACTOR_NUM; ii++)
		actors_[ii]->InitSpecial();

	// プレイヤー全てに他アクターのスローモーション関数ポインタを追加
	for (int jj = 1; jj < ALL_ACTOR_NUM; jj++)
		actors_[0]->SetOtherActorSlowFunc(actors_[jj]->GetMySlowFunc());

	// 当たり判定
	actorCollisionId_ = static_cast<ACTOR_ID>(actorCollisionId_ | ACTOR_ID::PLAYER);
	actorCollisionId_ = static_cast<ACTOR_ID>(actorCollisionId_ | ACTOR_ID::ENEMY);
	actorCollisionId_ = static_cast<ACTOR_ID>(actorCollisionId_ | ACTOR_ID::STAGE);
}

void GameScene::SetUpActorsCollision(void)
{
	long nowCountNum = 0;// 連続する配列の現在の要素番号

	// 必ず、ACTOR_IDの順番
	SetCollision(nowCountNum, 1, actorCollisionId_ & ACTOR_ID::PLAYER);
	SetCollision(nowCountNum, 1, actorCollisionId_ & ACTOR_ID::ENEMY);
	SetCollision(nowCountNum, 1, actorCollisionId_ & ACTOR_ID::STAGE);
}

void GameScene::SetCollision(long& nowCounter, const long size, bool isCanSetCollision)
{
	if(!isCanSetCollision)
	{
		nowCounter += size;
		return;
	}

	for (int ii = nowCounter; ii < nowCounter + size; ii++)
	{
		// 自分より後のアクター
		for (int ee = nowCounter - 1; ee >= 0; ee--)
			actors_[ii]->SetOtherActorCollision(actors_[ee]->GetMyCollision(), actors_[ee]->GetAddSpecialActorCollision());


		// 自分より先のアクター
		for (int jj = nowCounter + size; jj < ALL_ACTOR_NUM; jj++)
			actors_[ii]->SetOtherActorCollision(actors_[jj]->GetMyCollision(), actors_[jj]->GetAddSpecialActorCollision());
	}

	nowCounter += size;
}

void GameScene::SetUpActorsUi(void)
{
	for (int ii = 0; ii < ALL_ACTOR_NUM; ii++)
	{
		for(const auto& ui : actors_[ii]->GetImagesUi())
			actorsUi_.push_back(ui);
	}
}

void GameScene::InitSoundPlayLaunch(void)
{
	soundPlayLaunch_ = std::make_unique<SoundPlayLaunch>();

	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::GAME;
	soundPlayLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::GAME_OVER;
	soundPlayLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);
}

void GameScene::PlayBgm(void)
{
	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::GAME;

	if (soundPlayLaunch_->IsPlaySound(id))return;

	soundPlayLaunch_->Play(id, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(BGM_VOLUME, soundPlayLaunch_->GetHandleId(id));
}

void GameScene::Pause(void)
{
	if(pause_->IsOpen())
		pause_->Update();

	auto& input = SimpleInput::GetInstance();
	if (input.GetInput("pause") && !isSceneStop_)
	{
		InputManager::GetInstance().SetMousePos({ 0, 0 });
		pause_->SetMenyuIsOpen(!pause_->IsOpen());
	}
}

void GameScene::DrawPercent(void)
{
	const float HUNDRED_PERCENT = 100.0f;

	//残りゲージの割合を100倍
	int rate = static_cast<int>(loadGageRate_ * HUNDRED_PERCENT);

	Position2D pos = imgLoadPercent_->GetPos();

	pos.x -= imgLoadPercent_->GetSize().x;

	//数字を描画
	//百の位
	if (rate / static_cast<int>(HUNDRED_PERCENT))
	{
		imgLoadPercent_->DrawImageDiv(rate / static_cast<int>(HUNDRED_PERCENT), pos);
		rate = rate % static_cast<int>(HUNDRED_PERCENT);
	}

	pos.x += imgLoadPercent_->GetSize().x;

	//十の位
	const int TEN_RANK = 10;

	imgLoadPercent_->DrawImageDiv(rate / TEN_RANK, pos);
	rate = rate % TEN_RANK;

	pos.x += imgLoadPercent_->GetSize().x;

	//一の位
	imgLoadPercent_->DrawImageDiv(rate, pos);
}

void GameScene::UpdateSpecial(void)
{
	fader_->Update();

	// フェード中は何もしない
	if (!fader_->IsFadeEnd())return;

	// ローディング処理中ならこれ以降処理しない
	if (isLoadData_)
	{
		SceneManager::GetInstance().GetCamera().lock()->LockIsCameraUpdate();

		if (loadGageRate_ >= LOAD_RATE_MAX)
		{
			// マルチスレッドの終了
			SetUseASyncLoadFlag(false);

			isLoadData_ = false;
			InitActors();
			SetUpActorsCollision();
			SetUpActorsUi();

			// 影表現シェーダ
			InitShadowShader();

			// 一度フェードする
			fader_->FadeStart();

			SceneManager::GetInstance().GetCamera().lock()->UnLockIsCameraUpdate();
		}

		return;
	}


	if (!pause_->IsOpen() && !isSceneStop_)
	{
		for (const auto& actor : actors_)
			actor->Update();

		battleManager_->Update();
		battleStep_ = static_cast<BATTLE_STEP>(battleManager_->GetNowBattleState());
	}

	if (battleStep_ == BATTLE_STEP::BATTLE_START)
		for (const auto& actor : actors_)
			actor->ActorBattleStart();

	if (battleStep_ == BATTLE_STEP::BATTLE)
	{
		PlayBgm();
	}

	// バトル終了後はこれ以降処理しない
	if (battleStep_ == BATTLE_STEP::BATTLE_WIN ||
		battleStep_ == BATTLE_STEP::BATTLE_GAMEOVER)
	{
		if (soundPlayLaunch_->IsPlaySound(SoundResourceManager::SOUND_RESOURCE_ID::GAME))
			soundPlayLaunch_->Stop(SoundResourceManager::SOUND_RESOURCE_ID::GAME);


		nextSceneTime_.UpdateTime();
		if (nextSceneTime_.GetTotalNowTime() > SCENE_TRANSITION_TIME)
		{
			if (battleStep_ == BATTLE_WIN)
			{
				SetUpEnemyLevelResource();
				nextSceneId_ = SceneID::RESULT;
			}
			else
				nextSceneId_ = SceneID::TITLE;
		}
		else if (nextSceneTime_.GetTotalNowTime() < GAME_OVER_IMAGE_TIME)
		{
			for (const auto& actor : actors_)
				actor->SetUpMySlow(true);
		}
		else
		{
			for (const auto& actor : actors_)
				actor->SetUpMySlow(false);

			if (battleStep_ == BATTLE_GAMEOVER)
				soundPlayLaunch_->Play(SoundResourceManager::SOUND_RESOURCE_ID::GAME_OVER,
					DX_PLAYTYPE_BACK);
		}
		return;
	}

	UpdateAddSpecial();

	Pause();
}

void GameScene::UpdateAddSpecial(void)
{
}

void GameScene::SetUpEnemyLevelResource(void)
{
	SceneManager::SelectSceneToGameScene tempData = SceneManager::GetInstance().GetSelectToGameSceneData();
	SceneManager::GameSceneToResultScene data = SceneManager::GetInstance().GetGameToResultSceneData();

	if(tempData.enemyLevel == ActorBase::ENEMY_LEVEL_EASY)
		data.levelResource = ResourceManager::RESOURCE_ID::LEVEL_EASY;
	else if(tempData.enemyLevel == ActorBase::ENEMY_LEVEL_NORMAL)
		data.levelResource = ResourceManager::RESOURCE_ID::LEVEL_NORMAL;
	else if(tempData.enemyLevel == ActorBase::ENEMY_LEVEL_HARD)
		data.levelResource = ResourceManager::RESOURCE_ID::LEVEL_HARD;

	SceneManager::GetInstance().SetUpGameToResultScene(data);

}

void GameScene::DrawSpecial(void)
{
	if (isLoadData_ || fader_->IsFadeOut())
	{
		imgLoadBack_->DrawImage();
		imgLoadSymbol_->DrawImageDiv(0);
		imgLoadGage_->DrawTypeImage();
		DrawPercent();
		fader_->Draw();
		return;
	}

	for (const auto& actor : actors_)
		actor->Draw();

	shadowVertex_->Draw();

	for (const auto& ui : actorsUi_)
		ui->DrawTypeImage();

	for (const auto& actor : actors_)
		actor->DrawIconRecastTime();

	DrawAllEffect();

	pause_->Draw();

	if (battleStep_ == BATTLE_GAMEOVER)
	{
		if (nextSceneTime_.GetTotalNowTime() > GAME_OVER_IMAGE_TIME)
		{
			imgGameOver_->DrawImage();
		}
	}

	if(!pause_->IsOpen())DrawAddSpecial();

	fader_->Draw();
}

void GameScene::DrawAddSpecial(void)
{
}

void GameScene::DrawAllEffect(void)
{
	// エフェクトの描画処理
	auto& effectManager = EffectResourceManager::GetInstance();
	//カメラの状態が最新のときにカメラ同期するため
	//エフェクトマネージャの更新をここで行う
	if(!pause_->IsOpen())effectManager.Update();

	effectManager.Draw3DEffect();
}
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

	// コンストラクタ
	SceneBase(void);
	// デストラクタ
	virtual ~SceneBase(void);


	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);

	virtual void Release(void);

	// データのロード
	virtual void LoadSceneData(void);

protected:

	std::vector<std::unique_ptr<ImageGUI>>guiImages_;

	// PixcelRenderer
	std::unique_ptr<PixelRenderer>pixcelRenderer_;

	// SoundLauncher
	std::unique_ptr<SoundPlayLaunch>soundPlayLaunch_;

	// 数字画像
	std::unique_ptr<ImageDiv> imgNumber_;
	// 記号画像
	std::unique_ptr<ImageDiv> imgResultSymbol_;

	// ダメージ数字画像用の座標情報
	Position2D damageNumberPos_;


	// アクター全体に及ぶ頂点シェーダ
	std::unique_ptr<ModelShadowRenderer>shadowVertex_;



	// GUIの表示状態
	bool isGuiDraw_;

	// 遷移先シーンID
	SceneID::SCENE_ID nextSceneId_;

	void InitNumbersImage(LoadData::LoadDataInfo info);

	// 特別性の高い初期化処理
	virtual void InitSpecial(void);

	// 特別性の高い更新
	virtual void UpdateSpecial(void);

	// サウンドのランチャーを初期化
	virtual void InitSoundPlayLaunch(void);
	// BGMの再生処理
	virtual void PlayBgm(void);

	void SetUpGUI(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id, 
		const std::function<bool(void)>& conditionFunc, const std::function<void(void)>& runFunc);

	void AddGUICondition(const int number, const std::function<bool(void)>& conditionFunc);

	// PixcelRendererの初期化
	virtual void InitPixcelRenderer(void);
	// PixcelRendererの更新
	virtual void UpdatePixcelRenderer(void);
	void DrawPixcelRenderer(void);

	// 影シェーダの初期化
	virtual void InitShadowShader(void);
	void UpdateShadowShader(void);

	// 背景画像の描画
	virtual void DrawBack(void);

	// 特別性の高い描画
	virtual void DrawSpecial(void);

	// 現在のタイムを描画
	void DrawTime(const float param);
	// ダメージ量を描画
	void DrawDamage(const float param);

private:

	bool SceneTransition(void);

};
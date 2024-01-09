#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	DebugText* debugText_ = nullptr;

	void GamePlayUpdate();     // ゲームプレイ更新
	void TitleUpdate();        // タイトル更新
	void ManualUpdate();       // マニュアル更新
	void GameOverUpdate();     // ゲームオーバー更新
	void GameClearUpdate();    // ゲームクリア更新
	void CollisionPistonBox(); // 衝突判定
	int sceneMode_ = 0; // 0タイトル1プレイ2ゲームオーバー3ゲームクリア4マニュアル
	void GamePlayStart(); // 初期化

	// ボックス(不良品)
	uint32_t textureHandleBox_ = 0;
	Model* modelBox_ = nullptr;
	WorldTransform worldTransformBox_[5];
	int BoxFlag[5] = {};
	void BoxUpdate();
	float BoxSpeed[5] = {};
	// ボックス(正規品)
	uint32_t textureHandleBox2_ = 0;
	Model* modelBox2_ = nullptr;
	WorldTransform worldTransformBox2_[5];
	int Box2Flag[5] = {};
	void Box2Update();
	float Box2Speed[5] = {};
	void BoxBorn();
	void BoxMove();

	// 押し出し機
	uint32_t textureHandlePiston_ = 0;
	Model* modelPiston_ = nullptr;
	WorldTransform worldTransformPiston_;
	// 押し出し機2
	uint32_t textureHandlePiston2_ = 0;
	Model* modelPiston2_ = nullptr;
	WorldTransform worldTransformPiston2_;
	int PistonFlag = 0;
	void PistonUpdate();
	// ゲームオーバー
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;
	// ゲームクリア
	uint32_t textureHandleGameClear_ = 0;
	Sprite* spriteGameClear_ = nullptr;
	// ステージ
	uint32_t textureHandleStage_ = 0;
	Sprite* spriteStage_ = nullptr;

	// マニュアル
	uint32_t textureHandleManual_ = 0;
	Sprite* spriteManual_ = nullptr;

	// タイトル
	uint32_t textureHandleTitle1_ = 0;
	Sprite* spriteTitle1_ = nullptr;
	// タイトル
	uint32_t textureHandleTitle2_ = 0;
	Sprite* spriteTitle2_ = nullptr;
	// タイトル
	uint32_t textureHandleTitle3_ = 0;
	Sprite* spriteTitle3_ = nullptr;
	int TitleTimer_ = 0;

	// スコア数値
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[5] = {};
	void DrawScore();
	uint32_t textureHandleScore_ = 0;
	Sprite* spriteScore_ = 0;
	int gameScore_ = 0;

	int timer_ = 50;
	float Gametimer_ = 0;

	// サウンド
	uint32_t soundTitleBGM_ = 0;     // タイトルBGM
	uint32_t soundGamePlayBGM_ = 0;  // ゲームプレイBGM
	uint32_t soundGameOverBGM_ = 0;  // ゲームオーバーBGM
	uint32_t soundGameClearBGM_ = 0; // ゲームクリアBGM
	uint32_t soundPushSE_ = 0;       // 押し出し機SE
	uint32_t VoiceHandleBGM_ = 0;    // 音声再生ハンドル

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

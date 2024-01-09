#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelBox_;    // ボックス
	delete modelPiston_; // 押し出し機
	delete modelBox2_;
	delete modelPiston2_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 30;
	viewProjection_.translation_.z = 0;
	viewProjection_.rotation_ = {3.14f / 2.0f, 0.0f, 0.0f};
	viewProjection_.Initialize();
	// 不良品
	textureHandleBox_ = TextureManager::Load("box.png");
	modelBox_ = Model::Create();
	for (int i = 0; i < 5; i++) {
		worldTransformBox_[i].scale_ = {1.0f, 1.0f, 1.0f};
		worldTransformBox_[i].translation_ = {-20, 10, 2};
		worldTransformBox_[i].Initialize();
	}
	// 正規品
	textureHandleBox2_ = TextureManager::Load("box2.png");
	modelBox2_ = Model::Create();
	for (int i = 0; i < 5; i++) {
		worldTransformBox2_[i].scale_ = {1.0f, 1.0f, 1.0f};
		worldTransformBox2_[i].translation_ = {-20, 10, 2};
		worldTransformBox2_[i].Initialize();
	}
	// 押し出し機
	textureHandlePiston_ = TextureManager::Load("piston.png");
	modelPiston_ = Model::Create();
	worldTransformPiston_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformPiston_.translation_ = {7, 10, -1};
	worldTransformPiston_.Initialize();
	// 押し出し機2
	textureHandlePiston2_ = TextureManager::Load("piston2.png");
	modelPiston2_ = Model::Create();
	worldTransformPiston2_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformPiston2_.translation_ = {7, 10, 0.3f};
	worldTransformPiston2_.Initialize();
	// タイトル
	textureHandleTitle1_ = TextureManager::Load("GameTitle1.png");
	spriteTitle1_ = Sprite::Create(textureHandleTitle1_, {0, 0});
	// タイトル
	textureHandleTitle2_ = TextureManager::Load("GameTitle2.png");
	spriteTitle2_ = Sprite::Create(textureHandleTitle2_, {0, 0});
	// タイトル
	textureHandleTitle3_ = TextureManager::Load("GameTitle3.png");
	spriteTitle3_ = Sprite::Create(textureHandleTitle3_, {0, 0});
	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.png");
	spriteStage_ = Sprite::Create(textureHandleStage_, {0, 0});
	// マニュアル
	textureHandleManual_ = TextureManager::Load("manual.png");
	spriteManual_ = Sprite::Create(textureHandleManual_, {0, 0});
	// スコア数値
	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i] = Sprite::Create(textureHandleNumber_, {300.0f + i * 26, 0});
	}
	textureHandleScore_ = TextureManager::Load("score.png");
	spriteScore_ = Sprite::Create(textureHandleScore_, {150, 0});
	// ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});
	// ゲームクリア
	textureHandleGameClear_ = TextureManager::Load("gameclear.png");
	spriteGameClear_ = Sprite::Create(textureHandleGameClear_, {0, 0});
	// サウンドデータの読み込み
	soundTitleBGM_ = audio_->LoadWave("Audio/Title.mp3");
	soundGamePlayBGM_ = audio_->LoadWave("Audio/stage.mp3");
	soundGameOverBGM_ = audio_->LoadWave("Audio/Gameover.mp3");
	soundGameClearBGM_ = audio_->LoadWave("Audio/GameClear.mp3");
	soundPushSE_ = audio_->LoadWave("Audio/osidas.mp3");

	// タイトルBGMを再生
	audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
	VoiceHandleBGM_ = audio_->PlayWave(soundTitleBGM_, true);
}

void GameScene::Update() {
	// 各シーンの更新処理
	switch (sceneMode_) {
	case 0:

		TitleUpdate(); // タイトル更新
		break;
	case 1:
		GamePlayUpdate(); // ゲームプレイ更新

		break;
	case 2:
		GameOverUpdate(); // ゲームオーバー更新

		break;
	case 3:
		GameClearUpdate(); // ゲームクリア更新
		break;
	case 4:
		ManualUpdate(); // マニュアル更新
		break;
	}
}
// 初期化
void GameScene::GamePlayStart() {
	for (int i = 0; i < 5; i++) {
		worldTransformBox_[i].translation_.x = -20;
		worldTransformBox_[i].translation_.z = 2;
	}
	for (int i = 0; i < 5; i++) {
		BoxFlag[i] = 0;
	}
	for (int i = 0; i < 5; i++) {
		worldTransformBox2_[i].translation_.x = -20;
		worldTransformBox2_[i].translation_.z = 2;
	}
	for (int i = 0; i < 5; i++) {
		Box2Flag[i] = 0;
	}

	BoxUpdate();
	Box2Update();
	gameScore_ = 0;
	timer_ = 50;
	Gametimer_ = 0;
}
// 更新
void GameScene::GamePlayUpdate() {
	BoxUpdate();
	Box2Update();
	PistonUpdate();
	CollisionPistonBox();
}
// タイトル更新
void GameScene::TitleUpdate() {
	TitleTimer_ += 1;
	if (TitleTimer_ >= 20) {
		TitleTimer_ = 0;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		GamePlayStart();
		sceneMode_ = 4;
		// ゲームBGMを再生
		audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
		VoiceHandleBGM_ = audio_->PlayWave(soundGamePlayBGM_, true);
	}
}

// マニュアル更新
void GameScene::ManualUpdate() {

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneMode_ = 1;
	}
}
// ゲームオーバー更新
void GameScene::GameOverUpdate() {

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneMode_ = 0;
		// タイトルBGMを再生
		audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
		VoiceHandleBGM_ = audio_->PlayWave(soundTitleBGM_, true);
	}
}
// ゲームクリア更新
void GameScene::GameClearUpdate() {

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneMode_ = 0;
		// タイトルBGMを再生
		audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
		VoiceHandleBGM_ = audio_->PlayWave(soundTitleBGM_, true);
	}
}
// 不良品
void GameScene::BoxUpdate() {

	BoxBorn();
	BoxMove();

	for (int i = 0; i < 5; i++) {
		// 変換行列を更新
		worldTransformBox_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformBox_[i].scale_, worldTransformBox_[i].rotation_,
		    worldTransformBox_[i].translation_);
		// 変換行列を定数バッファに転送
		worldTransformBox_[i].TransferMatrix();
	}
}
// 正規品
void GameScene::Box2Update() {

	BoxBorn();
	BoxMove();

	for (int i = 0; i < 5; i++) {
		// 変換行列を更新
		worldTransformBox2_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformBox2_[i].scale_, worldTransformBox2_[i].rotation_,
		    worldTransformBox2_[i].translation_);
		// 変換行列を定数バッファに転送
		worldTransformBox2_[i].TransferMatrix();
	}
}
// 箱発生
void GameScene::BoxBorn() {
	timer_ -= 1;
	if (timer_ <= 0) {
		// 不良品
		if (rand() % 100 == 0) {
			for (int i = 0; i < 5; i++) {
				if (BoxFlag[i] == 0) {
					worldTransformBox_[i].translation_.x = -20;
					worldTransformBox_[i].translation_.z = 2;
					BoxFlag[i] = 1;
					timer_ = 50;
					break;
				}
			}
		}
		// 正規品
		if (rand() % 150 == 0) {
			for (int i = 0; i < 5; i++) {
				if (Box2Flag[i] == 0) {
					worldTransformBox2_[i].translation_.x = -20;
					worldTransformBox2_[i].translation_.z = 2;
					Box2Flag[i] = 1;
					timer_ = 50;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		if (worldTransformBox_[i].translation_.x >= 20 &&
		    worldTransformBox_[i].translation_.z == 5) {

			BoxFlag[i] = 0;
		}
	}

	for (int i = 0; i < 5; i++) {
		if (worldTransformBox2_[i].translation_.x >= 20 &&
		    worldTransformBox2_[i].translation_.z == 2) {

			Box2Flag[i] = 0;
		}
	}
}
void GameScene::BoxMove() {
	// 不良品移動
	for (int i = 0; i < 5; i++) {
		if (BoxFlag[i] == 1) {
			Gametimer_ += 0.1f;
			BoxSpeed[i] = 0.05f;
			worldTransformBox_[i].translation_.x += BoxSpeed[i];
			worldTransformBox_[i].translation_.x += Gametimer_ / 75000.0f;
		}
		if (worldTransformBox_[i].translation_.x >= 20 &&
		    worldTransformBox_[i].translation_.z == 2) {
			sceneMode_ = 3;
			// ゲームクリアBGMを再生
			audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
			VoiceHandleBGM_ = audio_->PlayWave(soundGameClearBGM_, true);
		}
	}

	// 正規品移動
	for (int i = 0; i < 5; i++) {
		if (Box2Flag[i] == 1) {
			Gametimer_ += 0.1f;
			Box2Speed[i] = 0.05f;
			worldTransformBox2_[i].translation_.x += Box2Speed[i];
			worldTransformBox2_[i].translation_.x += Gametimer_ / 75000.0f;
		}
		if (worldTransformBox2_[i].translation_.x >= 20 &&
		    worldTransformBox2_[i].translation_.z == 5) {
			sceneMode_ = 3;
			// ゲームクリアBGMを再生
			audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
			VoiceHandleBGM_ = audio_->PlayWave(soundGameClearBGM_, true);
		}
	}
}

void GameScene::PistonUpdate() {

	if (input_->PushKey(DIK_SPACE)) {
		PistonFlag = 1;
		worldTransformPiston2_.translation_.z = 1;
		// プッシュSE
		audio_->PlayWave(soundPushSE_);
	} else {
		worldTransformPiston2_.translation_.z = 0.3f;
	}

	// 変換行列を更新
	worldTransformPiston_.matWorld_ = MakeAffineMatrix(
	    worldTransformPiston_.scale_, worldTransformPiston_.rotation_,
	    worldTransformPiston_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPiston_.TransferMatrix();
	// 変換行列を更新
	worldTransformPiston2_.matWorld_ = MakeAffineMatrix(
	    worldTransformPiston2_.scale_, worldTransformPiston2_.rotation_,
	    worldTransformPiston2_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPiston2_.TransferMatrix();
}

void GameScene::CollisionPistonBox() {
	for (int i = 0; i < 5; i++) {
		if (BoxFlag[i] == 1) {
			if (PistonFlag == 1) {
				// 差を求める
				float ex = abs(
				    worldTransformPiston2_.translation_.x - worldTransformBox_[i].translation_.x);
				float ez = abs(
				    worldTransformPiston2_.translation_.z - worldTransformBox_[i].translation_.z);
				// 衝突したら
				if (ex <= 1 && ez <= 1) {
					worldTransformBox_[i].translation_.z += 3;
					gameScore_ += 100;
				}
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		if (Box2Flag[i] == 1) {
			if (PistonFlag == 1) {
				// 差を求める
				float ex2 = abs(
				    worldTransformPiston2_.translation_.x - worldTransformBox2_[i].translation_.x);
				float ez2 = abs(
				    worldTransformPiston2_.translation_.z - worldTransformBox2_[i].translation_.z);
				// 衝突したら
				if (ex2 <= 1 && ez2 <= 1) {
					worldTransformBox2_[i].translation_.z += 3;
				}
			}
		}
	}
	// if (gameScore_ >= 2000) {
	//	    sceneMode_ = 3;
	//	    // ゲームクリアBGMを再生
	//	    audio_->StopWave(VoiceHandleBGM_); // 現在のBGMを停止
	//	    VoiceHandleBGM_ = audio_->PlayWave(soundGameClearBGM_, true);
	// }
}
// スコア表示
void GameScene::DrawScore() {

	int eachNumber[5] = {};  // 各桁の値
	int number = gameScore_; // 表示する数字
	int keta = 10000;        // 桁

	for (int i = 0; i < 5; i++) {

		eachNumber[i] = number / keta; // 今の桁の値
		number = number % keta;        // 次の桁以下の値を取り出す
		keta = keta / 10;              // 桁を進める
	}
	for (int i = 0; i < 5; i++) {

		spriteNumber_[i]->SetSize({32, 64});
		spriteNumber_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		spriteNumber_[i]->Draw();
	}
	spriteScore_->Draw();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	switch (sceneMode_) {
	case 0:
		if (TitleTimer_ <= 5) {
			spriteTitle1_->Draw();
		}
		if (TitleTimer_ >= 5 && TitleTimer_ <= 10) {
			spriteTitle2_->Draw();
		}
		if (TitleTimer_ >= 10) {
			spriteTitle3_->Draw();
		}
		break;
	case 1:
		spriteStage_->Draw();
		break;
	case 2:
		// ゲームオーバー
		spriteGameOver_->Draw();
		break;
	case 3:
		// ゲームクリア
		spriteGameClear_->Draw();
		break;
	case 4:
		spriteManual_->Draw();
		break;
	}
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	switch (sceneMode_) {
	case 0:

		break;
	case 1:
		modelPiston_->Draw(worldTransformPiston_, viewProjection_, textureHandlePiston_);
		if (PistonFlag == 1) {
			if (input_->PushKey(DIK_SPACE)) {
				modelPiston2_->Draw(worldTransformPiston2_, viewProjection_, textureHandlePiston2_);
			}
		}
		for (int i = 0; i < 5; i++) {
			if (BoxFlag[i] == 1) {
				modelBox_->Draw(worldTransformBox_[i], viewProjection_, textureHandleBox_);
			}
		}
		for (int i = 0; i < 5; i++) {
			if (Box2Flag[i] == 1) {
				modelBox2_->Draw(worldTransformBox2_[i], viewProjection_, textureHandleBox2_);
			}
		}
		break;

	case 2:

		break;
	case 3:

		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (sceneMode_) {
	case 0:

		break;

	case 1:

		DrawScore();
		break;
	case 2:

		break;
	case 3:
		DrawScore();
		break;
	case 4:
		break;
	}
	debugText_->DrawAll();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LD2A_02_ワダ_ケイタ";

struct Vector3 {
	float x, y, z;
};

struct Matrix4x4 {
	float m[4][4];
	float x;
};

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int column = 0; column < 4; column++) {
		for (int row = 0; row < 4; row++) {

			result.m[column][row] = (m1.m[column][0] * m2.m[0][row]) + (m1.m[column][1] * m2.m[1][row]) + (m1.m[column][2] * m2.m[2][row]) + (m1.m[column][3] * m2.m[3][row]);

		}
	}

	return result;
};

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = { 0 };

	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
};

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = { 0 };

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
};

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {
		{
			{ 1, 0, 0, 0 },
			{ 0, std::cos(radian), std::sin(radian), 0 },
			{ 0, -std::sin(radian), std::cos(radian), 0 },
			{ 0, 0, 0, 1 }
		},
	};

	return result;
};

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {
		{
			{ std::cos(radian), 0, -std::sin(radian), 0 },
			{ 0, 1, 0, 0 },
			{ std::sin(radian), 0, std::cos(radian), 0 },
			{ 0, 0, 0, 1 }
		},
	};

	return result;
};

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {
		{
			{ std::cos(radian), std::sin(radian), 0, 0 },
			{ -std::sin(radian), std::cos(radian), 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		},
	};

	return result;
};


// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = { 0 };

	result = Multiply(MakeScaleMatrix(scale), Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z))));
	result = Multiply(result, MakeTranslateMatrix(translate));

	return result;
};

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight + 1, "%6.02f", matrix.m[row][column]);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Vector3 scale{ 1.2f, 0.79f, -2.1f };
		Vector3 rotate{ 0.4f, 1.43f, -0.8f };
		Vector3 translate{ 2.7f, -4.15f, 1.57f };

		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

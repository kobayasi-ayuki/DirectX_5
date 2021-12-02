/*
objファイルのパーサー(パーサーとは特定のファイルデータを読み込むイメージ)
*/

// Include
#include<vector>
#include "DxLib.h"

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpcmdLine, int nCmdShow)
{
    // ウィンドウモードに設定
    ChangeWindowMode(true);
    // 背景色の変更
    SetBackgroundColor(50, 50, 50);

    // DXライブラリの初期化
    if (DxLib_Init() == -1) return 0;

    // 裏画面に描画する
    SetDrawScreen(DX_SCREEN_BACK);

    // ライト無効
    SetUseLighting(false);

    // Zバッファ有効
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);

    // カメラ座標と注視点の設定
    SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 1.0f, -2.0f), VGet(0.0f, 0.0f, 0.0f));

    // カメラNEARとFARの設定
    SetCameraNearFar(1.0f, 100.0f);

    // ファイルを開く
    FILE* fp = fopen("cube.obj", "r");

    // NULLチェック
    if (!fp) { return 0; }

    // 頂点データとインデックス値
    std::vector<VERTEX3D> vertex;
    std::vector<unsigned short> index;

    while (!feof(fp))
    {
        char key[MAX_PATH] = "";

        // キーワード読み込み
        fscanf(fp, "%s ", key);

        // キーワードは'v'か
        if (!strcmp(key, "v"))
        {
            // 頂点構造体を作成
            VERTEX3D v = { 0 };

            // 色はランダム
            v.dif = GetColorU8(GetRand(255), GetRand(255), GetRand(255), 255);

            // 座標を格納
            fscanf(fp, "%f %f %f", &v.pos.x, &v.pos.y, &v.pos.z);

            // 出来上がったデータをvectorに追加
            vertex.push_back(v);

        }
        // キーワードは'f'か
        if (!strcmp(key, "f"))
        {
            // インデックス値を格納
            int v1, v2, v3;
            fscanf(fp, "%d//%d//%d//", &v1, &v2, &v3);

            // インデックスから始まるので-1する
            index.push_back(v1 - 1);
            index.push_back(v2 - 1);
            index.push_back(v3 - 1);
        }
    }

    // ファイルを閉じる
    fclose(fp);

    float rotate = 0.0f;

    // メインループ
    while (ProcessMessage() != -1 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        // 回転行列の作成
        rotate++;
        MATRIX world = MGetRotX(rotate * DX_PI_F / 180.0f);
        world = MMult(world, MGetRotY(rotate * DX_PI_F / 180.0f));
        world = MMult(world, MGetRotZ(rotate * DX_PI_F / 180.0f));


        // 画面初期化
        ClearDrawScreen();

        // 作成したワールド行列を適用
        SetTransformToWorld(&world);

        // obj描画
        DrawPolygonIndexed3D(vertex.data(), vertex.size(), index.data(), index.size() / 3, DX_NONE_GRAPH, false);

        // 裏画面に描画した内容を表に表示する
        ScreenFlip();
    }

    // DXライブラリの破棄
    DxLib_End();

    return 0;
}
/*
obj�t�@�C���̃p�[�T�[(�p�[�T�[�Ƃ͓���̃t�@�C���f�[�^��ǂݍ��ރC���[�W)
*/

// Include
#include<vector>
#include "DxLib.h"

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpcmdLine, int nCmdShow)
{
    // �E�B���h�E���[�h�ɐݒ�
    ChangeWindowMode(true);
    // �w�i�F�̕ύX
    SetBackgroundColor(50, 50, 50);

    // DX���C�u�����̏�����
    if (DxLib_Init() == -1) return 0;

    // ����ʂɕ`�悷��
    SetDrawScreen(DX_SCREEN_BACK);

    // ���C�g����
    SetUseLighting(false);

    // Z�o�b�t�@�L��
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);

    // �J�������W�ƒ����_�̐ݒ�
    SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 1.0f, -2.0f), VGet(0.0f, 0.0f, 0.0f));

    // �J����NEAR��FAR�̐ݒ�
    SetCameraNearFar(1.0f, 100.0f);

    // �t�@�C�����J��
    FILE* fp = fopen("cube.obj", "r");

    // NULL�`�F�b�N
    if (!fp) { return 0; }

    // ���_�f�[�^�ƃC���f�b�N�X�l
    std::vector<VERTEX3D> vertex;
    std::vector<unsigned short> index;

    while (!feof(fp))
    {
        char key[MAX_PATH] = "";

        // �L�[���[�h�ǂݍ���
        fscanf(fp, "%s ", key);

        // �L�[���[�h��'v'��
        if (!strcmp(key, "v"))
        {
            // ���_�\���̂��쐬
            VERTEX3D v = { 0 };

            // �F�̓����_��
            v.dif = GetColorU8(GetRand(255), GetRand(255), GetRand(255), 255);

            // ���W���i�[
            fscanf(fp, "%f %f %f", &v.pos.x, &v.pos.y, &v.pos.z);

            // �o���オ�����f�[�^��vector�ɒǉ�
            vertex.push_back(v);

        }
        // �L�[���[�h��'f'��
        if (!strcmp(key, "f"))
        {
            // �C���f�b�N�X�l���i�[
            int v1, v2, v3;
            fscanf(fp, "%d//%d//%d//", &v1, &v2, &v3);

            // �C���f�b�N�X����n�܂�̂�-1����
            index.push_back(v1 - 1);
            index.push_back(v2 - 1);
            index.push_back(v3 - 1);
        }
    }

    // �t�@�C�������
    fclose(fp);

    float rotate = 0.0f;

    // ���C�����[�v
    while (ProcessMessage() != -1 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        // ��]�s��̍쐬
        rotate++;
        MATRIX world = MGetRotX(rotate * DX_PI_F / 180.0f);
        world = MMult(world, MGetRotY(rotate * DX_PI_F / 180.0f));
        world = MMult(world, MGetRotZ(rotate * DX_PI_F / 180.0f));


        // ��ʏ�����
        ClearDrawScreen();

        // �쐬�������[���h�s���K�p
        SetTransformToWorld(&world);

        // obj�`��
        DrawPolygonIndexed3D(vertex.data(), vertex.size(), index.data(), index.size() / 3, DX_NONE_GRAPH, false);

        // ����ʂɕ`�悵�����e��\�ɕ\������
        ScreenFlip();
    }

    // DX���C�u�����̔j��
    DxLib_End();

    return 0;
}
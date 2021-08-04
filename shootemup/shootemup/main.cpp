#include<DxLib.h>
#include<memory>
#include<cmath>
#include<vector>
//#include"Geometry.h"
#include "HomingShot.h"

///�����蔻��֐�
///@param posA A�̍��W
///@param radiusA A�̔��a
///@param posB B�̍��W
///@param radiusB B�̔��a
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//�����蔻����������Ă�������
	return (posA - posB).Magnitude() <= radiusA + radiusB;
}

using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("2016019_�c����");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//�w�i�p
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH=LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	auto arrowH = LoadGraph("img/arrow.png");


	//�e�̔��a
	float bulletRadius = 5.0f;

	//���@�̔��a
	float playerRadius = 10.0f;

	//�K����256���炢����Ƃ�
	 //�G�e
	Bullet bullets[256];

	HomingShot homigshot[16] = {};
	for (auto & shot:homigshot)
	{
		shot.trail.SetHandle(arrowH);
	}

	Position2 enemypos(320,25);//�G���W
	Position2 playerpos(320, 400);//���@���W

	// ���@�e
	Bullet shots[8] = {};

	unsigned int frame = 0;//�t���[���Ǘ��p

	char keystate[256] = {};
	char lastkeystate[256] = {};
	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;

	constexpr float player_shot_Speed = 5.0f;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//�w�i
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//�v���C���[
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}

		if (keystate[KEY_INPUT_Z] && !lastkeystate[KEY_INPUT_Z])
		{
			for (auto& shot : homigshot)
			{
				if (!shot.isActive)
				{
					shot.isActive = true;
					bool isRight = rand() % 2;
					shot.pos = playerpos;
					//shot.vel = isRight ? Vector2(player_shot_Speed, 0.0f ) : Vector2( -player_shot_Speed, 0.0f );
					shot.vel = isRight ? Vector2(player_shot_Speed, player_shot_Speed) : Vector2(-player_shot_Speed, player_shot_Speed);
					shot.trail.Clear();
					break;
				}
			}
		}

		// �e�X�V�E�`��

		for (auto& shot : homigshot)
		{
			if (shot.isActive)
			{
				if (frame % 2 == 0)
				{
					shot.trail.Update();
				}
				shot.pos += shot.vel;

				shot.trail.Draw();

				// �G�_���e(���񂽂�Ȃ��)
				shot.vel = shot.vel + (enemypos - shot.pos).Normalized();
				shot.vel = shot.vel.Normalized() * player_shot_Speed; 

				//auto nVel = shot.vel.Normalized();
				//auto nTarget = (enemypos - shot.pos).Normalized();
				//auto dot = Dot(nVel, nTarget);		// dot=cos��
				//auto cross = Cross(nVel, nTarget);
				//auto angle = acos(dot);				// cos����0�𒆐S�ɂ����Ώ�
				//angle = std::fminf(angle, DX_PI_F / 24.0f);
				//angle = cross >= 0.0f ? angle : -angle;
				//angle = atan2(nVel.y, nVel.x) + angle;

				//shot.vel = Vector2(cosf(angle), sinf(angle)) * player_shot_Speed;


				// Dot��Crross���g���ق�
				//constexpr bool isSimple = false;
				//if (isSimple)
				//{
				//	shot.vel = shot.vel + (enemypos - shot.pos).Normalized();
				//	shot.vel = shot.vel.Normalized() * player_shot_Speed;
				//}
				//else
				//{
				//	auto nShotVel = shot.vel.Normalized();
				//	auto nToEnemyVec = (enemypos - shot.pos).Normalized();
				//	float dot = Dot(nShotVel, nToEnemyVec);
				//	float c = acos(dot);
				//	// �}�C�i�X�͉E���A�v���X���ƍ����
				//	float cross = Cross(nShotVel, nToEnemyVec);

				//}

				shot.pos += shot.vel;
				//DrawCircleAA(shot.pos.x, shot.pos.y, 10.0f, 16, 0xff0000);

				//��ʊO�s������
				if (shot.pos.x < -10 || 650 < shot.pos.x ||
					shot.pos.y < -10 || 490 < shot.pos.y)
				{
					//�ʏ�����
					shot.isActive = false;
				}
				// �G�ɓ������Ă�������
				if (IsHit(shot.pos, 5.0f, enemypos, 30.0f))
				{
					shot.isActive = false;
				}
			}
			
		}
		DrawCircleAA(enemypos.x, enemypos.y, 30.0f, 16, 0x0000ff, false, 3.0f);


		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//���@�̖{��(�����蔻��)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		//�e����
		if (frame % 12 == 0) {
			for (auto& b : homigshot) {
				if (!b.isActive) {
					//b.pos = enemypos;
					//b.vel = Vector2(0, 5);
					//b.isActive = true;
					break;
				}
			}
		}

		//�e�̍X�V����ѕ\��
		for (auto& b : homigshot) {
			if (!b.isActive) {
				continue;
			}

			//�e�̌��ݍ��W�ɒe�̌��ݑ��x�����Z���Ă�������
			
			float angle = 0.0f;
			//�e�̊p�x��atan2�Ōv�Z���Ă��������Bangle�ɒl������񂾂�I�D
			
			//DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			//if (isDebugMode) {
			//	//�e�̖{��(�����蔻��)
			//	DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			//}
			//�e���E��
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//������I
			//����IsHit�͎����������Ă܂���B�����ŏ����Ă��������B
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//�������������������Ă��������B
			}
		}

		//�G�̕\��
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		//if (isDebugMode) {
		//	//�G�̖{��(�����蔻��)
		//	DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		//}
		++frame;
		ScreenFlip();
		std::copy(std::begin(keystate), std::end(keystate), std::begin(lastkeystate));
		//copy(begin(keystate),end(keystate),begin(lastkeystate));	// begin�����Aend���K
	}

	DxLib_End();

	return 0;
}
/*
YRoads
2022.9.10
*/

/*
1.��ȡ����λ��
2.��ȡ������λ�� EOF
3.��ǰΪ�յء����ǣ���1
4.��ǰ1Ϊ�����ӡ�������
	4.1.��ǰ2Ϊ�յأ���1 ������2
	4.2.��ǰ2Ϊǽ������Ϊ
	4.3.��ǰ2Ϊ���ǣ���1 ������2
3.ʱ�̼������λ���Ƿ�Ϊ0��0���ӡ����
*/

/*
1.ѭ��123��ȡ�ļ��ַ���ֱ��"level"
2.ָ�����һ��
3.�ж��Ƿ�Ϊ����i����������ѭ��
4.��ȡ����
5.ѭ��������ģ�������ά����
*/

/*
1.��ȡ"level"�ַ�
2.�ҵ����������
3.��������µķ��ż��뵽������
4.����"end"ֹͣ
*/

/*
9.10 ��Ϸ�߼�ʵ��
9.11 ������֡�ͼƬ���޸���Ϸ�㷨����
9.12 ����ؿ����ܣ���д�ж���Ϸ��ʤ�㷨�������Զ���ؿ�����
ѡ��Ŀǰ�ò��ˣ�ģ�ͻ��ص�
*/



#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#pragma comment(lib,"Winmm.lib")

//�ؿ�ģ��
int Sence[50][50] = { 0 };
int SenceCopy[50][50] = { 0 };
int Ma_x;	//����x����
int Ma_y;	//����y����
int Box_num = 0;	//�������������
int Box_x[30], Box_y[30];
int lev, length, width;	//�ؿ����С���
int yxc = 0;	//ѡ���ж�����
int zxw = 0;	//�ؿ��ж�����

//��ȡ�û�����ؿ�
int Le_Num() {
	initgraph(20, 20);
	wchar_t le_num[5];
	bool tr = InputBox(le_num, 4, L"Please input the level: (Ŀǰ����10��)", L"ֻ��", 0, 0, 0, false);
	if (tr == 0 || _wtoi(le_num) < 1 || _wtoi(le_num) > 999) {
		printf("Please enter a number!");
		closegraph();
		return  0;
	}
	closegraph();
	return  _wtoi(le_num);
}

//��sence�ļ�
int OpenSence(FILE* psence) {
	char ch[50] = { 0 };
	int le_num = Le_Num();
	while (true) {
		while (fgetc(psence) == 32);	//�����ո�
		fseek(psence, -1, SEEK_CUR);
		fscanf(psence, "%s", ch);
		if (strcmp(ch, "end") == 0) {
			printf("��ȡʧ��");
			return 0;
		}

		if (strcmp(ch, "level") != 0) {
			continue;
		}
		else {
			while (fgetc(psence) == 32);	//�����ո�
			fseek(psence, -1, SEEK_CUR);
			fscanf(psence, "%s", ch);	//�ؿ���
			lev = atoi(ch);

			//�����û�ָ���ؿ�
			if (lev == le_num) {
				fscanf(psence, "%s", ch);	//��
				length = atoi(ch);
				while (fgetc(psence) == 32);
				fseek(psence, -1, SEEK_CUR);
				fscanf(psence, "%s", ch);	//��
				width = atoi(ch);
				while (fgetc(psence) == 32);
				fseek(psence, -1, SEEK_CUR);

				//��ȡ����
				for (int i = 0; i < width; i++) {
					fgetc(psence);
					for (int j = 0; j < length; j++) {
						fscanf(psence, "%c", ch);
						Sence[i][j] = atoi(ch);
					}
				}
				//��Ӹ�����ʵ��q�ؿ���
				for (int i = 0; i < width; i++) {
					for (int j = 0; j < length; j++) {
						SenceCopy[i][j] = Sence[i][j];
					}
				}	//���ɳ���
				break;
			}
			else {
				continue;
			}
		}
	}
	return 1;
}

//�ؿ�����������ԭ����
void qAgain() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			Sence[i][j] = SenceCopy[i][j];
		}
	}
}

//��ȡ��������������
void BoxS() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			if (Sence[i][j] == 5) {
				Box_x[Box_num] = j;
				Box_y[Box_num] = i;
				Box_num++;
			}
		}
	}
}

//��ȡ������������
void ManS() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			if (Sence[i][j] == 2) {
				Ma_x = j;
				Ma_y = i;
			}
		}
	}
}

//�������Ƹ���
void ConT(int x, int y, int U, int D, int L, int R) {
	switch (Sence[y - U + D][x - L + R]) {
	case 0:
	case 5:
		Sence[y][x] = 0;
		Sence[y - U + D][x - L + R] = 2;
		break;
	case 3:
	case 4:
		switch (Sence[y - 2 * U + 2 * D][x - 2 * L + 2 * R]) {
		case 0:
			Sence[y][x] = 0;
			Sence[y - U + D][x - L + R] = 2;
			Sence[y - 2 * U + 2 * D][x - 2 * L + 2 * R] = 3;
			break;
		case 5:
			Sence[y][x] = 0;
			Sence[y - U + D][x - L + R] = 2;
			Sence[y - 2 * U + 2 * D][x - 2 * L + 2 * R] = 4;
			break;
		}
	}
}

//������������
void ConTCenter() {
	int U = 0, D = 0, L = 0, R = 0;
	char key = _getch();
	switch (key) {
	case 'W':
	case 'w':
		U = 1;
		ConT(Ma_x, Ma_y, U, D, L, R);
		break;
	case 'S':
	case 's':
		D = 1;
		ConT(Ma_x, Ma_y, U, D, L, R);
		break;
	case 'A':
	case 'a':
		L = 1;
		ConT(Ma_x, Ma_y, U, D, L, R);
		break;
	case 'D':
	case 'd':
		R = 1;
		ConT(Ma_x, Ma_y, U, D, L, R);
		break;
		//case 'r':	//ѡ��ؿ�
		//	yxc = 520;
		//	break;
	case 'Q':
	case 'q':	//�ؿ�
		zxw = 1314;
		break;
	}
}

//���°�����״̬
void UpBox() {
	for (int i = 0; i < Box_num; i++) {
		if (Sence[Box_y[i]][Box_x[i]] == 0) {
			Sence[Box_y[i]][Box_x[i]] = 5;
		}
	}
}

//��Ϸʤ������֮һ(���ϰ�����Ϊ0)
int GameO(int width, int lenth) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < lenth; j++) {
			if (Sence[i][j] == 3) { return 1; }
		}
	}
	return 0;
}

int main() {
	//�������������
	/*mciSendString(_T("open soul.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);*/

	while (1) {
		FILE* psence = fopen("sence.txt", "r");
		int Over = OpenSence(psence);	//��sence�ļ�(�˴�����length��width)
		if (Over == 0) { return 0; }

		BoxS();		//��ȡ��������������

		//����ͼƬ
		IMAGE GROUND, WALL, MAN, BOX, BLACKBOX, STAR, WIN;
		loadimage(&GROUND, _T("0.jpg"), 50, 50);
		loadimage(&WALL, _T("1.jpg"), 50, 50);
		loadimage(&MAN, _T("2.jpg"), 50, 50);
		loadimage(&BOX, _T("3.jpg"), 50, 50);
		loadimage(&BLACKBOX, _T("4.jpg"), 50, 50);
		loadimage(&STAR, _T("5.jpg"), 50, 50);
		loadimage(&WIN, _T("win.jpg"), length * 50, width * 50);		//�˴�length��width��ҪOpenSence()������ȡ

		//��ʼ��Ϸ
		initgraph(length * 50, width * 50);
		while (true) {
			UpBox();	//���°�����״̬
			//���س���
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < length; j++) {
					switch (Sence[i][j]) {
					case 0:
						putimage(j * 50, i * 50, &GROUND); break;
					case 1:
						putimage(j * 50, i * 50, &WALL); break;
					case 2:
						putimage(j * 50, i * 50, &MAN); break;
					case 3:
						putimage(j * 50, i * 50, &BOX); break;
					case 4:
						putimage(j * 50, i * 50, &BLACKBOX); break;
					case 5:
						putimage(j * 50, i * 50, &STAR); break;
					}
				}
				printf("\n");
			}

			ManS();		//��ȡ������������

			//�����Ϸ״̬
			for (int i = 0; i < Box_num; i++) {
				if (GameO(width, length) == false) {
					goto GameOver;
				}
			}

			ConTCenter();	//��������
			if (yxc == 520) {
				yxc = 0;
				break;
			}		//����ѡ��1
			if (zxw == 1314) {
				zxw = 0;
				qAgain();
				initgraph(length * 50, width * 50);		//ˢ��ҳ��(�������ͼƬ����)
				continue;
			}		//�ؿ�

			cleardevice();
		}
		fclose(psence);
		continue;	//����ѡ��2

	GameOver:	//��ת
		//����ͨ��ͼƬ
		putimage(0, 0, &WIN);
		_getch();
	}

	return 0;
}
/*
YRoads
2022.9.10
*/

/*
1.获取人物位置
2.获取白箱子位置 EOF
3.人前为空地、星星：人1
4.人前1为白箱子、黑箱子
	4.1.人前2为空地：人1 白箱子2
	4.2.人前2为墙：无行为
	4.3.人前2为星星：人1 黑箱子2
3.时刻检测星星位置是否为0，0则打印星星
*/

/*
1.循环123读取文件字符串直到"level"
2.指针后移一格
3.判断是否为数字i，是则跳出循环
4.读取换行
5.循环将行列模型输入二维数组
*/

/*
1.获取"level"字符
2.找到后面的数字
3.添加数字下的符号加入到数组中
4.遇到"end"停止
*/

/*
9.10 游戏逻辑实现
9.11 添加音乐、图片，修复游戏算法错误
9.12 添加重开功能，重写判定游戏获胜算法，增添自定义关卡功能
选关目前用不了，模型会重叠
*/



#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#pragma comment(lib,"Winmm.lib")

//关卡模型
int Sence[50][50] = { 0 };
int SenceCopy[50][50] = { 0 };
int Ma_x;	//人物x坐标
int Ma_y;	//人物y坐标
int Box_num = 0;	//储存白箱子数量
int Box_x[30], Box_y[30];
int lev, length, width;	//关卡、行、列
int yxc = 0;	//选关判定数据
int zxw = 0;	//重开判定数据

//获取用户输入关卡
int Le_Num() {
	initgraph(20, 20);
	wchar_t le_num[5];
	bool tr = InputBox(le_num, 4, L"Please input the level: (目前仅有10关)", L"只因", 0, 0, 0, false);
	if (tr == 0 || _wtoi(le_num) < 1 || _wtoi(le_num) > 999) {
		printf("Please enter a number!");
		closegraph();
		return  0;
	}
	closegraph();
	return  _wtoi(le_num);
}

//打开sence文件
int OpenSence(FILE* psence) {
	char ch[50] = { 0 };
	int le_num = Le_Num();
	while (true) {
		while (fgetc(psence) == 32);	//跳过空格
		fseek(psence, -1, SEEK_CUR);
		fscanf(psence, "%s", ch);
		if (strcmp(ch, "end") == 0) {
			printf("读取失败");
			return 0;
		}

		if (strcmp(ch, "level") != 0) {
			continue;
		}
		else {
			while (fgetc(psence) == 32);	//跳过空格
			fseek(psence, -1, SEEK_CUR);
			fscanf(psence, "%s", ch);	//关卡数
			lev = atoi(ch);

			//载入用户指定关卡
			if (lev == le_num) {
				fscanf(psence, "%s", ch);	//行
				length = atoi(ch);
				while (fgetc(psence) == 32);
				fseek(psence, -1, SEEK_CUR);
				fscanf(psence, "%s", ch);	//列
				width = atoi(ch);
				while (fgetc(psence) == 32);
				fseek(psence, -1, SEEK_CUR);

				//读取场景
				for (int i = 0; i < width; i++) {
					fgetc(psence);
					for (int j = 0; j < length; j++) {
						fscanf(psence, "%c", ch);
						Sence[i][j] = atoi(ch);
					}
				}
				//添加副本（实现q重开）
				for (int i = 0; i < width; i++) {
					for (int j = 0; j < length; j++) {
						SenceCopy[i][j] = Sence[i][j];
					}
				}	//生成场景
				break;
			}
			else {
				continue;
			}
		}
	}
	return 1;
}

//重开（副本覆盖原本）
void qAgain() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			Sence[i][j] = SenceCopy[i][j];
		}
	}
}

//获取白箱子所在坐标
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

//获取人物所在坐标
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

//操作控制附属
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

//操作控制中心
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
		//case 'r':	//选择关卡
		//	yxc = 520;
		//	break;
	case 'Q':
	case 'q':	//重开
		zxw = 1314;
		break;
	}
}

//更新白箱子状态
void UpBox() {
	for (int i = 0; i < Box_num; i++) {
		if (Sence[Box_y[i]][Box_x[i]] == 0) {
			Sence[Box_y[i]][Box_x[i]] = 5;
		}
	}
}

//游戏胜利条件之一(场上白箱子为0)
int GameO(int width, int lenth) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < lenth; j++) {
			if (Sence[i][j] == 3) { return 1; }
		}
	}
	return 0;
}

int main() {
	//载入美妙的音乐
	/*mciSendString(_T("open soul.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);*/

	while (1) {
		FILE* psence = fopen("sence.txt", "r");
		int Over = OpenSence(psence);	//打开sence文件(此处生成length和width)
		if (Over == 0) { return 0; }

		BoxS();		//获取白箱子所在坐标

		//载入图片
		IMAGE GROUND, WALL, MAN, BOX, BLACKBOX, STAR, WIN;
		loadimage(&GROUND, _T("0.jpg"), 50, 50);
		loadimage(&WALL, _T("1.jpg"), 50, 50);
		loadimage(&MAN, _T("2.jpg"), 50, 50);
		loadimage(&BOX, _T("3.jpg"), 50, 50);
		loadimage(&BLACKBOX, _T("4.jpg"), 50, 50);
		loadimage(&STAR, _T("5.jpg"), 50, 50);
		loadimage(&WIN, _T("win.jpg"), length * 50, width * 50);		//此处length和width需要OpenSence()函数获取

		//开始游戏
		initgraph(length * 50, width * 50);
		while (true) {
			UpBox();	//更新白箱子状态
			//加载场景
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

			ManS();		//获取人物所在坐标

			//监测游戏状态
			for (int i = 0; i < Box_num; i++) {
				if (GameO(width, length) == false) {
					goto GameOver;
				}
			}

			ConTCenter();	//控制中心
			if (yxc == 520) {
				yxc = 0;
				break;
			}		//重新选关1
			if (zxw == 1314) {
				zxw = 0;
				qAgain();
				initgraph(length * 50, width * 50);		//刷新页面(否则会有图片残留)
				continue;
			}		//重开

			cleardevice();
		}
		fclose(psence);
		continue;	//重新选关2

	GameOver:	//跳转
		//弹出通关图片
		putimage(0, 0, &WIN);
		_getch();
	}

	return 0;
}
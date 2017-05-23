#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <time.h>
#include "chess.h"
#include "Pos.h"
#define mem(a,b) memset(a,b,sizeof(a))
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
#define window_x 1013
#define window_y 800
#define inf 0x3f3f3f3f
#define chess_r 49
#define debug() puts("what the fuck!!!")
int MODE = 1;//ģʽ��1����˫�ˣ�2�����ˣ�3��������
Pos map[20][20];//�洢ÿһ�����ӵ�����
int vis[20][20];//���һ��λ�õ�ǰ��û������
int color[20][20];//1������壬0�������
IMAGE MAPIMAGE[30][30];//ͼƬ����
IMAGE NowChessImage;//�洢��˭���ӵ�ͼ
void monse()
{
	POINT point;
	char s[10];
	while (true)
	{
		GetCursorPos(&point);
		// ����������
		sprintf(s, _T("%d    "), point.x);
		outtextxy(0, 0, s);

		sprintf(s, _T("%d    "), point.y);
		outtextxy(0, 20, s);

		// �ʵ���ʱ
		Sleep(10);

	}
}
void chess::welcome()
{
	mciSendString("open welcome.mp3", NULL, 0, 0);
	mciSendString("play welcome.mp3 repeat", NULL, 0, 0);


	initgraph(1024, 632);//����һ��1024*632�Ļ���
	loadimage(NULL, "welcome.jpg");//���ñ���
	 /*mciSendString("open welcome.mp3", NULL, 0, 0);
	 mciSendString("play welcome.mp3", NULL, 0, 0);*/
	 //monse();
	setbkmode(TRANSPARENT);//͸������
	HWND hwnd = GetHWnd();//��ȡ��ǰ���ھ��
	SetWindowText(hwnd, "�X�m�������� --- By:������");//���ô��ڱ���
	settextcolor(RGB(77, 77, 77));
	LOGFONT f;//���������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 70;
	strcpy_s(f.lfFaceName, "΢���ź�");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	RECT r1 = { 500, 300, 800, 400 };
	RECT r2 = { 500, 405, 800, 505 };
	RECT r3 = { 500, 510, 800, 610 };
	/*setlinecolor(BLACK);
	rectangle(500, 300, 800, 400);*/
	drawtext("��˫��ģʽ��", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��
	drawtext("������A   I��", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��
	drawtext("��������ս��", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��
																	//--------------------------------------------------------------------------------------------
	bool flag = true;
	MOUSEMSG m;//����һ��������
	while (flag)
	{
		//����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
		BeginBatchDraw();
		m = GetMouseMsg();//��ȡ�����Ϣ
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN://����������
			EndBatchDraw();
			if (m.x >= 500 && m.x <= 800 && m.y >= 300 && m.y <= 400)//�ж��ڿ�ʼ��Ϸ�ľ���������
			{

				flag = false;
				MODE = 1;
				closegraph();
			}
			else if (m.x >= 500 && m.x <= 800 && m.y >= 405 && m.y <= 505)//����aiģʽ
			{

				flag = false;
				MODE = 2;
				closegraph();

			}
		case WM_MOUSEMOVE://�������ƶ�
			EndBatchDraw();
			if (m.x >= 500 && m.x <= 800 && m.y >= 300 && m.y <= 400)//�ж�����ڿ�ʼ��Ϸ�ľ���������
			{
				settextcolor(RGB(254, 67, 101));
				drawtext("��˫��ģʽ��", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 500 && m.x <= 800 && m.y >= 405 && m.y <= 505)
			{
				settextcolor(RGB(254, 67, 101));
				drawtext("������A   I��", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x >= 500 && m.x <= 800 && m.y >= 510 && m.y <= 610)
			{
				settextcolor(RGB(254, 67, 101));
				drawtext("��������ս��", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				settextcolor(RGB(77, 77, 77));
				drawtext("��˫��ģʽ��", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				drawtext("������A   I��", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				drawtext("��������ս��", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			//break;
		}
	}

}
double chess::dis(int x1, int y1, int x2, int y2)//��������ľ���
{
	return sqrt((double)((x1 - x2)*(x1 - x2)) + (double)((y1 - y2)*(y1 - y2)));
}
bool chess::JudgeChess(int x, int y)
{
	//˼·���Ե�ǰ��Ϊ���ĵ㣬���ĸ�����չ������
	if (color[x][y] == 0)//�жϰ�������
	{
		int num = 0;//��¼��������
  //����x����
		for (int i = y; i <= 15; i++)//����
		{
			if (color[x][i] == 0)
			{
				num++;
			}
			else
				break;
		}
		for (int i = y - 1; i >= 0; i--)//����
		{
			if (color[x][i] == 0)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;//�������飬���ؽ��
		num = 0;//����
				//����y����
		for (int i = x; i <= 15; i++)//����
		{
			if (color[i][y] == 0)
			{
				num++;
			}
			else
				break;
		}
		for (int i = x - 1; i >= 0; i--)//����
		{
			if (color[i][y] == 0)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;//�������飬���ؽ��
		num = 0;//����
				//������б������'\'
		for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j--)//�ɵ�ǰ��������
		{
			if (color[i][j] == 0)
			{
				num++;
			}
			else
				break;
		}
		for (int i = x + 1, j = y + 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j++)//�ɵ�ǰ��������
		{
			if (color[i][j] == 0)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;// '\'���飬���ؽ��
		num = 0;//����
				//������б������'/'
		for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j++)//�ɵ�ǰ��б������
		{
			if (color[i][j] == 0)
			{
				num++;
			}
			else
				break;
		}
		for (int i = x + 1, j = y - 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j--)//�ɵ�ǰ��б������
		{
			if (color[i][j] == 0)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;// '/'���飬���ؽ��
		num = 0;//����
		return false;//���岻������
	}
	else if (color[x][y] == 1)//��������
	{
		int num = 0;//��¼��������
					//����x����
		for (int i = y; i <= 15; i++)//����
		{
			if (color[x][i] == 1)
			{
				num++;
			}
			else
				break;
		}
		for (int i = y - 1; i >= 0; i--)//����
		{
			if (color[x][i] == 1)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;//�������飬���ؽ��
		num = 0;//����
				//����y����
		for (int i = x; i <= 15; i++)//����
		{
			if (color[i][y] == 1)
			{
				num++;
			}
			else
				break;
		}
		for (int i = x - 1; i >= 0; i--)//����
		{
			if (color[i][y] == 1)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;//�������飬���ؽ��
		num = 0;//����
				//������б������'\'
		for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j--)//�ɵ�ǰ��������
		{
			if (color[i][j] == 1)
			{
				num++;
			}
			else
				break;
		}
		for (int i = x + 1, j = y + 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j++)//�ɵ�ǰ��������
		{
			if (color[i][j] == 1)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;// '\'���飬���ؽ��
		num = 0;//����
				//������б������'/'
		for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j++)//�ɵ�ǰ��б������
		{
			if (color[i][j] == 1)
			{
				num++;
			}
			else
				break;
		}
		for (int i = x + 1, j = y - 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j--)//�ɵ�ǰ��б������
		{
			if (color[i][j] == 1)
			{
				num++;
			}
			else
				break;
		}
		if (num >= 5)
			return true;// '/'���飬���ؽ��
		num = 0;//����
		return false;//���岻������
	}
}
void chess::PlayGame1()
{
	/*---------------------------------����������------------------------------------------------*/
	mciSendString("close welcome.mp3", NULL, 0, 0);
	mciSendString("open palying.mp3", NULL, 0, 0);
	mciSendString("play palying.mp3 repeat", NULL, 0, 0);
	/*---------------------------------������------------------------------------------------*/
	initgraph(window_x, window_y);
	loadimage(NULL, "4.jpg");
	HWND hwnd = GetHWnd();//��ȡ��ǰ���ھ��
	SetWindowText(hwnd, "�X�m�������� --- By:������");//���ô��ڱ���
	setlinestyle(PS_SOLID, 2);
	setlinecolor(BLACK);
	for (int x = 30; x <= 765; x += 49)
	{
		line(30, x, 765, x);
	}
	for (int y = 30; y <= 765; y += 49)
	{
		line(y, 30, y, 765);
	}
	/*---------------------------------�洢��������------------------------------------------------*/

	//�洢����
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 15; j++)
		{
			map[i][j].x = 30 + 49 * j;
		}
	}
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 15; j++)
		{
			map[j][i].y = 30 + 49 * j;
		}
	}

	//���ڵ�
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(map[3][3].x - 3, map[3][3].y - 3, map[3][3].x + 3, map[3][3].y + 3);
	fillrectangle(map[3][11].x - 3, map[3][11].y - 3, map[3][11].x + 3, map[3][11].y + 3);
	fillrectangle(map[7][7].x - 3, map[7][7].y - 3, map[7][7].x + 3, map[7][7].y + 3);
	fillrectangle(map[11][3].x - 3, map[11][3].y - 3, map[11][3].x + 3, map[11][3].y + 3);
	fillrectangle(map[11][11].x - 3, map[11][11].y - 3, map[11][11].x + 3, map[11][11].y + 3);
	//�洢ÿһ��λ�õ�Բ��ͼƬ
	/*for (int i = 0; i <= 15; i++)
	{
	for (int j = 0; j <= 15; j++)
	{
	getimage(&MAPIMAGE[i][j],)
	}
	}*/

	/*---------------------------------��Ϸ��ʼ------------------------------------------------*/
	mem(vis, 0);//����������
	mem(color, -1);//����ɫȫ��������
	bool flag = true;
	int opp = 0;//ż���Ǻ��壬�����ǰ���
	MOUSEMSG m;//����һ��������


			   /*setlinestyle(PS_SOLID, 1);
			   setlinecolor(BLACK);*/
	getimage(&NowChessImage, 780, 40, 1000, 100);//��ȡԭ����ͼ�񲢴洢
												 //rectangle(780,40, 1000, 100);
	settextcolor(RGB(77, 77, 77));
	RECT r1 = { 780,40, 1000, 100 };
	LOGFONT f;//���������
	setbkmode(TRANSPARENT);//͸������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 50;
	strcpy_s(f.lfFaceName, "΢���ź�");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	drawtext("��ڷ�����", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	while (flag)
	{
		//����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
		BeginBatchDraw();
		m = GetMouseMsg();//��ȡ�����Ϣ
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN://����������
			EndBatchDraw();
			Pos temp;
			temp.x = inf, temp.y = inf;
			double R = 10000000.0;
			for (int i = 0; i <= 15; i++)
			{
				for (int j = 0; j <= 15; j++)
				{
					if (dis(map[i][j].x, map[i][j].y, m.x, m.y)<R)
					{
						R = dis(map[i][j].x, map[i][j].y, m.x, m.y);
						temp.x = i;
						temp.y = j;
					}
				}
			}
			setlinecolor(BLACK);
			if (!vis[temp.x][temp.y])//ֻ��û�б���ǹ��ſ�����
			{
				vis[temp.x][temp.y] = 1;
				if (opp & 1)//�������ͻ�����
				{

					setfillcolor(WHITE);
					fillcircle(map[temp.x][temp.y].x, map[temp.x][temp.y].y, chess_r / 3);
					PlaySound(TEXT("chessmusic.wav"), 0, SND_FILENAME);//��������
					putimage(780, 40, &NowChessImage);
					drawtext("��ڷ�����", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					color[temp.x][temp.y] = 0;
					if (JudgeChess(temp.x, temp.y) == true)//�����������
					{
						flag = false;
						GameOver1(0);
					}

				}
				else//ż���ͻ�����
				{

					setfillcolor(BLACK);
					fillcircle(map[temp.x][temp.y].x, map[temp.x][temp.y].y, chess_r / 3);
					PlaySound(TEXT("chessmusic.wav"), 0, SND_FILENAME);

					putimage(780, 40, &NowChessImage);
					drawtext("��׷�����", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					color[temp.x][temp.y] = 1;//�����ɫ
					if (JudgeChess(temp.x, temp.y) == true)//�����������
					{
						flag = false;
						GameOver1(1);
					}
				}
				opp++;//�ı�״̬
			}
			//case WM_MOUSEMOVE://�������ƶ�
			//	EndBatchDraw();
		}
	}
}
void chess::GameOver1(int n)
{
	settextcolor(RGB(254, 67, 101));
	LOGFONT f;//���������
	setbkmode(TRANSPARENT);//͸������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 120;
	strcpy_s(f.lfFaceName, "����");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	RECT r1 = { 0, 0, 800, 800 };
	RECT r2 = { 790, 710, 980, 760 };
	RECT r3 = { 780,40, 1000, 100 };
	setlinecolor(BLACK);
	if (n == 0)
	{
		drawtext("�� �� �� ʤ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��

	}
	else if (n == 1)
	{
		drawtext("�� �� �� ʤ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��
	}
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 50;
	strcpy_s(f.lfFaceName, "΢���ź�");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	putimage(780, 40, &NowChessImage);
	drawtext("��Ϸ����", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//���þ���
	setlinestyle(PS_SOLID, 1);
	setlinecolor(BLACK);
	rectangle(790, 710, 980, 760);//�����¿�ʼ�ľ��α߿�
	setbkmode(TRANSPARENT);//͸������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 50;
	strcpy_s(f.lfFaceName, "΢���ź�");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	settextcolor(RGB(77, 77, 77));
	drawtext("���¿�ʼ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	bool flag = true;
	MOUSEMSG m;//����һ��������
	while (flag)
	{
		//����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
		BeginBatchDraw();
		m = GetMouseMsg();//��ȡ�����Ϣ
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN://����������
			EndBatchDraw();
			if (m.x >= 790 && m.x <= 980 && m.y >= 710 && m.y <= 760)//�ж��ڿ�ʼ��Ϸ�ľ���������
			{
				flag = false;
				//***********************************************
				PlayGame1();//���¿�ʼ��Ϸ
			}
			//break;
		case WM_MOUSEMOVE://�������ƶ�
			EndBatchDraw();
			if (m.x >= 790 && m.x <= 980 && m.y >= 710 && m.y <= 760)//�ж�����ڿ�ʼ��Ϸ�ľ���������
			{
				settextcolor(RGB(254, 67, 101));
				drawtext("���¿�ʼ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				settextcolor(RGB(77, 77, 77));
				drawtext("���¿�ʼ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			//break;
		}
	}


}
void chess::PlayGame2()
{
	/*---------------------------------����������------------------------------------------------*/
	mciSendString("close welcome.mp3", NULL, 0, 0);
	mciSendString("open palying.mp3", NULL, 0, 0);
	mciSendString("play palying.mp3 repeat", NULL, 0, 0);
	/*---------------------------------������------------------------------------------------*/
	initgraph(window_x, window_y);
	loadimage(NULL, "4.jpg");
	HWND hwnd = GetHWnd();//��ȡ��ǰ���ھ��
	SetWindowText(hwnd, "�X�m�������� --- By:������");//���ô��ڱ���
	setlinestyle(PS_SOLID, 2);
	setlinecolor(BLACK);
	for (int x = 30; x <= 765; x += 49)
	{
		line(30, x, 765, x);
	}
	for (int y = 30; y <= 765; y += 49)
	{
		line(y, 30, y, 765);
	}
	/*---------------------------------�洢��������------------------------------------------------*/

	//�洢����
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 15; j++)
		{
			map[i][j].x = 30 + 49 * j;
		}
	}
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 15; j++)
		{
			map[j][i].y = 30 + 49 * j;
		}
	}

	//���ڵ�
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(map[3][3].x - 3, map[3][3].y - 3, map[3][3].x + 3, map[3][3].y + 3);
	fillrectangle(map[3][11].x - 3, map[3][11].y - 3, map[3][11].x + 3, map[3][11].y + 3);
	fillrectangle(map[7][7].x - 3, map[7][7].y - 3, map[7][7].x + 3, map[7][7].y + 3);
	fillrectangle(map[11][3].x - 3, map[11][3].y - 3, map[11][3].x + 3, map[11][3].y + 3);
	fillrectangle(map[11][11].x - 3, map[11][11].y - 3, map[11][11].x + 3, map[11][11].y + 3);
	//�洢ÿһ��λ�õ�Բ��ͼƬ

	/*---------------------------------��Ϸ��ʼ------------------------------------------------*/
	mem(vis, 0);//����������
	mem(color, -1);//����ɫȫ��������
	bool flag = true;
	int opp = 0;//ż���Ǻ��壬�����ǰ���
	MOUSEMSG m;//����һ��������

	getimage(&NowChessImage, 780, 40, 1000, 100);//��ȡԭ����ͼ�񲢴洢

	settextcolor(RGB(77, 77, 77));
	RECT r1 = { 780,40, 1000, 100 };
	LOGFONT f;//���������
	setbkmode(TRANSPARENT);//͸������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 50;
	strcpy_s(f.lfFaceName, "΢���ź�");
	settextstyle(&f);
	drawtext("��ڷ�����", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	while (flag)
	{
		//����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
		BeginBatchDraw();
		m = GetMouseMsg();//��ȡ�����Ϣ
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN://����������
			EndBatchDraw();
			Pos temp;
			temp.x = inf, temp.y = inf;
			double R = 10000000.0;
			for (int i = 0; i <= 15; i++)
			{
				for (int j = 0; j <= 15; j++)
				{
					if (dis(map[i][j].x, map[i][j].y, m.x, m.y)<R)
					{
						R = dis(map[i][j].x, map[i][j].y, m.x, m.y);
						temp.x = i;
						temp.y = j;
					}
				}
			}
			setlinecolor(BLACK);
			int black_go = 0;
			if (!vis[temp.x][temp.y])//ֻ��û�б���ǹ��ſ�����
			{
				vis[temp.x][temp.y] = 1;
				black_go = 1;

				//////////////////////������//////////////////////////
				setfillcolor(BLACK);
				fillcircle(map[temp.x][temp.y].x, map[temp.x][temp.y].y, chess_r / 3);
				PlaySound(TEXT("chessmusic.wav"), 0, SND_FILENAME);

				putimage(780, 40, &NowChessImage);
				drawtext("��׷�����", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				color[temp.x][temp.y] = 1;//�����ɫ
				if (JudgeChess(temp.x, temp.y) == true)//�����������
				{
					flag = false;
					GameOver2(1);
					break;
				}
			}
				///////////////////////////////������//////////////////////////////////
			Pos PointWhite = GetAddress();
			if (!vis[PointWhite.x][PointWhite.y] && black_go == 1)
			{
				vis[PointWhite.x][PointWhite.y] = 1;
				setfillcolor(WHITE);
				//MessageBox(NULL, to_string(PointWhite.y).c_str(), "Title(����)", MB_OK);
				fillcircle(map[PointWhite.x][PointWhite.y].x, map[PointWhite.x][PointWhite.y].y, chess_r / 3);
				PlaySound(TEXT("chessmusic.wav"), 0, SND_FILENAME);//��������
				putimage(780, 40, &NowChessImage);
				drawtext("��ڷ�����", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				color[PointWhite.x][PointWhite.y] = 0;
				if (JudgeChess(PointWhite.x, PointWhite.y) == true)//�����������
				{
					flag = false;
					GameOver2(0);
					break;
				}
			}
		}
	}
}
void chess::GameOver2(int n)
{
	settextcolor(RGB(254, 67, 101));
	LOGFONT f;//���������
	setbkmode(TRANSPARENT);//͸������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 120;
	strcpy_s(f.lfFaceName, "����");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	RECT r1 = { 0, 0, 800, 800 };
	RECT r2 = { 790, 710, 980, 760 };
	RECT r3 = { 780,40, 1000, 100 };
	setlinecolor(BLACK);
	if (n == 0)
	{
		drawtext("�� �� �� ʤ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��

	}
	else if (n == 1)
	{
		drawtext("�� �� �� ʤ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//���־���,���ִ�ֱ���У�ʹ������ʾ��һ��
	}
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 50;
	strcpy_s(f.lfFaceName, "΢���ź�");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	putimage(780, 40, &NowChessImage);
	drawtext("��Ϸ����", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//���þ���
	setlinestyle(PS_SOLID, 1);
	setlinecolor(BLACK);
	rectangle(790, 710, 980, 760);//�����¿�ʼ�ľ��α߿�
	setbkmode(TRANSPARENT);//͸������
	gettextstyle(&f);//��ȡ��ǰ��������
	f.lfHeight = 50;
	strcpy_s(f.lfFaceName, "΢���ź�");
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���
	settextstyle(&f);
	settextcolor(RGB(77, 77, 77));
	drawtext("���¿�ʼ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	bool flag = true;
	MOUSEMSG m;//����һ��������
	while (flag)
	{
		//����������ڿ�ʼ������ͼ��ִ�к��κλ�ͼ����������ʱ���������Ļ�ϣ�ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
		BeginBatchDraw();
		m = GetMouseMsg();//��ȡ�����Ϣ
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN://����������
			EndBatchDraw();
			if (m.x >= 790 && m.x <= 980 && m.y >= 710 && m.y <= 760)//�ж��ڿ�ʼ��Ϸ�ľ���������
			{
				flag = false;
				//***********************************************
				PlayGame2();//���¿�ʼ��Ϸ

			}
			//break;
		case WM_MOUSEMOVE://�������ƶ�
			EndBatchDraw();
			if (m.x >= 790 && m.x <= 980 && m.y >= 710 && m.y <= 760)//�ж�����ڿ�ʼ��Ϸ�ľ���������
			{
				settextcolor(RGB(254, 67, 101));
				drawtext("���¿�ʼ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				settextcolor(RGB(77, 77, 77));
				drawtext("���¿�ʼ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			//break;
		}
	}
}
Pos chess::GetAddress()//��������ǰ���ӵ�����
{
	Pos now;
	int choose_score = 1;//�з�,�����Ҫ���ش�ֵΪ0��Ҫ����Ϊ1
//������أ��жϺ�����������ɼ������ӣ�Ȼ����з��
	int flag = 1;//�ж��Ƿ�����ѭ��
	for (int x = 0; x <= 15 && flag; x++)
	{
		for (int y = 0; y <= 15 && flag; y++)
		{
			if (color[x][y] == 1 && flag)//�����ǰ���ӵ���ɫΪ��ɫ
			{
				int num = 0;//��¼������ɫ��������
							//����x����
				for (int i = y; i <= 15; i++)//����
				{
					if (color[x][i] == 1)
					{
						num++;
					}
					else
						break;
				}
				for (int i = y - 1; i >= 0; i--)//����
				{
					if (color[x][i] == 1)
					{
						num++;
					}
					else
						break;
				}
				if (num >= 3)//��������Ѿ���������
				{
					choose_score = 0;
					int flog = 1;//�����Ƿ����ҵ�
					for (int i = y; i <= 15 && flog; i++)//����
					{
						if (color[x][i] == -1)
						{
							flog = 0;
							now.x = x;
							now.y = i;
						}
					}
					for (int i = y - 1; i >= 0 && flog; i--)//����
					{
						if (color[x][i] == -1)
						{
							flog = 0;
							now.x = x;
							now.y = i;
						}

					}
					flag = 0;
					break;
				}
				num = 0;
				//����y����
				for (int i = x; i <= 15; i++)//����
				{
					if (color[i][y] == 1)
					{
						num++;
					}
					else
						break;
				}
				for (int i = x - 1; i >= 0; i--)//����
				{
					if (color[i][y] == 1)
					{
						num++;
					}
					else
						break;
				}
				if (num >= 3)//��������Ѿ���������
				{
					choose_score = 0;
					int flog = 1;
					for (int i = x; i <= 15 && flog; i++)//����
					{
						if (color[i][y] == -1)
						{
							flog = 0;
							now.x = i;
							now.y = y;
						}
					}
					for (int i = x - 1; i >= 0 && flog; i--)//����
					{
						if (color[i][y] == 1)
						{
							flog = 0;
							now.x = i;
							now.y = y;
						}
					}
					flag = 0;
					break;
				}
				num = 0;
				//������б������'\'
				for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j--)//�ɵ�ǰ��������
				{
					if (color[i][j] == 1)
					{
						num++;
					}
					else
						break;
				}
				for (int i = x + 1, j = y + 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j++)//�ɵ�ǰ��������
				{
					if (color[i][j] == 1)
					{
						num++;
					}
					else
						break;
				}
				if (num >= 3)//���'\'�Ѿ���������
				{
					choose_score = 0;
					int flog = 1;
					for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15) && flog; i--, j--)//�ɵ�ǰ��������
					{
						if (color[i][j] == -1)
						{
							flog = 0;
							now.x = i;
							now.y = j;
						}
					}
					for (int i = x + 1, j = y + 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15) && flog; i++, j++)//�ɵ�ǰ��������
					{
						if (color[i][j] == -1)
						{
							flog = 0;
							now.x = i;
							now.y = j;
						}
					}
					flag = 0;
					break;
				}
				num = 0;
				//������б������'/'
				for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j++)//�ɵ�ǰ��б������
				{
					if (color[i][j] == 1)
					{
						num++;
					}
					else
						break;
				}
				for (int i = x + 1, j = y - 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j--)//�ɵ�ǰ��б������
				{
					if (color[i][j] == 1)
					{
						num++;
					}
					else
						break;
				}
				if (num >= 3)//����Ѿ���������
				{
					choose_score = 0;
					int flog = 1;
					for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15) && flog; i--, j++)//�ɵ�ǰ��б������
					{
						if (color[i][j] == -1)
						{
							flog = 0;
							now.x = i;
							now.y = j;
						}
					}
					for (int i = x + 1, j = y - 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15) && flog; i++, j--)//�ɵ�ǰ��б������
					{
						if (color[i][j] == -1)
						{
							flog = 0;
							now.x = i;
							now.y = j;
						}
					}
					flag = 0;
					break;
				}
				num = 0;

			}
		}
	}
	//����������ҵ����Լ��������ģ��жϵ�ǰ�������������������
	int maxx = 0;
	if (choose_score)
	{
		for (int x = 0; x <= 15; x++)
		{
			for (int y = 0; y <= 15; y++)
			{
				if (color[x][y] == -1)//��һ���������
				{
					int num = 0;//��¼��������
					int max_num = 0;
					//����x����
					for (int i = y; i <= 15; i++)//����
					{
						if (color[x][i] == 0)
						{
							num++;
						}
						else
							break;
					}
					for (int i = y - 1; i >= 0; i--)//����
					{
						if (color[x][i] == 0)
						{
							num++;
						}
						else
							break;
					}
					max_num = max(max_num, num);
					num = 0;
					//����y����
					for (int i = x; i <= 15; i++)//����
					{
						if (color[i][y] == 0)
						{
							num++;
						}
						else
							break;
					}
					for (int i = x - 1; i >= 0; i--)//����
					{
						if (color[i][y] == 0)
						{
							num++;
						}
						else
							break;
					}
					max_num = max(max_num, num);
					num = 0;
					//������б������'\'
					for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j--)//�ɵ�ǰ��������
					{
						if (color[i][j] == 0)
						{
							num++;
						}
						else
							break;
					}
					for (int i = x + 1, j = y + 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j++)//�ɵ�ǰ��������
					{
						if (color[i][j] == 0)
						{
							num++;
						}
						else
							break;
					}
					max_num = max(max_num, num);
					num = 0;
					//������б������'/'
					for (int i = x, j = y; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i--, j++)//�ɵ�ǰ��б������
					{
						if (color[i][j] == 0)
						{
							num++;
						}
						else
							break;
					}
					for (int i = x + 1, j = y - 1; (i >= 0 && i <= 15 && j >= 0 && j <= 15); i++, j--)//�ɵ�ǰ��б������
					{
						if (color[i][j] == 0)
						{
							num++;
						}
						else
							break;
					}
					max_num = max(max_num, num);
					num = 0;

					if (maxx <= max_num)
					{
						maxx = max_num;
						now.x = x;
						now.y = y;
					}
				}
			}
		}
	}
	//MessageBox(NULL, to_string(now.x).c_str(), "Title(����)", MB_OK);
	return now;
}
int main()
{
	chess fivech;
	fivech.welcome();//��ӭ����
	if (MODE == 1)//˫��ģʽ
	{
		fivech.PlayGame1();
	}
	else if (MODE == 2)//����ai
	{
		fivech.PlayGame2();
	}
	getch();
	closegraph();
	return 0;
}
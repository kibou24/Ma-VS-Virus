#include<graphics.h>//ͼ�ο�
#include<conio.h>
#include<cmath>
#include <stdio.h>
#include<mmsystem.h>
#include <string>
#pragma comment(lib,"Winmm.lib")//���ڲ�����Ƶ


#define Width 540//���廭�����
#define High 860//���廭���߶�

//IMAGE ��һ���࣬����ͼƬ������
IMAGE Ma, Ma_zz,mahurt,mahurt_zz;//�������
IMAGE Virus, Virus_zz;//�������
IMAGE HLL, HLL_zz;//�ӵ����
IMAGE bg;//�������
IMAGE go, go_zz;

//��������
double v_x;
double v_y;

//�����ٶ�
double vspeed_x;
double vspeed_y;

//�����ٶ�
double cspeed_x;
double cspeed_y;

//��������
double c_x;
double c_y;

//����޴���
int CountBullet = 0;

int score = 0;


void Score()
{
	outtextxy(Width * 0.48, High * 0.95, "�÷֣�");
	char s[5];
	sprintf(s, "%d", score);
	outtextxy(Width * 0.55, High * 0.95, s);
}

void BulletSound()
{
	if (CountBullet % 5 == 0)
	{
		mciSendString(_T("close btmusic"), NULL, 0, NULL);
		mciSendString(_T("open data\\audio\\Bullet.mp3 alias btmusic"), NULL, 0, NULL);//����һ�������
		mciSendString(_T("play btmusic"), NULL, 0, NULL);
	}
	CountBullet++;
}

typedef struct node
{
	double H_x;
	double H_y;
	double H_speed = 6;
	bool life = true;//һЩ����

	struct node* next;
}Bullet, * PBullet;


PBullet ListHead = NULL;	//����ͷ��	����������
PBullet ListTail = NULL;	//����β��	��������

//�����ӵ�����
void CreatList(float x, float y)
{
	ListHead = new Bullet;//�½�ͷ�ڵ� һ�㲻�洢����
	ListHead->H_x = x;
	ListHead->H_y = y;
	ListHead->next = NULL;
	ListTail = ListHead;
}

void addbullet(float x, float y)
{
	if (!ListHead || !ListTail)//���ȣ��жϵ�ǰ�ӵ������Ƿ����ӵ������û�еĻ����ʹ���
	{
		CreatList(x, y);
	}
	else 
	{
		ListTail->next = new Bullet;
		ListTail->next->H_x = x;
		ListTail->next->H_y = y;
		ListTail->next->next = NULL;
		ListTail = ListTail->next;
	}
}

void Start()//��Ϸ��ֵ��ʼֵ
{
	mciSendString(_T("close idmusic"), NULL, 0, NULL);
	mciSendString(_T("open data\\audio\\Introduce.mp3 alias idmusic"), NULL, 0, NULL);
	mciSendString(_T("play idmusic"), NULL, 0, NULL);//����һ�����ҽ���


	mciSendString(_T("close bkmusic"), NULL, 0, NULL);
	mciSendString(_T("open data\\audio\\bgm.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);


	c_x = Width / 2;
	c_y = High - 300;

	cspeed_x = 20;
	cspeed_y = 20;

	v_x = Width / 2-45;
	v_y = 30;

	vspeed_y=1.1;

}



void LoadImage()//����ͼƬ
{

	loadimage(&bg, _T("data\\bg.png"),Width,High);//����ͼƬ

	loadimage(&Ma, _T("data\\Ma.png"), 110,200);
	loadimage(&Ma_zz, _T("data\\Ma_zz.png"), 110,200);//��������ͼƬ

	loadimage(&mahurt, _T("data\\mahurt.png"), 110, 200);
	loadimage(&mahurt_zz, _T("data\\mahurt_zz.png"), 110, 200);//��������ͼƬ

	loadimage(&Virus, _T("data\\virus.png"), 90,90);
	loadimage(&Virus_zz, _T("data\\virus_zz.png"), 90,90);//����ͼƬ

	//����ͼƬ������ָ�룬·�������ߣ�
	loadimage(&HLL, _T("data\\HLL.png"), 90, 90);//�ӵ�ͼƬ
	loadimage(&HLL_zz, _T("data\\HLL_zz.png"), 90, 90);//�ӵ�������ͼ

	loadimage(&go, _T("data\\go.png"), 250, 250);//�ӵ�ͼƬ
	loadimage(&go_zz, _T("data\\go_zz.png"), 250, 250);//�ӵ�������ͼ

	BeginBatchDraw();//��ʼ��������

}

void PutImage()//����ͼƬ
{
	PBullet Current = ListHead;
	
	putimage(0, 0, &bg);//���ñ���ͼƬ

	putimage(c_x,c_y, &Ma_zz, NOTSRCERASE);
	putimage(c_x, c_y, &Ma, SRCINVERT);//������������ͼƬ

//	����ͼƬ�������꣬�����꣬�����ַ��
//	putimage(H_x, H_y, &HLL_zz, NOTSRCERASE);
//	putimage(H_x, H_y, &HLL, SRCINVERT);//�����ӵ���Ƭ
//	��������ɫ���ֿ������ӵ�ͼƬ͸����ʾ

	while (Current)
	{
		putimage(Current->H_x, Current->H_y, &HLL_zz, NOTSRCERASE);
		putimage(Current->H_x, Current->H_y, &HLL, SRCINVERT);//�����ӵ���Ƭ
		Current = Current->next;
	}
	
	putimage(v_x,v_y, &Virus_zz, NOTSRCERASE);
	putimage(v_x, v_y, &Virus, SRCINVERT);//���ò�����Ƭ


	outtextxy(Width * 0.48, High * 0.95, "�÷֣�");
	char s[5];
	sprintf(s, "%d", score);
	outtextxy(Width * 0.55, High * 0.95, s);
	
	FlushBatchDraw();
	Sleep(2);

	//cleardevice();
	
}


void CMove() //����WASD����
{
	char kg;
	if (_kbhit())
	{
		kg = _getch();
		if (c_y >= 0 && (kg == 'w' || kg == 'W'))
		{
			c_y -= cspeed_y;
		}
		else if (c_x > 0 && (kg == 'a' || kg == 'A'))
		{
			c_x -= cspeed_x;
		}
		else if (c_x < Width - 70 && (kg == 'd' || kg == 'D'))
		{
			c_x += cspeed_x;
		}
		else if (c_y < High - 100 && (kg == 's' || kg == 'S'))
		{
			c_y += cspeed_y;
		}
	}
}

void MouseMove()//������
{
	MOUSEMSG m;
	
	while (MouseHit())
	{
		m = GetMouseMsg();

		if (m.uMsg == WM_MOUSEMOVE)//����ƶ����
		{
			c_x = m.x - 55;
			c_y = m.y - 100;
		}
		
		if (m.uMsg == WM_LBUTTONDOWN)//����������ʱ�Ķ���
		{
			addbullet(m.x - 55, m.y - 100);
			BulletSound();
		}
	}
}

void updateWithoutInput()//���û���������
{

	v_y += vspeed_y;

}


void Attack()
{
	PBullet Current = ListHead;
	while (Current)
	{
		double distance = sqrt((Current->H_x - v_x)*(Current->H_x - v_x) + (Current->H_y - v_y)*(Current->H_y - v_y));
		if (distance <= 35)
		{
			v_y = -90;    //rand() % (High / 4 - 90 + 1) + 90;
			v_x = rand() % (Width-120 - 90 + 1) + 90;
			vspeed_y += 0.05; 
			score++;
			//Current->life = false;
			
		}
		
		Current = Current->next;
	}
}

void Damage()
{
	
		
		double distance = sqrt((c_x - v_x) * (c_x - v_x) + (c_y - v_y) * (c_y - v_y));
		if (distance <= 30)
		{
			BeginBatchDraw();
			putimage(Width / 2 - 125, High / 2-100, &go_zz, NOTSRCERASE);
			putimage(Width / 2 - 125, High / 2-100, &go, SRCINVERT);//������Ϸ������Ƭ
			
			FlushBatchDraw();
			cleardevice();
			Score();
			mciSendString(_T("close BEmusic"), NULL, 0, NULL);
			mciSendString(_T("open data\\audio\\BigE.mp3 alias BEmusic"), NULL, 0, NULL);
			mciSendString(_T("play BEmusic"), NULL, 0, NULL);//����һ�����ҽ���
			Sleep(2000);
			_getch();
			closegraph();

		}
		else if (v_y > High - 100)
		{
			BeginBatchDraw();
			putimage(Width / 2 - 125, High / 2 - 100, &go_zz, NOTSRCERASE);
			putimage(Width / 2 - 125, High / 2 - 100, &go, SRCINVERT);//������Ϸ������Ƭ
			
			FlushBatchDraw();
			cleardevice();
			Score();
			mciSendString(_T("close DDWZmusic"), NULL, 0, NULL);
			mciSendString(_T("open data\\audio\\DDWZ.mp3 alias DDWZmusic"), NULL, 0, NULL);
			mciSendString(_T("play DDWZmusic"), NULL, 0, NULL);//����һ�����ҽ���

			Sleep(2000);
			_getch();
			closegraph();
		}

		
		
	
}
void updata()
{
	PBullet Current = ListHead;

	PBullet Temp = NULL;	//����������һ���ڵ�

	PBullet Temp1 = NULL;	//����������һ���ڵ�

	while (Current)
	{

		Current->H_y = Current->H_y - Current->H_speed;	//�����ٶ��ƶ��ӵ�����

		if (Current->H_y <= -90)//���ж��ӵ��Ƿ����Χ��������˷�Χ����ɾ������
		{
			if (!Temp)//�����һ������ڵ���NULL ��ô˵����ǰ�ӵ�Ϊ��һ��
			{
				if (!Current->next)//�����һ���ӵ�����û�еڶ����ӵ�����ô�������ӵ����Ҹ�ֵNULL
				{
					delete ListHead;
					ListHead = NULL;
					return;
				}
				else//�����һ���ӵ����滹���ӵ�
				{
					Temp1 = Current->next;//����һ���ӵ�����ĵڶ����ӵ��ڵ������ڵ�
					delete Current;//�ͷŵ���һ���ӵ����ڴ�
					ListHead = Temp1;//�õڶ����ӵ���Ϊ��ͷ
					Current = Temp1;
				}
			}
			else//�����һ������ڵ㲻��NULL��˵����ǰ�������ӵ���ͷ
			{
				if (!Current->next)//�����һ���ڵ�Ϊ�գ��ͽ���ǰ�ӵ��ڴ��ͷ�
				{
					delete Temp->next;
					Temp->next = NULL;
					return;
				}
				else//�����һ���ڵ㲻Ϊ��
				{
					Temp1 = Current->next;//��һ���ڵ������ڵ�
					delete Temp->next;//�ͷŵ�ǰ�ڵ�
					Temp->next = Temp1;//����ڵ㸳ֵ����һ��
					Current = Temp;
				}
			}
		}

		Temp = Current;
		Current = Current->next;
	}
}

int main()
{
	
	initgraph(Width, High);//���ƻ���
	LoadImage();

	Start();

	
	while (1)
	{
		updateWithoutInput();
		updata();
		
		PutImage();
		Attack();
		Damage();
		
		//ShowHLL();
		//CMove();
		MouseMove();
	}

	_getch();//�����������
	closegraph();//�رջ���
}
#include<graphics.h>//图形库
#include<conio.h>
#include<cmath>
#include <stdio.h>
#include<mmsystem.h>
#include <string>
#pragma comment(lib,"Winmm.lib")//用于播放音频


#define Width 540//定义画布宽度
#define High 860//定义画布高度

//IMAGE 是一个类，包含图片的属性
IMAGE Ma, Ma_zz,mahurt,mahurt_zz;//人物相关
IMAGE Virus, Virus_zz;//病毒相关
IMAGE HLL, HLL_zz;//子弹相关
IMAGE bg;//背景相关
IMAGE go, go_zz;

//病毒坐标
double v_x;
double v_y;

//病毒速度
double vspeed_x;
double vspeed_y;

//人物速度
double cspeed_x;
double cspeed_y;

//人物坐标
double c_x;
double c_y;

//闪电鞭次数
int CountBullet = 0;

int score = 0;


void Score()
{
	outtextxy(Width * 0.48, High * 0.95, "得分：");
	char s[5];
	sprintf(s, "%d", score);
	outtextxy(Width * 0.55, High * 0.95, s);
}

void BulletSound()
{
	if (CountBullet % 5 == 0)
	{
		mciSendString(_T("close btmusic"), NULL, 0, NULL);
		mciSendString(_T("open data\\audio\\Bullet.mp3 alias btmusic"), NULL, 0, NULL);//播放一次闪电鞭
		mciSendString(_T("play btmusic"), NULL, 0, NULL);
	}
	CountBullet++;
}

typedef struct node
{
	double H_x;
	double H_y;
	double H_speed = 6;
	bool life = true;//一些数据

	struct node* next;
}Bullet, * PBullet;


PBullet ListHead = NULL;	//链表头部	用来遍历画
PBullet ListTail = NULL;	//链表尾部	用来增加

//创建子弹链表
void CreatList(float x, float y)
{
	ListHead = new Bullet;//新建头节点 一般不存储数据
	ListHead->H_x = x;
	ListHead->H_y = y;
	ListHead->next = NULL;
	ListTail = ListHead;
}

void addbullet(float x, float y)
{
	if (!ListHead || !ListTail)//首先，判断当前子弹表里是否有子弹，如果没有的话，就创建
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

void Start()//游戏数值初始值
{
	mciSendString(_T("close idmusic"), NULL, 0, NULL);
	mciSendString(_T("open data\\audio\\Introduce.mp3 alias idmusic"), NULL, 0, NULL);
	mciSendString(_T("play idmusic"), NULL, 0, NULL);//播放一次自我介绍


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



void LoadImage()//加载图片
{

	loadimage(&bg, _T("data\\bg.png"),Width,High);//背景图片

	loadimage(&Ma, _T("data\\Ma.png"), 110,200);
	loadimage(&Ma_zz, _T("data\\Ma_zz.png"), 110,200);//人物正常图片

	loadimage(&mahurt, _T("data\\mahurt.png"), 110, 200);
	loadimage(&mahurt_zz, _T("data\\mahurt_zz.png"), 110, 200);//人物受伤图片

	loadimage(&Virus, _T("data\\virus.png"), 90,90);
	loadimage(&Virus_zz, _T("data\\virus_zz.png"), 90,90);//病毒图片

	//载入图片（传入指针，路径，宽，高）
	loadimage(&HLL, _T("data\\HLL.png"), 90, 90);//子弹图片
	loadimage(&HLL_zz, _T("data\\HLL_zz.png"), 90, 90);//子弹的遮罩图

	loadimage(&go, _T("data\\go.png"), 250, 250);//子弹图片
	loadimage(&go_zz, _T("data\\go_zz.png"), 250, 250);//子弹的遮罩图

	BeginBatchDraw();//开始批量绘制

}

void PutImage()//放置图片
{
	PBullet Current = ListHead;
	
	putimage(0, 0, &bg);//放置背景图片

	putimage(c_x,c_y, &Ma_zz, NOTSRCERASE);
	putimage(c_x, c_y, &Ma, SRCINVERT);//放置人物正常图片

//	放置图片（横坐标，纵坐标，传入地址）
//	putimage(H_x, H_y, &HLL_zz, NOTSRCERASE);
//	putimage(H_x, H_y, &HLL, SRCINVERT);//放置子弹照片
//	这两个紫色部分可以让子弹图片透明显示

	while (Current)
	{
		putimage(Current->H_x, Current->H_y, &HLL_zz, NOTSRCERASE);
		putimage(Current->H_x, Current->H_y, &HLL, SRCINVERT);//放置子弹照片
		Current = Current->next;
	}
	
	putimage(v_x,v_y, &Virus_zz, NOTSRCERASE);
	putimage(v_x, v_y, &Virus, SRCINVERT);//放置病毒照片


	outtextxy(Width * 0.48, High * 0.95, "得分：");
	char s[5];
	sprintf(s, "%d", score);
	outtextxy(Width * 0.55, High * 0.95, s);
	
	FlushBatchDraw();
	Sleep(2);

	//cleardevice();
	
}


void CMove() //键盘WASD操作
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

void MouseMove()//鼠标操作
{
	MOUSEMSG m;
	
	while (MouseHit())
	{
		m = GetMouseMsg();

		if (m.uMsg == WM_MOUSEMOVE)//鼠标移动检测
		{
			c_x = m.x - 55;
			c_y = m.y - 100;
		}
		
		if (m.uMsg == WM_LBUTTONDOWN)//按下鼠标左键时的动作
		{
			addbullet(m.x - 55, m.y - 100);
			BulletSound();
		}
	}
}

void updateWithoutInput()//非用户输入内容
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
			putimage(Width / 2 - 125, High / 2-100, &go, SRCINVERT);//放置游戏结束照片
			
			FlushBatchDraw();
			cleardevice();
			Score();
			mciSendString(_T("close BEmusic"), NULL, 0, NULL);
			mciSendString(_T("open data\\audio\\BigE.mp3 alias BEmusic"), NULL, 0, NULL);
			mciSendString(_T("play BEmusic"), NULL, 0, NULL);//播放一次自我介绍
			Sleep(2000);
			_getch();
			closegraph();

		}
		else if (v_y > High - 100)
		{
			BeginBatchDraw();
			putimage(Width / 2 - 125, High / 2 - 100, &go_zz, NOTSRCERASE);
			putimage(Width / 2 - 125, High / 2 - 100, &go, SRCINVERT);//放置游戏结束照片
			
			FlushBatchDraw();
			cleardevice();
			Score();
			mciSendString(_T("close DDWZmusic"), NULL, 0, NULL);
			mciSendString(_T("open data\\audio\\DDWZ.mp3 alias DDWZmusic"), NULL, 0, NULL);
			mciSendString(_T("play DDWZmusic"), NULL, 0, NULL);//播放一次自我介绍

			Sleep(2000);
			_getch();
			closegraph();
		}

		
		
	
}
void updata()
{
	PBullet Current = ListHead;

	PBullet Temp = NULL;	//用来缓存上一个节点

	PBullet Temp1 = NULL;	//用来缓存下一个节点

	while (Current)
	{

		Current->H_y = Current->H_y - Current->H_speed;	//根据速度移动子弹坐标

		if (Current->H_y <= -90)//首判断子弹是否出范围，如果出了范围就做删除处理
		{
			if (!Temp)//如果上一个缓存节点是NULL 那么说明当前子弹为第一颗
			{
				if (!Current->next)//如果第一颗子弹后面没有第二颗子弹，那么就重置子弹表并且赋值NULL
				{
					delete ListHead;
					ListHead = NULL;
					return;
				}
				else//如果第一颗子弹后面还有子弹
				{
					Temp1 = Current->next;//将第一颗子弹下面的第二颗子弹节点给缓存节点
					delete Current;//释放掉第一颗子弹的内存
					ListHead = Temp1;//让第二颗子弹作为表头
					Current = Temp1;
				}
			}
			else//如果上一个缓存节点不是NULL，说明当前并不是子弹表头
			{
				if (!Current->next)//如果下一个节点为空，就将当前子弹内存释放
				{
					delete Temp->next;
					Temp->next = NULL;
					return;
				}
				else//如果下一个节点不为空
				{
					Temp1 = Current->next;//下一个节点给缓存节点
					delete Temp->next;//释放当前节点
					Temp->next = Temp1;//缓存节点赋值给下一个
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
	
	initgraph(Width, High);//绘制画布
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

	_getch();//按任意键继续
	closegraph();//关闭画布
}
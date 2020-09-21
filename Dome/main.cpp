#include <iostream>
#include <easyx.h>			// 引用图形库头文件
#include <conio.h>
#include<stdio.h >
#include <fstream>
#include <cassert>
#include <string>
#include <windows.h>
#include <conio.h>
#include <graphics.h>
#pragma comment(lib,"Winmm.lib")
#include "Tool.h"

using namespace std;
const int PLANE_NORMAL =1;
const int PLANE_ATTACK =40;
const int PLANE_HITTED =80;

/////////////////////////////////////////////
// 定义常量、枚举量、结构体、全局变量、用于控制飞机和读取暂停界面时的按键
/////////////////////////////////////////////

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_RETURN	    16
#define	CMD_QUIT		64




/////////////////////////////////////////////
// 函数声明
/////////////////////////////////////////////


void Quit();						// 退出
int  GetCommand();					// 获取控制命令
void DispatchCommand(int _cmd);		// 分发控制命令
void OnUp();						// 上移
void OnDown();						// 下移
void OnLeft();						// 左移
void OnRight();						// 右移



/////////////////////////////////////////////
// 函数定义
/////////////////////////////////////////////


// 获取控制命令
int GetCommand()
{
	int c = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)	c |= CMD_QUIT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)	c |= CMD_RETURN;

	return c;
}

// 分发控制命令
void DispatchCommand(int _cmd)
{
	if (_cmd & CMD_UP)			OnUp();
	if (_cmd & CMD_DOWN)		OnDown();
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
}


/*
	构造飞机
*/
struct Plane                
{
	int positionX;
	int positionY;
	int Life;
	int type;
};

/*
	构造子弹
*/
struct Bullet				
{
	POINT position;
	int is_active;			//判断是否被激活
};



/*
	构造敌人
*/
struct Enemy
{
	POINT position;
	int is_active;			//判断是否被激活
	int is_aive;
};

Plane plane;    //定义一架飞机
int planeX = 0;  //飞机X
int planeY = 0;  //飞机Y
int informationEnemyLength = 0;    //数据显示区域的敌人数量
int informationBulletLength = 0;   //数据显示区域的剩余子弹数量
int informationEnemyAttacked =0;   //被集中敌机的数量
const int bulletLength = 10000;    ///子弹数量
const int enemyLength = 1000;     //敌人数量
  
Bullet bullet[bulletLength];      //设计1w个子弹，没有弹性，需要修改
int bulletnum = 0;			//发射的数量
int enemynum = 0;         //子弹的中间变量

Enemy enemy[enemyLength];


void init();                //大方法初始化
void userInput();			//大方法用户输入
void clear();				//大方法清屏幕
void draw();				//大方法重画

void initBackground();
void initPlane();
void initBullet();
void initEnemy();
void initLoginInterface();
void initInformation();
void playmusic();
void stopmusic();
void shootmusic();


void getKey();
void ToUpdate();

void drawBackground();
void drawPlane();
void drawBullet();
void drawEnemy();
void drawInformation();



//读取开始菜单所需文件

void readTxt(string file)
{
	ifstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

	string s;
	while (getline(infile, s))
	{
		cout << s << endl;
	}
	infile.close();             //关闭文件输入流
}


//开始菜单
int menus()
{
	readTxt("4.txt");
	while (true)
	{


		char begintitle[] = "           press any key to start...          ";
		cout << begintitle;

		Sleep(500);

		for (int i = 0; i < strlen(begintitle); i++)
			cout << "\b \b";

		Sleep(25);

		if (_kbhit())
			break;
	}


	system("cls");   //清屏，进入加载界面
	readTxt("5.txt");
	for (int i = 0; i <= 80; i++)
	{
		cout << '>';
		Sleep(25);
	}
	system("cls");  //加载完成清屏



	return 0;

}


//碰撞监测
int main()
{
	menus();     //TODO:在这个地方加上一个上下选择的界面
restart:       //重新开始跳转到这里，不过需要重置子弹数和击中的敌人数量
	init();
	//playmusic();
	int c;
loop:        //继续游戏跳转到这儿
	//playmusic();
	do                     //循环读取按键状态控制飞机
	{
		clear();
		userInput();
		draw();
		FlushBatchDraw();
		c = GetCommand();
		DispatchCommand(c);
		Sleep(5);
	} while (!(c & CMD_QUIT));

	Quit();          // 暂停界面；
	Sleep(300);

	while (true)                 
	{
		char d;
		d = getch();
		c = GetCommand();
		if (c & CMD_QUIT)                  //退出游戏
			break;
		if (d == char('r') || d==char('R'))// 重新开始
			goto restart;
		if (c & CMD_RETURN)                //继续游戏
			goto loop;

	}


	return 0;
}

// 暂停菜单
void Quit()
{
	//stopmusic();
	for (int i = 0; i < 20; i++)                                                 //绘制框架
	{
		fillrectangle(200 + i * 20, 200, 200 + i*20 + 20  , 220);
	}
	for (int i = 0; i < 20; i++)
	{
		fillrectangle(200 + i * 20, 400, 200 + i * 20 + 20, 420);
	}
	for (int i = 0; i < 10; i++)
	{
		fillrectangle(200, 200 + i * 20, 220, 220 + i * 20 + 20);
	}
	for (int i = 0; i < 10; i++)
	{
		fillrectangle(200 + 19 * 20 , 200 + i * 20, 220 + 19*20, 220 + i * 20 + 20);
	}
	outtextxy(350, 250, "ENTER 回到游戏");
	outtextxy(350, 280, "  R   重新开始");
	outtextxy(350, 310, " ESC  退出游戏");



}

/*初始化方法调用
	initBackground();
	initPlane();
	initBullet();
	对整easyx界面初始化
*/
void init()
{
	plane.Life = 3;
	plane.type = 0;
	initBackground();
	initPlane();
	initBullet();
	initEnemy();
	initLoginInterface();
	initInformation();
}
/*播放背景音乐*/
void playmusic()
{
	// 打开音乐
	mciSendString("open background.mp3 alias mymusic", NULL, 0, NULL);

	// 播放音乐
		mciSendString("play mymusic repeat", NULL, 0, NULL);

}
/*暂停播放背景音乐*/
void stopmusic()
{
	// 打开音乐
	mciSendString("open background.mp3 alias mymusic", NULL, 0, NULL);
	mciSendString("stop mymusic", NULL, 0, NULL);
	mciSendString("close mymusic", NULL, 0, NULL);
}
/*击中音乐*/
void shootmusic()
{
	// 打开音乐
	mciSendString("open shoot.mp3 alias mymusic", NULL, 0, NULL);
	// 播放音乐
	mciSendString("play mymusic repeat", NULL, 0, NULL);
}
/*
	getKey();对用户数据进行截取
	截取数据放到公共部分
	使用ToUpdate();将截取的数据放回结构体变量中，完成对数据的更新
*/
void userInput()
{
	getKey();
	ToUpdate();
}

/*
	清除上一个循环的残余图案
*/
void clear()
{
	clearrectangle(0,0,700,700);
	clearrectangle(722,22,878,678);
}

/*
	从结构体变量中取出数据
	重绘制
*/
void draw()
{
	drawPlane();
	drawBullet();
	drawEnemy();
	drawInformation();
}

/*
	初始化函数集合
*/
void initBackground()
{
	initgraph(900, 700);	// 创建绘图窗口，大小为 640x480 像素
	
}
void initPlane()
{
	plane.Life = 0;
	planeX = 200;
	planeY = 200;
	plane.positionX = planeX;
	plane.positionY = planeY;
		//机身
	
	fillrectangle(plane.positionX,plane.positionY,10+plane.positionX,10+plane.positionY);
	fillrectangle(plane.positionX,10+plane.positionY,10+plane.positionX,20+plane.positionY);
	fillrectangle(plane.positionX,20+plane.positionY,10+plane.positionX,30+plane.positionY);
	fillrectangle(plane.positionX,30+plane.positionY,10+plane.positionX,40+plane.positionY);
	fillrectangle(plane.positionX,40+plane.positionY,10+plane.positionX,50+plane.positionY);
	//左机翼
	fillrectangle(plane.positionX-10,40+plane.positionY,plane.positionX,50+plane.positionY);
	fillrectangle(plane.positionX-20,40+plane.positionY,plane.positionX-10,50+plane.positionY);
	fillrectangle(plane.positionX-30,40+plane.positionY,plane.positionX-20,50+plane.positionY);
	fillrectangle(plane.positionX-40,30+plane.positionY,plane.positionX-30,40+plane.positionY);
	//右机翼
	fillrectangle(plane.positionX+10,40+plane.positionY,plane.positionX+20,50+plane.positionY);
	fillrectangle(plane.positionX+20,40+plane.positionY,plane.positionX+30,50+plane.positionY);
	fillrectangle(plane.positionX+30,40+plane.positionY,plane.positionX+40,50+plane.positionY);
	fillrectangle(plane.positionX+40,30+plane.positionY,plane.positionX+50,40+plane.positionY);
}
void initBullet()
{	
	bulletnum = 0;
	for(int i=0;i<bulletLength;i++){
		bullet[i].is_active = 0;
	}
}
void initEnemy()
{
	for(int i=0;i<enemyLength;i++)
	{
		enemy[i].position.x =  getRandInt(0,640);
		enemy[i].position.y =  0;
		enemy[i].is_active = 0;
		enemy[i].is_aive = 1;
	}
}
void initLoginInterface()
{
	for(int i=0;i<20;i++)
	{
		rectangle(701+i*20,0,700+i*20+20,20);
	}
	for(int i=0;i<20;i++)
	{
		rectangle(701+i*20,680,700+i*20+20,700);
	}
	for(int i=0;i<35;i++)
	{
		rectangle(701,i*20,720,i*20+20);
	}
	for(int i=0;i<35;i++)
	{
		rectangle(880,i*20,900,i*20+20);
	}
}
void initInformation()
{
	informationEnemyLength = enemyLength;
	informationBulletLength = bulletLength;
	informationEnemyAttacked = 0;

	outtextxy(750,80,"敌机数量");
	char num[20];
	sprintf(num, "%d", informationEnemyLength);
	outtextxy(750,100,num);



	outtextxy(750,120,"子弹数量");
	char num2[20];
	sprintf(num2, "%d", informationBulletLength);
	outtextxy(750,140,num2);


	outtextxy(750,160,"击中敌人数量");
	char num3[20];
	sprintf(num3, "%d", informationEnemyAttacked);
	outtextxy(750,180,num3);
}


/*
	用户输入集合  
*/

void getKey()
{
	if(_kbhit()){
	char val  = _getch();
	switch(val)
	{
	case 'w':
	case 'W':
		if(planeY == 0)
		{
			planeY = planeY;
		}
		else
		{
			planeY = planeY - 30;
		}
		break;
	case 's':
	case 'S':
		if(planeY >= 630)
		{
			planeY = planeY;
		}
		else
		{
			planeY = planeY + 30;
		}
		break;
	case 'a':
	case 'A':
		if(planeX == 20)
		{
			planeX = planeX;
		}
		else
		{
			planeX = planeX - 30;
		}
		break;
	case 'd':
	case 'D':
		if(planeX ==650)
		{
			planeX = planeX;
		}
		else
		{
			planeX = planeX + 30;
		}
		break;
	case ' ':
		plane.type=40;
		bulletnum++;
		informationBulletLength--;
		bullet[bulletnum].is_active = 1;
		bullet[bulletnum].position.x = plane.positionX;
		bullet[bulletnum].position.y = plane.positionY-10;
		break;
	}
	}
}

// 上移
void OnUp()
{
	if (planeY <= 0) planeY = 0; else planeY -= 3;
}

// 下移
void OnDown()
{
	if (planeY >= 630) planeY = 630; else planeY += 3;
}

// 左移
void OnLeft()
{
	if (planeX <= 30) planeX = 30; else planeX -= 3;
}

// 右移
void OnRight()
{
	if (planeX >= 648) planeX = 648; else planeX += 3;
}

void ToUpdate()
{
	plane.positionX = planeX;
	plane.positionY = planeY;


	//更新敌人位置
	enemynum =  timeApperNums(1000);
		if(enemynum != -1)
		{
			enemy[enemynum].is_active=1;
		}
		for(int i=0;i<enemyLength;i++)
		{
			if(enemy[i].position.y>700)
			{
				enemy[i].is_active = 0;
			}
			if(enemy[i].is_active)
			{
				enemy[i].position.y  = enemy[i].position.y + 3;
			}
		}



	//更新子弹位置
	for(int i=0;i<10000;i++){
		if(bullet[i].is_active){
			if(bullet[i].position.y<0){bullet[i].is_active=0;}
			else{
			bullet[i].position.y = bullet[i].position.y - 8;
			}
			//碰撞监测
			for(int j=0;j<1000;j++){
				if(enemy[j].is_active)
				{

					if(bullet[i].position.x>=enemy[j].position.x&&bullet[i].position.x+10<enemy[j].position.x+30&&bullet[i].position.y>=enemy[j].position.y&&bullet[i].position.y<=enemy[j].position.y+30)
					{
						bullet[i].is_active = 0;
						enemy[j].is_active = 0;
						informationEnemyAttacked ++;
						//shootmusic();
						
					}
					if(bullet[i].position.x<=enemy[j].position.x&&bullet[i].position.x+10>enemy[j].position.x&&bullet[i].position.y>=enemy[j].position.y&&bullet[i].position.y<=enemy[j].position.y+30)
					{
						bullet[i].is_active = 0;
						enemy[j].is_active = 0;
						informationEnemyAttacked ++;
						shootmusic();
						
					}
					if(bullet[i].position.x>=enemy[j].position.x&&bullet[i].position.x+10<enemy[j].position.x+30&&bullet[i].position.y>=enemy[j].position.y&&bullet[i].position.y<=enemy[j].position.y+30)
					{
						bullet[i].is_active = 0;
						enemy[j].is_active = 0;
						shootmusic();
					}
				}
			}
		}
	}


}



/*
	重画集合
*/
void drawBullet()
{
	for(int i=0;i<bulletLength;i++){
		if(bullet[i].is_active){   //如果该子弹处于激活状态
			setlinecolor(BLACK);
			setfillcolor(WHITE);
			fillrectangle(bullet[i].position.x,bullet[i].position.y,bullet[i].position.x+10,bullet[i].position.y+10); //绘制子弹
		}
	}
}
void drawPlane()
{
	//机身
	plane.positionX = planeX;
	plane.positionY = planeY;
	setlinecolor(BLACK);
	if(plane.type==0){setfillcolor(WHITE);}
	else if(1<=plane.type&&plane.type<=40){setfillcolor(GREEN);plane.type -= 5 ;}
	//else if(plane.type==80){setfillcolor(RED);}
	fillrectangle(plane.positionX,plane.positionY,10+plane.positionX,10+plane.positionY);
	fillrectangle(plane.positionX,10+plane.positionY,10+plane.positionX,20+plane.positionY);
	fillrectangle(plane.positionX,20+plane.positionY,10+plane.positionX,30+plane.positionY);
	fillrectangle(plane.positionX,30+plane.positionY,10+plane.positionX,40+plane.positionY);
	fillrectangle(plane.positionX,40+plane.positionY,10+plane.positionX,50+plane.positionY);
	//左机翼
	fillrectangle(plane.positionX-10,40+plane.positionY,plane.positionX,50+plane.positionY);
	fillrectangle(plane.positionX-20,40+plane.positionY,plane.positionX-10,50+plane.positionY);
	fillrectangle(plane.positionX-20,50+plane.positionY,plane.positionX-10,60+plane.positionY);
	fillrectangle(plane.positionX-30,40+plane.positionY,plane.positionX-20,50+plane.positionY);
	fillrectangle(plane.positionX-40,30+plane.positionY,plane.positionX-30,40+plane.positionY);
	//右机翼
	fillrectangle(plane.positionX+10,40+plane.positionY,plane.positionX+20,50+plane.positionY);
	fillrectangle(plane.positionX+20,40+plane.positionY,plane.positionX+30,50+plane.positionY);
	fillrectangle(plane.positionX+20,50+plane.positionY,plane.positionX+30,60+plane.positionY);
	fillrectangle(plane.positionX+30,40+plane.positionY,plane.positionX+40,50+plane.positionY);
	fillrectangle(plane.positionX+40,30+plane.positionY,plane.positionX+50,40+plane.positionY);
}
void drawEnemy()
{
	for(int i=0;i<enemyLength;i++)
		{
			if(enemy[i].is_active)
			{
				setfillcolor(RED);
				fillrectangle(enemy[i].position.x,enemy[i].position.y,enemy[i].position.x+30,enemy[i].position.y+30);
			}
		}
}
void drawInformation()
{
	
	outtextxy(750,80,"敌机数量");
	char num[20];
	sprintf(num, "%d", informationEnemyLength);
	outtextxy(750,100,num);


	outtextxy(750,120,"子弹数量");
	char num2[20];
	sprintf(num2, "%d", informationBulletLength);
	outtextxy(750,140,num2);

	outtextxy(750,160,"击中敌人数量");
	char num3[20];
	sprintf(num3, "%d", informationEnemyAttacked);
	outtextxy(750,180,num3);
	outtextxy(750, 580, "控制：");
	outtextxy(750, 600, "←→↑↓");
	outtextxy(750, 620, "攻击： 空格");
	outtextxy(750, 640, "暂停：ESC");
	
}



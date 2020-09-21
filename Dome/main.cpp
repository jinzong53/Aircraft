#include <iostream>
#include <easyx.h>			// ����ͼ�ο�ͷ�ļ�
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
// ���峣����ö�������ṹ�塢ȫ�ֱ��������ڿ��Ʒɻ��Ͷ�ȡ��ͣ����ʱ�İ���
/////////////////////////////////////////////

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_RETURN	    16
#define	CMD_QUIT		64




/////////////////////////////////////////////
// ��������
/////////////////////////////////////////////


void Quit();						// �˳�
int  GetCommand();					// ��ȡ��������
void DispatchCommand(int _cmd);		// �ַ���������
void OnUp();						// ����
void OnDown();						// ����
void OnLeft();						// ����
void OnRight();						// ����



/////////////////////////////////////////////
// ��������
/////////////////////////////////////////////


// ��ȡ��������
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

// �ַ���������
void DispatchCommand(int _cmd)
{
	if (_cmd & CMD_UP)			OnUp();
	if (_cmd & CMD_DOWN)		OnDown();
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
}


/*
	����ɻ�
*/
struct Plane                
{
	int positionX;
	int positionY;
	int Life;
	int type;
};

/*
	�����ӵ�
*/
struct Bullet				
{
	POINT position;
	int is_active;			//�ж��Ƿ񱻼���
};



/*
	�������
*/
struct Enemy
{
	POINT position;
	int is_active;			//�ж��Ƿ񱻼���
	int is_aive;
};

Plane plane;    //����һ�ܷɻ�
int planeX = 0;  //�ɻ�X
int planeY = 0;  //�ɻ�Y
int informationEnemyLength = 0;    //������ʾ����ĵ�������
int informationBulletLength = 0;   //������ʾ�����ʣ���ӵ�����
int informationEnemyAttacked =0;   //�����ел�������
const int bulletLength = 10000;    ///�ӵ�����
const int enemyLength = 1000;     //��������
  
Bullet bullet[bulletLength];      //���1w���ӵ���û�е��ԣ���Ҫ�޸�
int bulletnum = 0;			//���������
int enemynum = 0;         //�ӵ����м����

Enemy enemy[enemyLength];


void init();                //�󷽷���ʼ��
void userInput();			//�󷽷��û�����
void clear();				//�󷽷�����Ļ
void draw();				//�󷽷��ػ�

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



//��ȡ��ʼ�˵������ļ�

void readTxt(string file)
{
	ifstream infile;
	infile.open(file.data());   //���ļ����������ļ���������
	assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ��������

	string s;
	while (getline(infile, s))
	{
		cout << s << endl;
	}
	infile.close();             //�ر��ļ�������
}


//��ʼ�˵�
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


	system("cls");   //������������ؽ���
	readTxt("5.txt");
	for (int i = 0; i <= 80; i++)
	{
		cout << '>';
		Sleep(25);
	}
	system("cls");  //�����������



	return 0;

}


//��ײ���
int main()
{
	menus();     //TODO:������ط�����һ������ѡ��Ľ���
restart:       //���¿�ʼ��ת�����������Ҫ�����ӵ����ͻ��еĵ�������
	init();
	//playmusic();
	int c;
loop:        //������Ϸ��ת�����
	//playmusic();
	do                     //ѭ����ȡ����״̬���Ʒɻ�
	{
		clear();
		userInput();
		draw();
		FlushBatchDraw();
		c = GetCommand();
		DispatchCommand(c);
		Sleep(5);
	} while (!(c & CMD_QUIT));

	Quit();          // ��ͣ���棻
	Sleep(300);

	while (true)                 
	{
		char d;
		d = getch();
		c = GetCommand();
		if (c & CMD_QUIT)                  //�˳���Ϸ
			break;
		if (d == char('r') || d==char('R'))// ���¿�ʼ
			goto restart;
		if (c & CMD_RETURN)                //������Ϸ
			goto loop;

	}


	return 0;
}

// ��ͣ�˵�
void Quit()
{
	//stopmusic();
	for (int i = 0; i < 20; i++)                                                 //���ƿ��
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
	outtextxy(350, 250, "ENTER �ص���Ϸ");
	outtextxy(350, 280, "  R   ���¿�ʼ");
	outtextxy(350, 310, " ESC  �˳���Ϸ");



}

/*��ʼ����������
	initBackground();
	initPlane();
	initBullet();
	����easyx�����ʼ��
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
/*���ű�������*/
void playmusic()
{
	// ������
	mciSendString("open background.mp3 alias mymusic", NULL, 0, NULL);

	// ��������
		mciSendString("play mymusic repeat", NULL, 0, NULL);

}
/*��ͣ���ű�������*/
void stopmusic()
{
	// ������
	mciSendString("open background.mp3 alias mymusic", NULL, 0, NULL);
	mciSendString("stop mymusic", NULL, 0, NULL);
	mciSendString("close mymusic", NULL, 0, NULL);
}
/*��������*/
void shootmusic()
{
	// ������
	mciSendString("open shoot.mp3 alias mymusic", NULL, 0, NULL);
	// ��������
	mciSendString("play mymusic repeat", NULL, 0, NULL);
}
/*
	getKey();���û����ݽ��н�ȡ
	��ȡ���ݷŵ���������
	ʹ��ToUpdate();����ȡ�����ݷŻؽṹ������У���ɶ����ݵĸ���
*/
void userInput()
{
	getKey();
	ToUpdate();
}

/*
	�����һ��ѭ���Ĳ���ͼ��
*/
void clear()
{
	clearrectangle(0,0,700,700);
	clearrectangle(722,22,878,678);
}

/*
	�ӽṹ�������ȡ������
	�ػ���
*/
void draw()
{
	drawPlane();
	drawBullet();
	drawEnemy();
	drawInformation();
}

/*
	��ʼ����������
*/
void initBackground()
{
	initgraph(900, 700);	// ������ͼ���ڣ���СΪ 640x480 ����
	
}
void initPlane()
{
	plane.Life = 0;
	planeX = 200;
	planeY = 200;
	plane.positionX = planeX;
	plane.positionY = planeY;
		//����
	
	fillrectangle(plane.positionX,plane.positionY,10+plane.positionX,10+plane.positionY);
	fillrectangle(plane.positionX,10+plane.positionY,10+plane.positionX,20+plane.positionY);
	fillrectangle(plane.positionX,20+plane.positionY,10+plane.positionX,30+plane.positionY);
	fillrectangle(plane.positionX,30+plane.positionY,10+plane.positionX,40+plane.positionY);
	fillrectangle(plane.positionX,40+plane.positionY,10+plane.positionX,50+plane.positionY);
	//�����
	fillrectangle(plane.positionX-10,40+plane.positionY,plane.positionX,50+plane.positionY);
	fillrectangle(plane.positionX-20,40+plane.positionY,plane.positionX-10,50+plane.positionY);
	fillrectangle(plane.positionX-30,40+plane.positionY,plane.positionX-20,50+plane.positionY);
	fillrectangle(plane.positionX-40,30+plane.positionY,plane.positionX-30,40+plane.positionY);
	//�һ���
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

	outtextxy(750,80,"�л�����");
	char num[20];
	sprintf(num, "%d", informationEnemyLength);
	outtextxy(750,100,num);



	outtextxy(750,120,"�ӵ�����");
	char num2[20];
	sprintf(num2, "%d", informationBulletLength);
	outtextxy(750,140,num2);


	outtextxy(750,160,"���е�������");
	char num3[20];
	sprintf(num3, "%d", informationEnemyAttacked);
	outtextxy(750,180,num3);
}


/*
	�û����뼯��  
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

// ����
void OnUp()
{
	if (planeY <= 0) planeY = 0; else planeY -= 3;
}

// ����
void OnDown()
{
	if (planeY >= 630) planeY = 630; else planeY += 3;
}

// ����
void OnLeft()
{
	if (planeX <= 30) planeX = 30; else planeX -= 3;
}

// ����
void OnRight()
{
	if (planeX >= 648) planeX = 648; else planeX += 3;
}

void ToUpdate()
{
	plane.positionX = planeX;
	plane.positionY = planeY;


	//���µ���λ��
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



	//�����ӵ�λ��
	for(int i=0;i<10000;i++){
		if(bullet[i].is_active){
			if(bullet[i].position.y<0){bullet[i].is_active=0;}
			else{
			bullet[i].position.y = bullet[i].position.y - 8;
			}
			//��ײ���
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
	�ػ�����
*/
void drawBullet()
{
	for(int i=0;i<bulletLength;i++){
		if(bullet[i].is_active){   //������ӵ����ڼ���״̬
			setlinecolor(BLACK);
			setfillcolor(WHITE);
			fillrectangle(bullet[i].position.x,bullet[i].position.y,bullet[i].position.x+10,bullet[i].position.y+10); //�����ӵ�
		}
	}
}
void drawPlane()
{
	//����
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
	//�����
	fillrectangle(plane.positionX-10,40+plane.positionY,plane.positionX,50+plane.positionY);
	fillrectangle(plane.positionX-20,40+plane.positionY,plane.positionX-10,50+plane.positionY);
	fillrectangle(plane.positionX-20,50+plane.positionY,plane.positionX-10,60+plane.positionY);
	fillrectangle(plane.positionX-30,40+plane.positionY,plane.positionX-20,50+plane.positionY);
	fillrectangle(plane.positionX-40,30+plane.positionY,plane.positionX-30,40+plane.positionY);
	//�һ���
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
	
	outtextxy(750,80,"�л�����");
	char num[20];
	sprintf(num, "%d", informationEnemyLength);
	outtextxy(750,100,num);


	outtextxy(750,120,"�ӵ�����");
	char num2[20];
	sprintf(num2, "%d", informationBulletLength);
	outtextxy(750,140,num2);

	outtextxy(750,160,"���е�������");
	char num3[20];
	sprintf(num3, "%d", informationEnemyAttacked);
	outtextxy(750,180,num3);
	outtextxy(750, 580, "���ƣ�");
	outtextxy(750, 600, "��������");
	outtextxy(750, 620, "������ �ո�");
	outtextxy(750, 640, "��ͣ��ESC");
	
}



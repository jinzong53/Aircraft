#include<stdio.h>
#include"Tool.h"
#include <conio.h>
#include<stdlib.h>
//ÿ��time�����һ��1
int timeApperActive(int time)
{
    static int s = 0;
    int times = time/5;
    if(s == times)
    {
        s=0;
        return 1;
    }
    else
    {
        s++;
        return 0;
    }
}
//ÿ��time����һ�����ִ�1��ʼ�����ϼ�1
int timeApperNums(int time)
{
    static int s = 0;
    static int Nums = 0;
    int times = time/5;
    if(s == times)
    {
        Nums++;
        s=0;
        return Nums-1;
    }
    else
    {
        s++;
        return -1;
    }
}

//��ȡ�������
int getRandInt(int start, int end)
{
    int dis = end - start;
    return rand() % dis + start;
}




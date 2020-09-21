#include<stdio.h>
#include"Tool.h"
#include <conio.h>
#include<stdlib.h>
//每隔time秒出现一个1
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
//每隔time出现一个数字从1开始，不断加1
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

//获取随机数字
int getRandInt(int start, int end)
{
    int dis = end - start;
    return rand() % dis + start;
}




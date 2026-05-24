/*
 * main.c — 程序入口
 * 显示主菜单循环，接收用户输入并路由到对应的服务模块。
 */

#include <stdio.h>
#include <string.h>
#include "menu.h"

int main()
{
    printf("欢迎进入计费管理系统\n");

    // 从 data/ 目录加载卡片与账单数据到内存链表
    initializeSystem();

    int nChoice;
    do {
        outputMenu();

        // 读取菜单选择（0~8）
        char sInput[10];
        scanf("%9s", sInput);
        if (discard_rest_of_line()) {
            printf("输入失效！请输入阿拉伯数字0~8\n\n");
            continue;
        }
        printf("\n");

        if (strlen(sInput) > 1 || sInput[0] < '0' || sInput[0] > '8') {
            printf("输入失效！请输入阿拉伯数字0~8\n");
            continue;
        }

        nChoice = sInput[0] - '0';
        runService(nChoice);

    } while (nChoice);

    printf("已退出，欢迎下次使用！");

    return 0;
}
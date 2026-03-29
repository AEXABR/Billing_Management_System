#ifndef MENU_H
#define MENU_H
#include <stdio.h>
#include "service.h"

void outputMenu()
{
    printf(
        "==========菜单==========\n"
        "1.添加卡\n"
        "2.查询卡\n"
        "3.上机 \n"
        "4.下机 \n"
        "5.充值 \n"
        "6.退费 \n"
        "7.查询统计\n"
        "8.注销卡\n"
        "0.退出 \n"
        "请选择菜单项编号（0~8）："
    );
}

void runService(int choice)
{
    switch (choice) {
    case 1:
        card_add();
        printf("\n");
        break;
    case 2:
        card_query();
        printf("\n");
        break;
    case 3:
        computer_login();
        printf("\n");
        break;
    case 4:
        computer_logout();
        printf("\n");
        break;
    case 5:
        billing_rechange();
        printf("\n");
        break;
    case 6:
        billing_refund();
        printf("\n");
        break;
    case 7:
        billing_query();
        printf("\n");
        break;
    case 8:
        card_cancel();
        printf("\n");
        break;
    case 0:
        break;
    }
}

#endif

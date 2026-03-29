#ifndef SERVICE_H
#define SERVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card_service.h"
#include "billing_service.h"

void card_add()
{
    printf("----------添加卡---------\n");

    char name[19];
    while (1) {
        printf("请输入要添加的卡的卡号（长度为1到18）：");
        scanf("%s", name);
        if (strlen(name) > 18) {
            printf("无效输入，卡号过长\n");
            continue;
        }
        int found = 0;
        CARD* ptr = cardlist.head;
        while (ptr != NULL) {
            if (!strcmp(ptr->aName, name)) {
                found = 1;
                printf("无效输入，卡号已存在\n");
            }
            ptr = ptr->next;
        }
        if (found) continue;
        break;
    }

    char pwd[9];
    while (1) {
        printf("请输入要添加的卡的密码（长度为1到8）：");
        scanf("%s", pwd);
        if (strlen(pwd) > 8) {
            printf("无效输入，密码过长\n");
            continue;
        }
        break;
    }
    while (getchar() != '\n');

    int money;
    char c;
    while (1) {
        printf("请输入开卡金额（RMB）：");
        money = 0;
        int valid = 1;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c >= '0' && c <= '9')
                money = money * 10 + (c - '0');
            else valid = 0;
        }
        if (valid && money > 0) break;
        else printf("输入无效，请输入正整数！\n");
    }

    //添加 card 信息
    time_t curtime = time(0);
    CARD* newcard = (CARD*)malloc(sizeof(CARD));
    strcpy(newcard->aName, name);
    strcpy(newcard->aPwd, pwd);
    newcard->nStatus = 0;
    newcard->tStart = curtime;
    newcard->tEnd = curtime;
    newcard->fTotalUse = 0.0f;
    newcard->tLastTime = curtime;
    newcard->nUseCount = 0;
    newcard->fBalance = (float)money;
    newcard->nDel = 0;
    list_add(newcard);

    //将 card 信息写入文件
    FILE* fp = fopen("data/card.txt", "a");
    if (fp) {
        fprintf(fp, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n",
            newcard->aName,
            newcard->aPwd,
            newcard->nStatus,
            T_transform_S(newcard->tStart),
            T_transform_S(newcard->tEnd),
            newcard->fTotalUse,
            T_transform_S(newcard->tLastTime),
            newcard->nUseCount,
            newcard->fBalance,
            newcard->nDel);
        fclose(fp);
    }

    //将 card 信息写入二进制文件
    // fp = fopen("data/card.dat", "a");
    // if (fp) {
    //     fwrite(&card[cardcnt], sizeof(CARD), 1, fp);
    //     fclose(fp);
    // }

    printf("添加成功\n");
    printf("-----添加的卡信息如下-----\n");
    printf("卡号                密码        状态    开卡金额\n");
    printf("%-20s%-12s%-8d%-8.2f\n",
        newcard->aName,
        newcard->aPwd,
        newcard->nStatus,
        newcard->fBalance);
}

void card_query()
{
    printf("----------查询卡---------\n");

    printf("请输入查询的卡号（长度为1到18）：");
    char name[19];
    scanf("%s", name);
    if (strlen(name) > 18) {
        printf("无效输入，卡号过长\n");
        return;
    }

    int found = 0;
    CARD* ptr = cardlist.head;
    while (ptr != NULL) {
        if (!strncmp(ptr->aName, name, strlen(name))) {
            printf("卡号                状态  余额     累计使用  使用次数  上次使用时间\n");
            struct tm* ptm = localtime(&ptr->tLastTime);
            printf("%-18s  %-4d  %-8.1f %-9.1f %-9d %04d-%02d-%02d %02d:%02d:%02d\n",
                ptr->aName,
                ptr->nStatus,
                ptr->fBalance,
                ptr->fTotalUse,
                ptr->nUseCount,
                ptm->tm_year + 1900,
                ptm->tm_mon + 1,
                ptm->tm_mday,
                ptm->tm_hour,
                ptm->tm_min,
                ptm->tm_sec);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (!found) printf("无效输入，卡号不存在\n");
}

void card_cancel()
{
    printf("----------注销卡---------\n");
}

void billing_rechange()
{
    printf("----------充值----------\n");
}

void billing_refund()
{
    printf("----------退费----------\n");
}

void billing_query()
{
    printf("---------查询统计---------\n");
}

void computer_login()
{
    printf("----------上机----------\n");
}

void computer_logout()
{
    printf("----------下机----------\n");
}

#endif

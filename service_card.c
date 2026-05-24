#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_card.h"

void card_add()
{
    printf("----------添加卡---------\n");

    char sName[19];
    while (1) {
        printf("请输入要添加的卡的卡号（长度为1到18）：");
        scanf("%18s", sName);
        if (discard_rest_of_line()) {
            printf("无效输入，卡号过长\n\n");
            continue;
        }
        int nFound = 0;
        CARD* pCard = card_list.head;
        while (pCard != NULL) {
            if (!strcmp(pCard->sName, sName)) {
                nFound = 1;
                printf("无效输入，卡号已存在\n\n");
            }
            pCard = pCard->pNext;
        }
        if (nFound) continue;
        break;
    }

    char sPwd[9];
    while (1) {
        printf("请输入要添加的卡的密码（长度为1到8）：");
        scanf("%8s", sPwd);
        if (discard_rest_of_line()) {
            printf("无效输入，密码过长\n\n");
            continue;
        }
        break;
    }

    int nMoney;
    char sMoneyInput[32];
    while (1) {
        printf("请输入开卡金额（RMB）：");
        scanf("%31s", sMoneyInput);
        if (discard_rest_of_line()) {
            printf("输入无效，请输入正整数！\n\n");
            continue;
        }

        nMoney = 0;
        int nValid = 1;
        for (int nIndex = 0; sMoneyInput[nIndex] != '\0'; nIndex++) {
            if (sMoneyInput[nIndex] >= '0' && sMoneyInput[nIndex] <= '9')
                nMoney = nMoney * 10 + (sMoneyInput[nIndex] ^ '0');
            else {
                nValid = 0;
                break;
            }
        }
        if (nValid && nMoney > 0) break;
        else printf("输入无效，请输入正整数！\n\n");
    }

    time_t ttCurTime = time(0);
    CARD* pNewCard = (CARD*)malloc(sizeof(CARD));
    if (!pNewCard) {
        printf("添加失败，内存不足\n");
        return;
    }
    strcpy(pNewCard->sName, sName);
    strcpy(pNewCard->sPwd, sPwd);
    pNewCard->nStatus = 0;
    pNewCard->ttStart = ttCurTime;
    pNewCard->ttEnd = ttCurTime;
    pNewCard->fTotalUse = 0.0f;
    pNewCard->ttLastTime = ttCurTime;
    pNewCard->nUseCount = 0;
    pNewCard->fBalance = (float)nMoney;
    pNewCard->nDel = 0;
    list_addcard(pNewCard);

    if (!save_card_list_to_file())
        printf("警告：卡信息写入文件失败\n");

    printf("-------添加的卡信息如下-------\n");
    printf("%-22s  %-14s  %-10s  %-12s\n",
        "卡号",
        "密码",
        "状态",
        "开卡金额");
    printf("%-20s  %-12s  %-8d  %-8.1f\n",
        pNewCard->sName,
        pNewCard->sPwd,
        pNewCard->nStatus,
        pNewCard->fBalance);
}

void card_cancel()
{
    printf("----------注销卡---------\n");

    char sName[19], sPwd[9];
    printf("请输入注销卡号（长度为1到18）：");
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("注销失败，卡号过长\n");
        return;
    }
    printf("请输入注销密码（长度为1到8）：");
    scanf("%8s", sPwd);
    if (discard_rest_of_line()) {
        printf("注销失败，密码过长\n");
        return;
    }

    int nFound = 0;
    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        if (!strcmp(pCard->sName, sName)) {
            nFound = 1;

            if (strcmp(pCard->sPwd, sPwd)) {
                printf("注销失败，密码错误\n");
                return;
            }

            if (pCard->nStatus == 1) {
                printf("注销失败，卡正在上机\n");
                return;
            }

            if (pCard->nStatus == 2) {
                printf("注销失败，卡已注销\n");
                return;
            }

            float fRefundMoney = pCard->fBalance;
            pCard->nStatus = 2;
            pCard->fBalance = 0.0f;

            if (!save_card_list_to_file()) {
                printf("警告：卡信息写入文件失败\n");
            }

            printf("--------注销信息如下-------\n");
            printf("%-20s  %-12s\n",
                "卡号",
                "退费金额");
            printf("%-18s  %-8.1f\n",
                pCard->sName,
                fRefundMoney);
        }
    }
    if (!nFound) printf("无效输入，卡号不存在\n");
}
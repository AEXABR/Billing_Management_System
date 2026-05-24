#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_computer.h"

void computer_login()
{
    printf("----------上机----------\n");

    char sName[19], sPwd[9];
    printf("请输入上机卡号（长度为1到18）：");
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("上机失败，卡号过长\n");
        return;
    }
    printf("请输入上机密码（长度为1到8）：");
    scanf("%8s", sPwd);
    if (discard_rest_of_line()) {
        printf("上机失败，密码过长\n");
        return;
    }

    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        if (!strcmp(pCard->sName, sName)) {
            if (strcmp(pCard->sPwd, sPwd)) {
                printf("上机失败，密码错误\n");
                return;
            }

            if (pCard->nStatus == 1) {
                printf("上机失败，卡正在运行\n");
                return;
            }
            if (pCard->nStatus == 2) {
                printf("上机失败，卡已注销\n");
                return;
            }

            if (pCard->fBalance <= 0.0f) {
                printf("上机失败，余额不足\n");
                return;
            }

            pCard->nStatus = 1;
            pCard->ttLastTime = time(0);
            pCard->nUseCount++;

            BILLING* pNewBilling = (BILLING*)malloc(sizeof(BILLING));
            if (!pNewBilling) {
                printf("上机失败，内存不足\n");
                return;
            }
            strcpy(pNewBilling->sName, pCard->sName);
            pNewBilling->ttStart = time(0);
            pNewBilling->ttEnd = 0;
            pNewBilling->fBalance = pCard->fBalance;
            pNewBilling->fAmount = 0.0f;
            pNewBilling->nStatus = 0;
            list_addbilling(pNewBilling);

            if (!save_card_list_to_file() || !save_billing_list_to_file())
                printf("警告：信息写入文件失败\n");

            printf("上机成功\n");
            printf("------上机信息如下------\n");
            printf("%-20s  %-10s  %-23s\n",
                "卡号",
                "余额",
                "上机时间");
            printf("%-18s  %-8.1f  %s\n",
                pCard->sName,
                pCard->fBalance,
                TT_transform_S(time(0)));

            return;
        }
    }
    printf("上机失败，卡号不存在\n");
}

void computer_logout()
{
    printf("----------下机----------\n");

    char sName[19], sPwd[9];
    printf("请输入下机卡号（长度为1到18）：");
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("下机失败，卡号过长\n");
        return;
    }
    printf("请输入下机密码（长度为1到8）：");
    scanf("%8s", sPwd);
    if (discard_rest_of_line()) {
        printf("下机失败，密码过长\n");
        return;
    }

    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        if (!strcmp(pCard->sName, sName)) {
            if (strcmp(pCard->sPwd, sPwd)) {
                printf("下机失败，密码错误\n");
                return;
            }

            if (pCard->nStatus == 0) {
                printf("下机失败，卡未上机\n");
                return;
            }
            if (pCard->nStatus == 2) {
                printf("下机失败，卡已注销\n");
                return;
            }

            time_t ttCurTime = time(0);
            float fUsedAmount = (float)(ttCurTime - pCard->ttLastTime) / 3600.0f * 10.0f;
            if (fUsedAmount > pCard->fBalance) {
                printf("下机失败，余额不足\n");
                return;
            }

            pCard->nStatus = 0;
            pCard->fBalance -= fUsedAmount;
            pCard->fTotalUse += fUsedAmount;

            for (BILLING* pBilling = billing_list.head; pBilling != NULL; pBilling = pBilling->pNext) {
                if (!strcmp(pBilling->sName, sName) && pBilling->nStatus == 0) {
                    pBilling->ttEnd = ttCurTime;
                    pBilling->fAmount = fUsedAmount;
                    pBilling->nStatus = 1;
                    break;
                }
            }

            if (!save_card_list_to_file() || !save_billing_list_to_file()) {
                printf("警告：信息写入文件失败\n");
            }

            printf("下机成功\n");
            printf("------下机信息如下------\n");
            char sLoginTime[20];
            char sLogoutTime[20];
            strcpy(sLoginTime, TT_transform_S(pCard->ttLastTime));
            strcpy(sLogoutTime, TT_transform_S(ttCurTime));
            printf("%-20s  %-12s  %-10s  %-23s  %-23s\n",
                "卡号",
                "消费金额",
                "余额",
                "上机时间",
                "下机时间");
            printf("%-18s  %-8.1f  %-8.1f  %-19s  %-19s\n",
                pCard->sName,
                fUsedAmount,
                pCard->fBalance,
                sLoginTime,
                sLogoutTime);

            return;
        }
    }

    printf("下机失败，卡号不存在\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_billing.h"

void billing_rechange()
{
    printf("----------充值----------\n");

    char sName[19], sPwd[9];
    printf("请输入充值卡号（长度为1到18）：");
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("充值失败，卡号过长\n");
        return;
    }
    printf("请输入充值密码（长度为1到8）：");
    scanf("%8s", sPwd);
    if (discard_rest_of_line()) {
        printf("充值失败，密码过长\n");
        return;
    }

    int nMoney;
    char sMoneyInput[32];
    while (1) {
        printf("请输入充值金额（RMB）：");
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

    int nFound = 0;
    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        if (!strcmp(pCard->sName, sName)) {
            nFound = 1;

            if (strcmp(pCard->sPwd, sPwd)) {
                printf("充值失败，密码错误\n");
                return;
            }

            if (pCard->nStatus == CANCELLED) {
                printf("充值失败，卡已注销\n");
                return;
            }

            pCard->fBalance += (float)nMoney;

            if (!save_card_list_to_file()) {
                printf("警告：卡信息写入文件失败\n");
            }

            printf("--------充值信息如下-------\n");
            printf("%-20s  %-12s  %-10s\n",
                "卡号",
                "充值金额",
                "余额");
            printf("%-18s  %-8.1f  %-8.1f\n",
                pCard->sName,
                (float)nMoney,
                pCard->fBalance);
        }
    }
    if (!nFound) printf("无效输入，卡号不存在\n");
}

void billing_refund()
{
    printf("----------退费----------\n");

    char sName[19], sPwd[9];
    printf("请输入退费卡号（长度为1到18）：");
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("退费失败，卡号过长\n");
        return;
    }
    printf("请输入退费密码（长度为1到8）：");
    scanf("%8s", sPwd);
    if (discard_rest_of_line()) {
        printf("退费失败，密码过长\n");
        return;
    }

    int nFound = 0;
    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        if (!strcmp(pCard->sName, sName)) {
            nFound = 1;

            if (strcmp(pCard->sPwd, sPwd)) {
                printf("退费失败，密码错误\n");
                return;
            }

            if (pCard->nStatus == ACTIVE) {
                printf("退费失败，卡正在上机\n");
                return;
            }

            if (pCard->nStatus == CANCELLED) {
                printf("退费失败，卡已注销\n");
                return;
            }

            if (pCard->fBalance <= 0.0f) {
                printf("退费失败，卡余额不足\n");
                return;
            }

            float fRefundAmount = pCard->fBalance;
            pCard->fBalance = 0.0f;

            if (!save_card_list_to_file()) {
                printf("警告：卡信息写入文件失败\n");
            }

            printf("--------退费信息如下-------\n");
            printf("%-20s  %-12s  %-10s\n",
                "卡号",
                "退费金额",
                "余额");
            printf("%-18s  %-8.1f  %-8.1f\n",
                pCard->sName,
                fRefundAmount,
                pCard->fBalance);
        }
    }
    if (!nFound) printf("无效输入，卡号不存在\n");
}
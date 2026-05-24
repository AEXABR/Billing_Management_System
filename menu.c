#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

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

void runService(int nChoice)
{
    switch (nChoice) {
    case 1:
        card_add();
        printf("\n");
        break;
    case 2:
        query_card();
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
        query_billing();
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

void initializeSystem()
{
    FILE* pFile;
    char sStart[20], sEnd[20], sLastTime[20];

    pFile = fopen("data/card.txt", "r");
    if (!pFile) return;

    while (1) {
        CARD* pCard = (CARD*)malloc(sizeof(CARD));
        if (!pCard) break;

        if (fscanf(pFile, "%18[^#]##%8[^#]##%d##%19[^#]##%19[^#]##%f##%19[^#]##%d##%f##%d\n",
            pCard->sName,
            pCard->sPwd,
            &pCard->nStatus,
            sStart,
            sEnd,
            &pCard->fTotalUse,
            sLastTime,
            &pCard->nUseCount,
            &pCard->fBalance,
            &pCard->nDel) != 10) {
            free(pCard);
            break;
        }
        pCard->ttStart = S_transform_TT(sStart);
        pCard->ttEnd = S_transform_TT(sEnd);
        pCard->ttLastTime = S_transform_TT(sLastTime);
        list_addcard(pCard);
    }
    fclose(pFile);

    pFile = fopen("data/billing.txt", "r");
    if (!pFile) return;

    while (1) {
        BILLING* pBilling = (BILLING*)malloc(sizeof(BILLING));
        if (!pBilling) break;

        if (fscanf(pFile, "%18[^#]##%19[^#]##%19[^#]##%f##%f##%d\n",
            pBilling->sName,
            sStart,
            sEnd,
            &pBilling->fBalance,
            &pBilling->fAmount,
            &pBilling->nStatus) != 6) {
            free(pBilling);
            break;
        }
        pBilling->ttStart = S_transform_TT(sStart);
        pBilling->ttEnd = S_transform_TT(sEnd);
        list_addbilling(pBilling);
    }
    fclose(pFile);
}
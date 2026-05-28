/*
 * menu.c — 菜单显示与路由
 * outputMenu() 打印主菜单，runService() 根据选项分发到各业务函数。
 * initializeSystem() 在启动时从 data/ 目录恢复卡片和账单链表。
 */

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
    case 1: card_add();           break;
    case 2: query_card();         break;
    case 3: computer_login();     break;
    case 4: computer_logout();    break;
    case 5: billing_rechange();   break;
    case 6: billing_refund();     break;
    case 7: query_billing();      break;  // 扩展功能
    case 8: card_cancel();        break;
    case 0:                       break;  // 退出
    }
    if (nChoice != 0) printf("\n");
}

// 启动时从文件恢复卡片链表和账单链表
void initializeSystem()
{
    FILE* pFile;
    char sStart[20], sEnd[20], sLastTime[20];

    // 读取卡片数据（## 分隔的文本格式）
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
        // 时间字符串 → time_t
        pCard->ttStart = S_transform_TT(sStart);
        pCard->ttEnd = S_transform_TT(sEnd);
        pCard->ttLastTime = S_transform_TT(sLastTime);
        list_addcard(pCard);
    }
    fclose(pFile);

    // 读取账单数据
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

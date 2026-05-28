/*
 * service_billing.c — 充值/退费实现
 * billing_rechange() 充值 — 增加卡内余额
 * billing_refund()   退费 — 退还全部余额（不清零则退）
 */

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

    // 输入充值金额（正整数校验）
    int nMoney;
    input_positive_int("请输入充值金额（RMB）：", &nMoney);

    CARD* pCard = find_card_by_name(sName);
    if (!pCard) {
        printf("无效输入，卡号不存在\n");
        return;
    }

    if (strcmp(pCard->sPwd, sPwd)) {
        printf("充值失败，密码错误\n");
        return;
    }
    // 已注销卡不能充值
    if (pCard->nStatus == CANCELLED) {
        printf("充值失败，卡已注销\n");
        return;
    }

    pCard->fBalance += (float)nMoney;

    if (!save_card_list_to_file())
        printf("警告：卡信息写入文件失败\n");

    printf("--------充值信息如下-------\n");
    printf("%-20s  %-12s  %-10s\n", "卡号", "充值金额", "余额");
    printf("%-18s  %-8.1f  %-8.1f\n", pCard->sName, (float)nMoney, pCard->fBalance);
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

    CARD* pCard = find_card_by_name(sName);
    if (!pCard) {
        printf("无效输入，卡号不存在\n");
        return;
    }

    if (strcmp(pCard->sPwd, sPwd)) {
        printf("退费失败，密码错误\n");
        return;
    }

    // 正在上机或已注销的卡不能退费
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

    if (!save_card_list_to_file())
        printf("警告：卡信息写入文件失败\n");

    printf("--------退费信息如下-------\n");
    printf("%-20s  %-12s  %-10s\n", "卡号", "退费金额", "余额");
    printf("%-18s  %-8.1f  %-8.1f\n", pCard->sName, fRefundAmount, pCard->fBalance);
}

/*
 * service_tool.c — 工具函数实现
 * 链表操作、输入校验、时间转换、文件持久化。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "service_tool.h"

// 全局链表定义
CARD_LIST card_list = { NULL, NULL };
BILLING_LIST billing_list = { NULL, NULL };

// 卡片链表 — 尾插法
void list_addcard(CARD* pCard)
{
    pCard->pNext = NULL;
    if (card_list.tail)
        card_list.tail = card_list.tail->pNext = pCard;
    else
        card_list.tail = card_list.head = pCard;
}

// 账单链表 — 头插法（最新记录在前，便于查找未结算记录）
void list_addbilling(BILLING* pBilling)
{
    pBilling->pNext = NULL;
    if (billing_list.head)
        pBilling->pNext = billing_list.head, billing_list.head = pBilling;
    else
        billing_list.head = billing_list.tail = pBilling;
}

// 丢弃 stdin 当前行剩余字符，返回 1 表示用户输入超出缓冲区（输入过长）
int discard_rest_of_line()
{
    int nChar = getchar();
    if (nChar == '\n' || nChar == EOF) return 0;
    while ((nChar = getchar()) != '\n' && nChar != EOF) {}
    return 1;
}

// 字符串时间 "YYYY-MM-DD HH:MM:SS" → time_t
time_t S_transform_TT(char* sTime)
{
    struct tm tmTime = { 0 };
    if (sscanf(sTime, "%d-%d-%d %d:%d:%d",
        &tmTime.tm_year,
        &tmTime.tm_mon,
        &tmTime.tm_mday,
        &tmTime.tm_hour,
        &tmTime.tm_min,
        &tmTime.tm_sec) != 6) {
        return (time_t)0;
    }
    tmTime.tm_year -= 1900;
    tmTime.tm_mon -= 1;
    tmTime.tm_isdst = -1;
    return mktime(&tmTime);
}

// time_t → 字符串 "YYYY-MM-DD HH:MM:SS"（返回静态缓冲区，非线程安全）
char* TT_transform_S(time_t ttTime)
{
    struct tm* tmCurrent = localtime(&ttTime);
    static char sTime[20];
    sprintf(sTime, "%04d-%02d-%02d %02d:%02d:%02d",
        tmCurrent->tm_year + 1900,
        tmCurrent->tm_mon + 1,
        tmCurrent->tm_mday,
        tmCurrent->tm_hour,
        tmCurrent->tm_min,
        tmCurrent->tm_sec);
    return sTime;
}

// 持久化：遍历 card_list 写入 data/card.txt 和 data/card.dat
int save_card_list_to_file()
{
    FILE* pFileTxt = fopen("data/card.txt", "w");
    FILE* pFileDat = fopen("data/card.dat", "wb");
    if (!pFileTxt || !pFileDat) {
        if (pFileTxt) fclose(pFileTxt);
        if (pFileDat) fclose(pFileDat);
        return 0;
    }

    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        char sStartTime[20];
        char sEndTime[20];
        char sLastTime[20];
        strcpy(sStartTime, TT_transform_S(pCard->ttStart));
        strcpy(sEndTime, TT_transform_S(pCard->ttEnd));
        strcpy(sLastTime, TT_transform_S(pCard->ttLastTime));
        fprintf(pFileTxt, "%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n",
            pCard->sName,
            pCard->sPwd,
            pCard->nStatus,
            sStartTime,
            sEndTime,
            pCard->fTotalUse,
            sLastTime,
            pCard->nUseCount,
            pCard->fBalance,
            pCard->nDel);
        fwrite(pCard, sizeof(CARD), 1, pFileDat);
    }

    fclose(pFileTxt);
    fclose(pFileDat);
    return 1;
}

// 持久化：遍历 billing_list 写入 data/billing.txt 和 data/billing.dat
int save_billing_list_to_file()
{
    FILE* pFileTxt = fopen("data/billing.txt", "w");
    FILE* pFileDat = fopen("data/billing.dat", "wb");
    if (!pFileTxt || !pFileDat) {
        if (pFileTxt) fclose(pFileTxt);
        if (pFileDat) fclose(pFileDat);
        return 0;
    }

    for (BILLING* pBilling = billing_list.head; pBilling != NULL; pBilling = pBilling->pNext) {
        char sStartTime[20];
        char sEndTime[20];
        strcpy(sStartTime, TT_transform_S(pBilling->ttStart));
        strcpy(sEndTime, TT_transform_S(pBilling->ttEnd));
        fprintf(pFileTxt, "%s##%s##%s##%.1f##%.1f##%d\n",
            pBilling->sName,
            sStartTime,
            sEndTime,
            pBilling->fBalance,
            pBilling->fAmount,
            pBilling->nStatus);
        fwrite(pBilling, sizeof(BILLING), 1, pFileDat);
    }

    fclose(pFileTxt);
    fclose(pFileDat);
    return 1;
}

// 字符串转整数：逐字符校验数字并累加，成功返回 1
int str_to_int(const char* s, int* out)
{
    *out = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9')
            return 0;
        *out = *out * 10 + (s[i] ^ '0');
    }
    return 1;
}

int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_of_month(int year, int month)
{
    if (month == 2)
        return is_leap_year(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

// 交互式输入正整数（循环至合法输入为止）
int input_positive_int(const char* prompt, int* out)
{
    char sInput[32];
    while (1) {
        printf("%s", prompt);
        scanf("%31s", sInput);
        if (discard_rest_of_line()) {
            printf("输入无效，请输入正整数！\n\n");
            continue;
        }
        if (str_to_int(sInput, out) && *out > 0)
            return 1;
        printf("输入无效，请输入正整数！\n\n");
    }
}

// 按卡号精确查找卡片，返回指针或 NULL
CARD* find_card_by_name(const char* sName)
{
    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext)
        if (!strcmp(pCard->sName, sName))
            return pCard;
    return NULL;
}

// 交互式输入日期（年/月/日），含范围校验和闰年判断
void input_time(const char* sOutput, struct tm* tmInput)
{
    time_t ttCurTime = time(0);
    struct tm tmNow = *localtime(&ttCurTime);

    char sDateInput[5];
    int nYear, nMonth, nDay, nMaxDay;

    printf("%s\n", sOutput);

    // 输入年份
    while (1) {
        printf("年：");
        scanf("%5s", sDateInput);
        if (discard_rest_of_line() || !str_to_int(sDateInput, &nYear)
            || nYear < 1900 || nYear > tmNow.tm_year + 1900) {
            printf("输入无效，请输入1900~%d之间的年份\n\n", tmNow.tm_year + 1900);
            continue;
        }
        break;
    }

    // 输入月份
    while (1) {
        printf("月：");
        scanf("%3s", sDateInput);
        if (discard_rest_of_line() || !str_to_int(sDateInput, &nMonth)
            || nMonth < 1 || nMonth > 12) {
            printf("输入无效，请输入1~12之间的月份\n\n");
            continue;
        }
        break;
    }

    nMaxDay = days_of_month(nYear, nMonth);

    // 输入日期
    while (1) {
        printf("日：");
        scanf("%3s", sDateInput);
        if (discard_rest_of_line() || !str_to_int(sDateInput, &nDay)
            || nDay < 1 || nDay > nMaxDay) {
            printf("输入无效，请输入1~%d之间的日期\n\n", nMaxDay);
            continue;
        }
        break;
    }

    tmInput->tm_year = nYear - 1900;
    tmInput->tm_mon = nMonth - 1;
    tmInput->tm_mday = nDay;
    tmInput->tm_isdst = -1;
}
/*
 * service_query.c — 查询统计实现
 *
 * query_card()    查询卡信息（支持模糊匹配）
 *
 * —— 以下为课程扩展功能，仅需完成课程基本要求可不实现 ——
 * query_billing() 查询统计入口（3 个子功能）
 *   choice_one()   按卡号 + 时间范围查询消费明细
 *   choice_two()   按时间范围统计总营业额
 *   choice_three() 按年份统计每月营业额
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_query.h"

// [扩展功能] 查询单卡在指定时间段内的消费明细
void choice_one()
{
    printf("----------查询单卡消费--------\n");

    char sName[19];
    printf("请输入查询卡号（长度为1到18）：");
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("查询失败，卡号过长\n");
        return;
    }

    struct tm tmStart = { 0 };
    struct tm tmEnd = { 0 };
    input_time("请输入查询起始日期：", &tmStart);
    input_time("请输入查询截止日期：", &tmEnd);

    time_t ttStartTime = mktime(&tmStart);
    time_t ttEndTime = mktime(&tmEnd);

    int nFound = 0;
    int nCnt = 0;
    for (BILLING* pBilling = billing_list.head; pBilling != NULL; pBilling = pBilling->pNext) {
        if (!strcmp(pBilling->sName, sName)) {
            nFound = 1;
            if (pBilling->ttStart >= ttStartTime && pBilling->ttStart <= ttEndTime) {
                ++nCnt;
                char sLoginTime[20], sLogoutTime[20];
                strcpy(sLoginTime, TT_transform_S(pBilling->ttStart));
                strcpy(sLogoutTime, TT_transform_S(pBilling->ttEnd));
                printf("%-20s  %-12s  %-10s  %-23s  %-23s\n",
                    "卡号", "消费金额", "余额", "上机时间", "下机时间");
                printf("%-18s  %-8.1f  %-8.1f  %-19s  %-19s\n",
                    pBilling->sName, pBilling->fAmount, pBilling->fBalance,
                    sLoginTime, sLogoutTime);
            }
        }
    }
    if (!nFound) printf("查询失败，卡号不存在\n");
    printf("共查询到 %d 条记录\n", nCnt);
}

// [扩展功能] 统计指定时间段内的总营业额
void choice_two()
{
    printf("----------统计总营业额--------\n");

    struct tm tmStart = { 0 };
    struct tm tmEnd = { 0 };
    input_time("请输入查询起始日期：", &tmStart);
    input_time("请输入查询截止日期：", &tmEnd);

    time_t ttStartTime = mktime(&tmStart);
    time_t ttEndTime = mktime(&tmEnd);

    float fTotalAmount = 0.0f;
    for (BILLING* pBilling = billing_list.head; pBilling != NULL; pBilling = pBilling->pNext) {
        if (pBilling->ttStart >= ttStartTime && pBilling->ttStart <= ttEndTime)
            fTotalAmount += pBilling->fAmount;
    }
    printf("总营业额：%.1f\n", fTotalAmount);
}

// [扩展功能] 统计某一年各月份的营业额
void choice_three()
{
    printf("----------统计月营业额--------\n");

    time_t ttCurTime = time(0);
    struct tm tmNow = *localtime(&ttCurTime);
    int nCurrentYear = tmNow.tm_year + 1900;

    char sYearInput[5];
    printf("请输入要统计的年份（4位阿拉伯数字）：");
    scanf("%5s", sYearInput);
    if (discard_rest_of_line()) {
        printf("输入无效，请输入1900~%d之间的年份\n\n", nCurrentYear);
        return;
    }

    int nYear;
    if (!str_to_int(sYearInput, &nYear) || nYear < 1900 || nYear > nCurrentYear) {
        printf("输入无效，请输入1900~%d之间的年份\n\n", nCurrentYear);
        return;
    }

    // 构造该年的起止时间戳
    struct tm tmYearStart = { 0 };
    tmYearStart.tm_year = nYear - 1900;
    tmYearStart.tm_mon = 0;
    tmYearStart.tm_mday = 1;
    tmYearStart.tm_hour = 0;
    tmYearStart.tm_min = 0;
    tmYearStart.tm_sec = 0;
    tmYearStart.tm_isdst = -1;
    time_t ttStartTime = mktime(&tmYearStart);

    struct tm tmNextYearStart = tmYearStart;
    tmNextYearStart.tm_year += 1;
    time_t ttEndTime = mktime(&tmNextYearStart) - 1;

    // 按月累加消费金额
    float fMonths[12] = { 0 };
    for (BILLING* pBilling = billing_list.head; pBilling != NULL; pBilling = pBilling->pNext) {
        if (pBilling->ttStart >= ttStartTime && pBilling->ttStart <= ttEndTime)
            fMonths[localtime(&pBilling->ttStart)->tm_mon] += pBilling->fAmount;
    }

    printf("%d 年每月营业额：\n", nYear);
    printf("%-6s  %-10s\n", "月份", "营业额");
    for (int i = 0; i < 12; i++)
        printf("%02d月     %-8.1f\n", i + 1, fMonths[i]);
}

// 查询卡信息（支持模糊搜索）
void query_card()
{
    printf("----------查询卡---------\n");

    printf("请输入查询的卡号（长度为1到18）：");
    char sName[19];
    scanf("%18s", sName);
    if (discard_rest_of_line()) {
        printf("无效输入，卡号过长\n");
        return;
    }

    int nFound = 0;
    for (CARD* pCard = card_list.head; pCard != NULL; pCard = pCard->pNext) {
        if (strstr(pCard->sName, sName) != NULL) {
            printf("%-20s  %-6s  %-10s  %-13s  %-8s  %-25s\n",
                "卡号", "状态", "余额", "累计消费", "使用次数", "上次使用时间");
            printf("%-18s  %-4d  %-8.1f  %-9.1f  %-8d  %-19s\n",
                pCard->sName, pCard->nStatus, pCard->fBalance,
                pCard->fTotalUse, pCard->nUseCount, TT_transform_S(pCard->ttLastTime));
            nFound = 1;
        }
    }
    if (!nFound) printf("无效输入，卡号不存在\n");
}

// [扩展功能] 查询统计入口
void query_billing()
{
    printf("--------查询统计--------\n");

    printf("查询类型：\n");
    printf("1. 查询单卡消费\n");
    printf("2. 统计总营业额\n");
    printf("3. 统计月营业额\n");
    printf("请输入查询类型（1~3）：");

    char sInput[2];
    scanf("%2s", sInput);
    if (discard_rest_of_line()) {
        printf("输入失效！请输入阿拉伯数字1~3\n\n");
        return;
    }
    printf("\n");

    if (strlen(sInput) > 1 || sInput[0] < '1' || sInput[0] > '3') {
        printf("输入失效！请输入阿拉伯数字1~3\n");
        return;
    }

    switch (sInput[0] - '0') {
    case 1: choice_one();   break;
    case 2: choice_two();   break;
    case 3: choice_three(); break;
    }
}

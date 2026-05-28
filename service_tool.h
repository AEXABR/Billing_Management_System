/*
 * service_tool.h — 核心数据结构与工具函数声明
 *
 * 定义两种核心数据模型：
 *   CARD   — 上网卡（卡号、密码、状态、余额、消费统计）
 *   BILLING — 账单记录（对应每次上/下机的消费明细）
 *
 * 均使用单向链表管理，card_list 尾插，billing_list 头插。
 */

#ifndef SERVICE_TOOL_H
#define SERVICE_TOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 卡片状态枚举
enum STATUS {
    ACTIVE = 0,    // 正在上机
    INACTIVE = 1,  // 未上机
    CANCELLED = 2  // 已注销
};

// 上网卡
typedef struct CARD {
    char sName[19];        // 卡号（最大18字符）
    char sPwd[9];          // 密码（最大8字符）
    int nStatus;           // 状态：0=上机 1=未上机 2=注销
    time_t ttStart;        // 开卡时间
    time_t ttEnd;          // （保留字段）
    float fTotalUse;       // 累计消费金额
    time_t ttLastTime;     // 上次使用时间
    int nUseCount;         // 使用次数
    float fBalance;        // 余额
    int nDel;              // 删除标记
    struct CARD* pNext;    // 链表指针
} CARD;

// 卡片链表（尾插法，保持插入顺序）
typedef struct {
    CARD* head;
    CARD* tail;
} CARD_LIST;

// 账单记录
typedef struct BILLING {
    char sName[19];        // 卡号
    time_t ttStart;        // 上机时间
    time_t ttEnd;          // 下机时间
    float fBalance;        // 上机前余额
    float fAmount;         // 本次消费金额
    int nStatus;           // 结算状态
    struct BILLING* pNext;
} BILLING;

// 账单链表（头插法，最新记录在前）
typedef struct {
    BILLING* head;
    BILLING* tail;
} BILLING_LIST;

// 全局链表（定义在 service_tool.c）
extern CARD_LIST card_list;
extern BILLING_LIST billing_list;

// —— 链表操作 ——
void list_addcard(CARD* pCard);
void list_addbilling(BILLING* pBilling);

// —— 输入工具 ——
int discard_rest_of_line();                       // 丢弃当前行剩余字符，返回 1 表示输入过长
void input_time(const char* sOutput, struct tm* tmInput);  // 交互式输入日期

// —— 时间转换 ——
time_t S_transform_TT(char* s);                   // 字符串 → time_t
char* TT_transform_S(time_t ttTime);              // time_t → 字符串（静态缓冲区）

// —— 持久化 ——
int save_card_list_to_file();
int save_billing_list_to_file();

// —— 数据校验与查找 ——
int str_to_int(const char *s, int *out);           // 字符串转整数，成功返回 1
int is_leap_year(int year);                        // 判断闰年
int days_of_month(int year, int month);            // 返回指定年月的天数
int input_positive_int(const char *prompt, int *out);  // 交互式输入正整数
CARD* find_card_by_name(const char *sName);        // 按卡号查找卡片

#endif // SERVICE_TOOL_H

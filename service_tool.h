#ifndef SERVICE_TOOL_H
#define SERVICE_TOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct CARD {
    char sName[19];
    char sPwd[9];
    int nStatus;
    time_t ttStart;
    time_t ttEnd;
    float fTotalUse;
    time_t ttLastTime;
    int nUseCount;
    float fBalance;
    int nDel;
    struct CARD* pNext;
} CARD;

typedef struct {
    CARD* head;
    CARD* tail;
} CARD_LIST;

typedef struct BILLING {
    char sName[19];
    time_t ttStart;
    time_t ttEnd;
    float fBalance;
    float fAmount;
    int nStatus;
    struct BILLING* pNext;
} BILLING;

typedef struct {
    BILLING* head;
    BILLING* tail;
} BILLING_LIST;

extern CARD_LIST card_list;
extern BILLING_LIST billing_list;

void list_addcard(CARD* pCard);
void list_addbilling(BILLING* pBilling);
int discard_rest_of_line();
time_t S_transform_TT(char* s);
char* TT_transform_S(time_t ttTime);
int save_card_list_to_file();
int save_billing_list_to_file();
void input_time(const char* sOutput, struct tm* tmInput);

#endif // SERVICE_TOOL_H
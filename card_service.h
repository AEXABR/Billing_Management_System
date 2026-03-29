#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct CARD {
    char aName[19];
    char aPwd[9];
    int nStatus;
    time_t tStart;
    time_t tEnd;
    float fTotalUse;
    time_t tLastTime;
    int nUseCount;
    float fBalance;
    int nDel;
    struct CARD* next;
}CARD;

typedef struct {
    CARD* head;
    CARD* tail;
}LIST;

LIST cardlist = { NULL,NULL };

time_t S_transform_T(char* s)
{
    struct tm tm;
    sscanf(s, "%d-%d-%d %d:%d:%d",
        &tm.tm_year,
        &tm.tm_mon,
        &tm.tm_mday,
        &tm.tm_hour,
        &tm.tm_min,
        &tm.tm_sec);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    return mktime(&tm);
}

char* T_transform_S(time_t t)
{
    struct tm* ptm = localtime(&t);
    static char s[20];
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
        ptm->tm_year + 1900,
        ptm->tm_mon + 1,
        ptm->tm_mday,
        ptm->tm_hour,
        ptm->tm_min,
        ptm->tm_sec);
    return s;
}

void list_add(CARD* pos)
{
    pos->next = NULL;
    if (cardlist.tail)
        cardlist.tail = cardlist.tail->next = pos;
    else cardlist.tail = cardlist.head = pos;
}

void initializeSystem()
{
    FILE* fp = fopen("data/card.txt", "r");
    char sStart[20], sEnd[20], sLastTime[20];
    while (1) {
        CARD* card = (CARD*)malloc(sizeof(CARD));
        if (fscanf(fp, "%[^#]##%[^#]##%d##%[^#]##%[^#]##%f##%[^#]##%d##%f##%d\n",
            card->aName,
            card->aPwd,
            &card->nStatus,
            sStart,
            sEnd,
            &card->fTotalUse,
            sLastTime,
            &card->nUseCount,
            &card->fBalance,
            &card->nDel) != 10) {
            free(card);
            break;
        }
        card->tStart = S_transform_T(sStart);
        card->tEnd = S_transform_T(sEnd);
        card->tLastTime = S_transform_T(sLastTime);
        list_add(card);
    }
    fclose(fp);

    /*
    FILE* fp = fopen("d:/VSCODE.text/#Billing_Management_System/data/card.dat", "r");
    if (fp) {
    = 0;
        while (fread(&card, sizeof(CARD), 1, fp) == 1) {
            +
        }
        fclose(fp);
    }
    */
}

#endif

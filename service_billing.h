/*
 * service_billing.h — 充值/退费模块
 */

#ifndef SERVICE_BILLING_H
#define SERVICE_BILLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_tool.h"

void billing_rechange(); // 充值
void billing_refund();   // 退费

#endif // SERVICE_BILLING_H

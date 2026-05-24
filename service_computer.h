/*
 * service_computer.h — 上机/下机模块
 */

#ifndef SERVICE_COMPUTER_H
#define SERVICE_COMPUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_tool.h"

void computer_login();   // 上机（开始计费）
void computer_logout();  // 下机（结算费用）

#endif // SERVICE_COMPUTER_H

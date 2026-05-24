/*
 * service_query.h — 查询统计模块
 */

#ifndef SERVICE_QUERY_H
#define SERVICE_QUERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "service_tool.h"

void choice_one();    // 查询单卡消费明细
void choice_two();    // 统计总营业额
void choice_three();  // 统计月营业额
void query_card();    // 查询卡信息（支持模糊搜索）
void query_billing(); // 查询统计入口

#endif // QUERY_SERVICE_H

/*
 * menu.h — 菜单模块头文件
 * 聚合所有业务模块头文件，声明菜单相关函数。
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "service_card.h"
#include "service_computer.h"
#include "service_billing.h"
#include "service_query.h"

void outputMenu();
void runService(int nChoice);
void initializeSystem();

#endif // MENU_H

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
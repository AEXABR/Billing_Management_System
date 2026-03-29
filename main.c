#include <stdio.h>
#include <string.h>
#include "menu.h"

int main()
{
    printf("欢迎进入计费管理系统\n");

    initializeSystem();
    int choice;
    do {
        outputMenu();
        char input[10];
        scanf("%s", input);
        printf("\n");

        if (strlen(input) > 1 || input[0] < '0' || input[0]>'8') {
            printf("输入失效！请输入阿拉伯数字0~8\n");
            continue;
        }

        choice = input[0] - '0';
        runService(choice);

    } while (choice);

    printf("已退出，欢迎下次使用！");

    return 0;
}

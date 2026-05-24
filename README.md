# 计费管理系统

基于 C 语言开发的**符合武汉理工大学课程要求**命令行计费管理系统，适用于网吧/机房场景的卡片管理、上机计费和消费统计。

## 功能模块

- **卡片管理** — 开卡、注销卡、查询卡信息（支持模糊搜索）
- **上机/下机** — 按卡号启动/结束计时，自动按 10 元/小时 计费
- **充值/退费** — 为卡片充值余额，或退还卡内余额
- **消费查询** — 单卡消费明细查询、总营业额统计、月度营业额统计

## 数据结构

| 模块 | 文件 |
|------|------|
| 程序入口 | `main.c` |
| 菜单路由 | `menu.c` / `menu.h` |
| 卡片操作 | `service_card.c` / `service_card.h` |
| 上下机管理 | `service_computer.c` / `service_computer.h` |
| 充值退费 | `service_billing.c` / `service_billing.h` |
| 查询统计 | `service_query.c` / `service_query.h` |
| 数据模型 & 工具函数 | `service_tool.c` / `service_tool.h` |

## 编译运行

```bash
# GCC 编译
gcc -o billing main.c menu.c service_card.c service_computer.c service_billing.c service_query.c service_tool.c

# 运行
./billing
```

## 数据存储

运行时会自动创建 `data/` 目录，数据以文本文件（`.txt`）和二进制文件（`.dat`）双份存储：

- `data/card.txt` / `data/card.dat` — 卡片信息（卡号、密码、状态、余额、使用次数等）
- `data/billing.txt` / `data/billing.dat` — 消费记录（卡号、上下机时间、消费金额等）

## 菜单说明

```
==========菜单==========
1.添加卡
2.查询卡
3.上机
4.下机
5.充值
6.退费
7.查询统计
8.注销卡
0.退出
```

卡状态：`0` 正在上机 (**ACTIVE**) 、`1` 未上机 (**INACTIE**) 、`2` 已注销 (**CANCELLED**)

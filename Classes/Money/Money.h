// ==========================================
// File: Money.h
// ==========================================
#ifndef __MONEY_H__
#define __MONEY_H__

#include "cocos2d.h"
#include "../Inventory/Item.h"    
#include "Constant.h"  
#include <vector>

class Money {
public:
    // 获取单例
    static Money* getInstance();
    static void destroyInstance();

    Money() : money(500) {};
    int getMoney() { return money; }
    bool canAfford(int cost);
    void spendMoney(int cost);
    void addMoney(int amount);

private:
    int money;
    static Money* _instance;

    // 统一数据结构：使用 vector 存储指针
    std::vector<Item*> _inventory;
};

#endif // __MONEY_H__
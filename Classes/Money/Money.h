#ifndef __MONEY_H__
#define __MONEY_H__

#include "cocos2d.h"
#include "Constant.h"
#include "../Inventory/Item.h"    

class Money {
public:
    // 获取单例
    static Money* getInstance();
    // 销毁单例
    static void destroyInstance();

    // 获取当前金钱 (只读)
    int getMoney() const;

    // 检查是否买得起 (只读)
    bool canAfford(int cost) const;

    // 花费金钱
    void spendMoney(int cost);

    // 增加金钱
    void addMoney(int amount);

    // 设置金钱
    void setMoney(int money) { _money = money; }

private:
    Money();
    ~Money();

private:
    static Money* _instance;

    int _money;

    // 物品库存容器
    std::vector<Item*> _inventory;
};


#endif // __MONEY_H__
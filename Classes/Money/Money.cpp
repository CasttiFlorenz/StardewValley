/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Money.cpp
 * File Function: Money类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "Money.h"


USING_NS_CC;

// 静态成员初始化
Money* Money::_instance = nullptr;

/**
 * 获取 Money 单例对象
 * 如果实例不存在，则使用 std::nothrow 创建新实例
 * @return Money 类的唯一实例指针
 */
Money* Money::getInstance() {
    // 检查实例是否存在
    if (_instance == nullptr) {
        // 使用 std::nothrow 防止内存分配异常
        _instance = new (std::nothrow) Money();
    }
    return _instance;
}

/**
 * 销毁 Money 单例对象
 * 释放内存并将静态指针置空，防止悬空指针
 */
void Money::destroyInstance() {
    // 释放单例内存
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

/**
 * 构造函数
 * 使用初始化列表设置玩家的初始金钱数值
 */
Money::Money()
    : _money(INITIAL_PLAYER_MONEY) // 使用常量列表初始化
{
}

/**
 * 析构函数
 * 清理类中持有的资源（如库存列表）
 */
Money::~Money() {
    // 析构逻辑 
    _inventory.clear();
}

/**
 * 获取当前金钱总额
 * @return 当前持有的金钱数值
 */
int Money::getMoney() const {
    return _money;
}

/**
 * 检查是否能够支付指定费用
 * @param cost 需要支付的金额
 * @return 如果当前金钱大于或等于费用返回 true，否则返回 false
 */
bool Money::canAfford(int cost) const {
    return _money >= cost;
}

/**
 * 消费金钱
 * 只有在当前金钱充足的情况下才会执行扣除操作
 * @param cost 要花费的金额
 */
void Money::spendMoney(int cost) {
    if (_money >= cost) {
        _money -= cost;
    }
}

/**
 * 增加金钱
 * @param amount 要获得的金额数值
 */
void Money::addMoney(int amount) {
    _money += amount;
}
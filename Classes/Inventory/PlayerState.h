// ==========================================
// File: PlayerState.h
// ==========================================
#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include "cocos2d.h"
#include "Item.h"      // 引用你的 Item 类
#include "Constant.h"  // 引用你的 Objects 枚举
#include <vector>

class PlayerState {
public:
    // 获取单例
    static PlayerState* getInstance();
    static void destroyInstance();

public:
    // --- 金钱管理 ---
    int money;

    bool canAfford(int cost);
    void spendMoney(int cost);
    void addMoney(int amount);

    // --- 背包管理 ---

    // 获取背包列表 (注意：统一使用 vector)
    std::vector<Item*>& getInventoryItems();

    // 添加物品 (支持堆叠)
    void addItem(Item* templateItem, int quantity);

    // 移除物品
    void removeItem(Item* item, int quantity);

private:
    // 构造函数私有化
    PlayerState();
    ~PlayerState();

    // 初始化初始道具
    void initDefaultItems();

    static PlayerState* _instance;

    // 统一数据结构：使用 vector 存储指针
    std::vector<Item*> _inventory;
};

#endif // __PLAYER_STATE_H__
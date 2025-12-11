/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.cpp
 * File Function: Item类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

class Item
{
public:
    // 构造函数
    Item();
    Item(const std::string& customName, int _count, char* _pos);

    // 获取物品信息
    char* getPos() const;
    std::string getName() const;
    int getCount() const;

    // 设置物品信息
    void setName(const std::string& name);
    void setCount(int count);

    // 物品操作
    void addCount(int amount = 1);
    void removeCount(int amount = 1);

    // 重置物品
    void clear();

private:
    char* _pos;              // 物品地址
    std::string _name;      // 物品名称
    int _count;             // 物品数量
};

#endif // __ITEM_H__#pragma once

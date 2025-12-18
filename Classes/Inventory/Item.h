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
#include "Constant.h"

class Item
{
public:
    // 构造函数
    Item();
    Item(Objects tag, int count, float scale, float printPos, char* pos, int price, std::string name);

    int getPrice() const { return _price; }
    std::string getName() const { return _name; }

    // 获取物品信息
    char* getPos() const;
    Objects getTag() const;
    int getCount() const;
    float getScale() const;
    float getPrintPos() const;

    // 设置物品信息
    void setCount(int count);

    // 物品操作
    void addCount(int amount = 1);
    void removeCount(int amount = 1);

private:
    int _price;          // 单价
    std::string _name;   // 显示名称 
    char* _pos;              // 物品地址
    Objects _tag;                // 物品标签
    int _count;              // 物品数量
    float _scale;            // 缩放大小
    float _printPos;         // 显示地址
};

#endif // __ITEM_H__#pragma once

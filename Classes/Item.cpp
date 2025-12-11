/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.h
 * File Function: Item类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#include "Item.h"

// 默认构造函数
Item::Item(): _count(0)
{
    _pos = nullptr;
    _name = "未知物品";
}

// 带参数构造函数
Item::Item(const std::string& customName,int count,char* pos)
    : _pos(pos), _count(count),_name(customName){ }

// 获取物品地址
char* Item::getPos() const
{
    return _pos;
}

// 获取物品名称
std::string Item::getName() const
{
    return _name;
}

// 获取物品数量
int Item::getCount() const
{
    return _count;
}

// 设置物品名称
void Item::setName(const std::string& name)
{
    _name = name;
}

// 设置物品数量
void Item::setCount(int count)
{
    _count = count < 0 ? 0 : count;
}

// 增加物品数量
void Item::addCount(int amount)
{
    if (amount > 0) {
        _count += amount;
    }
}

// 减少物品数量
void Item::removeCount(int amount)
{
    if (amount > 0) {
        _count -= amount;
        if (_count < 0) {
            _count = 0;
        }
    }
}

// 重置物品
void Item::clear()
{
    _pos=" ";
    _count = 0;
    _name = "未知物品";
}
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
    _tag = Objects::AXE;
}

// 带参数构造函数
Item::Item(Objects tag, int count, float scale, float printPos, char* pos, int price, std::string name)
    : _pos(pos), _count(count), _scale(scale), _printPos(printPos), _tag(tag), _price(price), _name(name)
{
}
// 获取物品地址
char* Item::getPos() const
{
    return _pos;
}

// 获取物品标签
Objects Item::getTag() const
{
    return _tag;
}

// 获取物品数量
int Item::getCount() const
{
    return _count;
}

// 获取物品缩放大小
float Item::getScale() const
{
    return _scale;
}

// 获取物品打印位置
float Item::getPrintPos() const
{
    return _printPos;
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

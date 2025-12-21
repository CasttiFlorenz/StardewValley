/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.h
 * File Function: Item类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#include "Item.h"

// 默认构造函数
Item::Item(): _count(0)
{
    _path = nullptr;
    _tag = Objects::NONE;
}

// 带参数构造函数
Item::Item(Objects tag,int count,float scale, float printPos,char* path,int price,char* name)
    : _path(path), _count(count),_scale(scale),_printPos(printPos),_tag(tag),_price(price),_name(name){ }

// 获取物品路径
char* Item::getPath() const
{
    return _path;
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

// 获取物品价格 
int Item::getPrice() const
{
    return _price;
}

// 获取物品名字
char* Item::getName() const
{
    return _name;
}



// 设置物品数量
void Item::setCount(int count)
{
    _count = count < 0 ? 0 : count;
}

// 设置物品打印位置
void Item::setPrintPos(float printPos)
{
    _printPos = printPos;
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

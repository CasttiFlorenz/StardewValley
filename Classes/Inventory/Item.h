/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.cpp
 * File Function: Item类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>
#include "json/document.h"
#include "Constant.h"

class Item
{
public:
    // 构造函数
    Item();
    Item(ItemType tag , int count, float scale,float printPos ,
        const std::string& path,int price, const std::string& name);

    static bool createFromJson(const rapidjson::Value& jsonObj, Item& item);
    // 获取物品信息
    std::string getPath() const;
    ItemType getTag() const;
    int getCount() const;
    float getScale() const;
    float getPrintPos() const;
    int getPrice() const;
    std::string getName() const;


    // 设置物品信息
    void setName(const std::string& name) { _name = name; }
    void setPrice(int price) { _price = price; }
    void setTag(ItemType tag) { _tag = tag; }
    void setCount(int count) { _count = count; }
    void setScale(float scale) { _scale = scale; }
    void setPrintPos(float printPos) { _printPos = printPos; }
    void setPath(const std::string& path) { _path = path; }

    // 物品数量操作
    void addCount(int amount = 1);
    void removeCount(int amount = 1);

private:
    int _price;                // 单价
    std::string _name;         // 物品名字
    std::string _path;         // 物品路径
    ItemType _tag;             // 物品标签
    int _count;                // 物品数量
    float _scale;              // 缩放大小
    float _printPos;           // 显示地址
};

#endif // __ITEM_H__#pragma once

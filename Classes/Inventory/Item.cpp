/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.cpp
 * File Function: Item类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#include "Item.h"

// 默认构造函数
Item::Item() : _tag(ItemType::NONE), _count(0), _scale(1.0f),
_printPos(0.0f), _path(""), _price(0), _name("") {
}

// 带参数构造函数
Item::Item(ItemType tag, int count, float scale, float printPos,
    const std::string& path, int price, const std::string& name)
    : _tag(tag), _count(count), _scale(scale), _printPos(printPos),
    _path(path), _price(price), _name(name) {}

// 存档物品
bool Item::createFromJson(const rapidjson::Value& jsonObj, Item& item)
{
    if (!jsonObj.IsObject()) return false;

    // 检查必需字段
    if (!jsonObj.HasMember("name") || !jsonObj["name"].IsString()) return false;
    if (!jsonObj.HasMember("path") || !jsonObj["path"].IsString()) return false;
    if (!jsonObj.HasMember("tag") || !jsonObj["tag"].IsInt()) return false;
    if (!jsonObj.HasMember("count") || !jsonObj["count"].IsInt()) return false;
    if (!jsonObj.HasMember("scale") || !jsonObj["scale"].IsFloat()) return false;
    if (!jsonObj.HasMember("printPos") || !jsonObj["printPos"].IsFloat()) return false;
    if (!jsonObj.HasMember("price") || !jsonObj["price"].IsInt()) return false;

    // 直接赋值给std::string
    item.setName(jsonObj["name"].GetString());
    item.setPath(jsonObj["path"].GetString());
    item.setTag(static_cast<ItemType>(jsonObj["tag"].GetInt()));
    item.setCount(jsonObj["count"].GetInt());
    item.setScale(jsonObj["scale"].GetFloat());
    item.setPrintPos(jsonObj["printPos"].GetFloat());
    item.setPrice(jsonObj["price"].GetInt());

    return true;
}

// 获取物品路径
std::string Item::getPath() const
{
    return _path;
}

// 获取物品标签
ItemType Item::getTag() const
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
std::string Item::getName() const
{
    return _name;
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


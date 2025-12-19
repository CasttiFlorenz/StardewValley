/****************************************************************
 * Project Name:  StardewValley
 * File Name:     PlaceItems.cpp
 * File Function: PlaceItems类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#include "PlaceItems.h"

USING_NS_CC;

PlaceItems::PlaceItems()
    : _parent(nullptr)
    , _currentInventory(nullptr)
    , _itemCount(0)
    , _backpackX(nullptr)
    , _backpackY(nullptr)
    , _backpackWidth(0)
    , _cellWidth(0)
{
}

// 设置上下文信息
void PlaceItems::setContext(cocos2d::Node* parent, std::vector<Item>& inventory, int& itemCount,
    float* backpackX, float* backpackY, float backpackWidth, float cellWidth)
{
    _parent = parent;
    _currentInventory = &inventory;
    _itemCount = &itemCount;
    _backpackX = backpackX;
    _backpackY = backpackY;
    _backpackWidth = backpackWidth;
    _cellWidth = cellWidth;
}

// 初始化物品数组
void PlaceItems::initItem()
{
    // 清空 vector
    _currentInventory->clear();

    // 初始化所有物品为空
    for (int i = 0; i < 36; i++) {
        _inventory[i] = Item();
    }

    // 放置初始物品
    _inventory[0] = Item(Objects::HOE, 1, 1.5f, 0.0f, "/Items/hoe.png", 0, "Hoe");
    _inventory[1] = Item(Objects::AXE, 1, 1.4f, 0.0f, "/Items/axe.png", 0, "Axe");
    _inventory[2] = Item(Objects::WATERING_CAN, 1, 1.6f, 0.0f, "/Items/wateringCan.png", 0, "Watering Can");
    _inventory[3] = Item(Objects::PICKAXE, 1, 1.4f, 0.0f, "/Items/pickaxe.png", 0, "Pickaxe");
    _inventory[4] = Item(Objects::SCYTHE, 1, 1.4f, 0.0f, "/Items/scythe.png", 0, "Scythe");
    _inventory[5] = Item(Objects::FISHINGROD, 1, 1.3f, 0.0f, "/Items/fishingRod.png", 0, "Fishingrod");
    _inventory[6] = Item(Objects::STONE, 1, 4.3f, 0.0f, "/Items/stone.png", 0, "Stone");
    _inventory[7] = Item(Objects::WOOD, 1, 2.8f, 0.0f, "/Items/wood.png", 0, "Wood");
    _inventory[8] = Item(Objects::COPPER_ORE, 1, 3.3f, 0.0f, "/Items/copper ore.png", 0, "Copper Ore");
    _inventory[9] = Item(Objects::FERTILIZER, 1, 4.3f, 0.0f, "/Items/fertilizer.png", 0, "Fertilizer");
    _inventory[10] = Item(Objects::PARSNIP_SEED, 1, 4.3f, 0.0f, "/Items/parsnip seed.png", 0, "Parsnip Seed");
    _inventory[11] = Item(Objects::CAULIFLOWER_SEED, 1, 4.3f, 0.0f, "/Items/cauliflower seed.png", 0, "Cauliflower Seed");
    _inventory[12] = Item(Objects::POTATO_SEED, 1, 4.3f, 0.0f, "/Items/potato seed.png", 0, "Potato Seed");
    _inventory[13] = Item(Objects::PARSNIP, 1, 4.3f, 0.0f, "/Items/parsnip.png", 0, "Parsnip");
    _inventory[14] = Item(Objects::CAULIFLOWER, 1, 4.3f, 0.0f, "/Items/cauliflower.png", 0, "Cauliflower");
    _inventory[15] = Item(Objects::POTATO, 1, 4.3f, 0.0f, "/Items/potato.png", 0, "Potato");
    _inventory[16] = Item(Objects::FIBER, 1, 4.3f, 0.0f, "/Items/fiber.png", 0, "Fiber");
    _inventory[17] = Item(Objects::DAFFODILS, 1, 4.3f, 0.0f, "/Items/daffodils.png", 0, "Daffodils");
    _inventory[18] = Item(Objects::LEEK, 1, 4.3f, 0.0f, "/Items/leek.png", 0, "Leek");
    _inventory[19] = Item(Objects::HAY, 0, 4.3f, 0.0f, "/Items/hay.png", 0, "Hay");
    _inventory[20] = Item(Objects::EGG, 1, 4.8f, 0.0f, "/Items/egg.png", 0, "Egg");
    _inventory[21] = Item(Objects::FRIED_EGG, 1, 4.3f, 0.0f, "/Items/fried egg.png", 0, "Fried Egg");
    _inventory[22] = Item(Objects::CARP, 1, 4.3f, 0.0f, "/Items/carp.png", 0, "Carp");
    _inventory[23] = Item(Objects::MILK, 1, 4.3f, 0.0f, "/Items/milk.png", 0, "Milk");
    _inventory[24] = Item(Objects::SALAD, 1, 4.3f, 0.0f, "/Items/salad.png", 0, "Salad");

    // 放置初始物品 - 使用 push_back
    _currentInventory->push_back(Item(Objects::HOE, 1, 1.5f, 0.7f, "/Items/hoe.png", 0, "Hoe"));
    _currentInventory->push_back(Item(Objects::AXE, 1, 1.4f, 1.6f, "/Items/axe.png", 0, "Axe"));
    _currentInventory->push_back(Item(Objects::WATERING_CAN, 1, 1.6f, 2.6f, "/Items/wateringCan.png", 0, "Watering Can"));
    _currentInventory->push_back(Item(Objects::PICKAXE, 1, 1.4f, 3.6f, "/Items/pickaxe.png", 0, "Pickaxe"));
    _currentInventory->push_back(Item(Objects::SCYTHE, 1, 1.4f, 4.6f, "/Items/scythe.png", 0, "Scythe"));
    _currentInventory->push_back(Item(Objects::FISHINGROD, 1, 1.3f, 5.5f, "/Items/fishingRod.png", 0, "Fishingrod"));
    _currentInventory->push_back(Item(Objects::PARSNIP_SEED, 5, 4.3f, 6.5f, "/Items/parsnip seed.png", 0, "Parsnip Seed"));

    (*_itemCount) = _currentInventory->size();
}

// 放置物品
void PlaceItems::placeTools(int highlightIndex)
{
    if (!_parent || !_currentInventory || !_backpackX || !_backpackY)
        return;

    // 清除所有已存在的工具精灵
    clearAllTools();

    // 计算背包左边界（使用第一个背包）
    float backpackLeft0 = _backpackX[0] - _backpackWidth / 2;

    // 遍历工具格
    for (size_t i = 0; i < _currentInventory->size(); i++) {
        Item& item = (*_currentInventory)[i];
        float baseScale = item.getScale();       // 基础大小
        float highlightScale = baseScale + 0.2f; // 高亮时放大

        float x = backpackLeft0 + _cellWidth * item.getPrintPos(); // 简单位置计算
        float scale = (i == highlightIndex) ? highlightScale : baseScale;

        // 确定在哪一行
        int row = i / 12;
        float y = _backpackY[row];

        updateToolSprite(i, item.getPath(), scale, x,y);
    }
}

// 清除物品精灵
void PlaceItems::clearAllTools()
{
    if (!_parent) return;

    // 遍历所有可能的工具tag
    for (int i = 0; i < (*_itemCount); i++) {
        // 计算当前工具的tag
        int tag = TOOL_TAG_BASE + i;

        // 查找并移除该tag的精灵
        auto toolSprite = _parent->getChildByTag(tag);
        if (toolSprite) {
            toolSprite->removeFromParent();
        }

        // 同时移除数量标签
        int countTag = COUNT_TAG_BASE + i;
        auto countLabel = _parent->getChildByTag(countTag);
        if (countLabel) {
            countLabel->removeFromParent();
        }
    }
}

// 显示物品数量
void PlaceItems::showItemCount(int itemIndex, int count, float x, float y)
{
    if (!_parent) return;

    // 先清除该索引原有的数量标签
    int oldCountTag = COUNT_TAG_BASE + itemIndex;
    auto oldCountLabel = _parent->getChildByTag(oldCountTag);
    if (oldCountLabel) {
        oldCountLabel->removeFromParent();
    }

    // 创建数量标签
    std::string countText = std::to_string(count);
    auto countLabel = Label::createWithTTF(countText, "/fonts/arial.ttf", 14);

    if (countLabel) {
        // 计算右下角位置
        float offsetX = 25.0f;  // 向右偏移
        float offsetY = -22.0f; // 向下偏移

        countLabel->setPosition(Vec2(x + offsetX, y + offsetY));
        countLabel->setColor(Color3B::WHITE);
        countLabel->enableOutline(Color4B::BLACK, 2);  // 黑色描边，更清晰

        // 设置唯一tag
        countLabel->setTag(COUNT_TAG_BASE + itemIndex);

        _parent->addChild(countLabel, 2);  // 比工具精灵更高的层级
    }
}

// 更新工具精灵
void PlaceItems::updateToolSprite(int toolIndex, const std::string& imagePath,
    float size, float x, float y)
{
    if (!_parent || !_currentInventory || toolIndex < 0 || toolIndex >= *_itemCount)
        return;

    // 计算当前工具的tag
    int tag = TOOL_TAG_BASE + toolIndex;

    // 查找是否已存在相同tag的精灵
    auto oldSprite = _parent->getChildByTag(tag);
    if (oldSprite) {
        oldSprite->removeFromParent();
    }

    // 创建新的工具精灵
    auto sprite = Sprite::create(imagePath);
    if (sprite) {
        Item& item = (*_currentInventory)[toolIndex];

        sprite->setPosition(Vec2(x, y));
        sprite->setScale(size);
        if (item.getCount() <= 0) {
            sprite->setOpacity(70); // 透明度 
        }

        // 设置唯一tag，便于后续管理
        sprite->setTag(tag);

        // 添加到场景中
        _parent->addChild(sprite, 1);

        // 显示数量
        if (item.getTag() > Objects::FISHINGROD) {
            showItemCount(toolIndex, item.getCount(), x, y);
        }

        // 添加点击时的轻微动画效果
        if (size > item.getScale()) {  // 如果是高亮状态
            // 轻微上浮动画
            auto floatUp = MoveBy::create(0.1f, Vec2(0, 3));
            sprite->runAction(floatUp);
        }
    }
}

// 增加物品
bool PlaceItems::addItem(Objects object, int amount)
{
    if (!_currentInventory || amount <= 0)
        return false;

    // 先在 currentInventory 中查找是否已有该物品
    for (auto& item : *_currentInventory) {
        if (item.getTag() == object) {
            // 找到物品，增加数量
            item.addCount(amount);
            return true;
        }
    }

    // 如果没有找到，在 inventory 中搜索信息
    for (int i = 0; i < 36; i++) {
        if (_inventory[i].getTag() == object) {
            // 创建新物品并添加到 vector
            Item newItem = _inventory[i];
            newItem.setCount(amount);
            newItem.setPrintPos(_pos[(*_itemCount) % 12]);

            _currentInventory->push_back(newItem);
            (*_itemCount) = _currentInventory->size();  // 更新计数
            return true;
        }
    }

    // 如果都没有找到，返回失败
    return false;
}

// 减少物品数量
bool PlaceItems::removeItem(Objects object, int amount)
{
    if (!_currentInventory || amount <= 0)
        return false;

    // 在 currentInventory 中查找物品
    for (auto it = _currentInventory->begin(); it != _currentInventory->end(); ++it) {
        if (it->getTag() == object) {
            // 找到物品，减少数量
            int newCount = it->getCount() - amount;

            if (newCount > 0) {
                // 数量仍大于0，更新数量
                it->setCount(newCount);
                return true;
            }
            else if (newCount <= 0) {
                // 数量为0，删除该物品
                _currentInventory->erase(it);
                (*_itemCount) = _currentInventory->size();  // 更新计数
                adjustPos();
                return true;
            }
            else {
                // 数量不足，无法减少
                CCLOG("错误: 物品 %d 数量不足 (现有: %d, 需要减少: %d)",
                    static_cast<int>(object), it->getCount(), amount);
                return false;
            }
        }
    }

    // 没有找到该物品
    CCLOG("错误: 找不到物品 %d", static_cast<int>(object));
    return false;
}

// 调整删除位置后面的物品位置
void PlaceItems::adjustPos()
{
    if (!_currentInventory) return;

    // 遍历所有物品，重新计算位置
    for (size_t i = 0; i < _currentInventory->size(); i++) {
        Item& item = (*_currentInventory)[i];

        item.setPrintPos(_pos[i % 12]);
    }

}

int PlaceItems::findItemIndex(Objects tag)
{
    if (!_currentInventory) 
        return -1;

    for (int i = 0; i < _currentInventory->size(); i++) {
        if ((*_currentInventory)[i].getTag() == tag) {
            return i;
        }
    }
    return -1;
}
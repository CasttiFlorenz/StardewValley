// ==========================================
// File: PlayerState.cpp
// ==========================================
#include "PlayerState.h"

USING_NS_CC;

PlayerState* PlayerState::_instance = nullptr;

PlayerState* PlayerState::getInstance() {
    if (!_instance) {
        _instance = new PlayerState();
    }
    return _instance;
}

void PlayerState::destroyInstance() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

// 构造函数：只做最基本的赋值，调用初始化函数
PlayerState::PlayerState() {
    money = 500; // 初始金钱

    // 初始化背包
    initDefaultItems();
}

// 析构函数：清理内存
PlayerState::~PlayerState() {
    for (auto item : _inventory) {
        delete item; // 释放 vector 中的 Item 对象
    }
    _inventory.clear();
}

// 初始化默认道具 (替代你原来的数组赋值)
void PlayerState::initDefaultItems() {
    // 辅助 lambda：简化添加代码
    auto add = [&](Objects tag, float scale, float printPos, const char* path, int price, std::string name, int count = 1) {
        Item* item = new Item(tag, count, scale, printPos, (char*)path, price, name);
        _inventory.push_back(item);
        };

    // 这里完全照搬你之前的数据，但改成了动态添加
    add(Objects::HOE, 1.5f, 0.7f, "/Items/hoe.png", 100, "Hoe");
    add(Objects::AXE, 1.4f, 1.6f, "/Items/axe.png", 100, "Axe");
    add(Objects::WATERING_CAN, 1.6f, 2.6f, "/Items/wateringCan.png", 100, "Watering Can");
    add(Objects::PICKAXE, 1.4f, 3.6f, "/Items/pickaxe.png", 100, "Pickaxe");
    add(Objects::SCYTHE, 1.4f, 4.6f, "/Items/scythe.png", 100, "Scythe");
    add(Objects::FISHINGROD, 1.3f, 5.5f, "/Items/fishingRod.png", 100, "Fishing Rod");

    // 初始资源
    add(Objects::STONE, 4.3f, 6.5f, "/Items/stone.png", 100, "Stone", 1);
    add(Objects::WOOD, 2.8f, 7.5f, "/Items/wood.png", 100, "Wood", 1);
    add(Objects::COPPER_ORE, 3.3f, 8.4f, "/Items/copper ore.png", 100, "Copper Ore", 1);
    add(Objects::FERTILIZER, 4.3f, 9.4f, "/Items/fertilizer.png", 100, "Fertilizer", 1);
    add(Objects::PARSNIP_SEED, 4.3f, 10.4f, "/Items/parsnip seed.png", 20, "Parsnip Seed", 5); // 给5个种子

    // ... 你可以继续添加剩下的 ...
}

bool PlayerState::canAfford(int cost) {
    return money >= cost;
}

void PlayerState::spendMoney(int cost) {
    if (money >= cost) {
        money -= cost;
    }
}

void PlayerState::addMoney(int amount) {
    money += amount;
}

std::vector<Item*>& PlayerState::getInventoryItems() {
    return _inventory;
}

// 添加物品逻辑 (买东西时调用)
void PlayerState::addItem(Item* templateItem, int quantity) {
    if (!templateItem) return;

    // 1. 检查堆叠
    for (auto item : _inventory) {
        if (item->getTag() == templateItem->getTag()) {
            item->addCount(quantity);
            CCLOG("Stacking item: %s, new count: %d", item->getName().c_str(), item->getCount());
            return;
        }
    }

    // 2. 新增物品
    // 使用深拷贝创建一个新对象
    Item* newItem = new Item(
        templateItem->getTag(),
        quantity,
        templateItem->getScale(),
        templateItem->getPrintPos(),
        templateItem->getPos(),
        templateItem->getPrice(),
        templateItem->getName()
    );
    _inventory.push_back(newItem);
    CCLOG("Added new item: %s", newItem->getName().c_str());
}

// 移除物品逻辑 (卖东西时调用)
void PlayerState::removeItem(Item* targetItem, int quantity) {
    for (auto it = _inventory.begin(); it != _inventory.end(); ++it) {
        Item* item = *it;

        // 匹配指针地址 或者 Tag
        if (item == targetItem || item->getTag() == targetItem->getTag()) {

            item->removeCount(quantity);

            // 数量归零则彻底删除
            if (item->getCount() <= 0) {
                delete item; // 释放内存
                _inventory.erase(it);
            }
            return;
        }
    }
}
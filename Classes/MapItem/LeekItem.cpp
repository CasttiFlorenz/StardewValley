#include "LeekItem.h"

// 创建实例
LeekItem* LeekItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) LeekItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 初始化
bool LeekItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::LEEK, tileCoord)) {
        return false;
    }

    this->setTexture(LEEK_ITEM_TEXTURE_PATH);
    return true;
}
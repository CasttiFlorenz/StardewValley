#include "StoneItem.h"

// 创建实例
StoneItem* StoneItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) StoneItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 初始化
bool StoneItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::STONE, tileCoord)) {
        return false;
    }

    this->setTexture(STONE_ITEM_TEXTURE_PATH);
    return true;
}
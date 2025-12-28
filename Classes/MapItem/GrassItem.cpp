#include "GrassItem.h"

// 创建实例
GrassItem* GrassItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) GrassItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 初始化
bool GrassItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::GRASS, tileCoord)) {
        return false;
    }

    this->setTexture(GRASS_ITEM_TEXTURE_PATH);
    return true;
}
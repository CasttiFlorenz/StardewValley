#include "EnvironmentItem.h"

// 创建实例
EnvironmentItem* EnvironmentItem::create(EnvironmentItemType type, const Vec2& tileCoord) {
    auto p = new (std::nothrow) EnvironmentItem();
    if (p && p->init(type, tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 初始化
bool EnvironmentItem::init(EnvironmentItemType type, const Vec2& tileCoord) {
    if (!Sprite::init()) {
        return false;
    }

    _type = type;
    _tileCoord = tileCoord;

    return true;
}
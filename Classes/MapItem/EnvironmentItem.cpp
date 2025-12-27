#include "EnvironmentItem.h"

// 工厂方法：创建环境物体实例
EnvironmentItem* EnvironmentItem::create(EnvironmentItemType type, const Vec2& tileCoord) {
    auto p = new (std::nothrow) EnvironmentItem();
    if (p && p->init(type, tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool EnvironmentItem::init(EnvironmentItemType type, const Vec2& tileCoord) {
    // 初始化 Sprite 基类
    if (!Sprite::init()) {
        return false;
    }

    // 记录逻辑属性
    _type = type;
    _tileCoord = tileCoord;

    return true;
}

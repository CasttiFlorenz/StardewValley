/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameMap.h
 * File Function: GameMap类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

class GameMap : public Scene
{
public:
    virtual bool init() = 0;
    virtual void update(float dt) = 0;

    virtual bool isCollidable(Vec2 worldPos);

    Vec2 calMapPos(Vec2 worldPos);
    // 将瓦片坐标转换为 TiledMap 内部的节点坐标（中心点）
    Vec2 calWorldPos(const Vec2& tileCoord);

    Rect getObjectRect(const std::string& objectName);

    TMXTiledMap* getTiledMap() const { return _map; }

    std::string getMapName() const { return _mapName; }

    virtual bool isCameraFollow() const { return false; }
    virtual void setStartPosition(std::string lastMap) = 0;
    virtual Vec2 getPlayerStartPosition(std::string lastMap) = 0;

    virtual std::string getNewMap(const Vec2& curPos, bool isStart, const Direction& direction) = 0;
    virtual bool isOutdoor() { return false; }

    // 左键和右键事件
    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction,Objects objects) { return MouseEvent::NONE; }
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction){ return MouseEvent::NONE; }

protected:
    TMXTiledMap* _map;
    std::string _mapName;
};

#endif // __GAME_MAP_H__
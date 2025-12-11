/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameMap.h
 * File Function: GameMap类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
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

    bool isCollidable(Vec2 worldPos);

    Vec2 calMapPos(Vec2 worldPos);

    Rect getObjectRect(const std::string& objectName);

    TMXTiledMap* getTiledMap() const { return _map; }

    std::string getMapName() const { return _mapName; }

    virtual void setStartPosition(std::string lastMap) = 0;
    virtual Vec2 getPlayerStartPosition(std::string lastMap) = 0;

    virtual std::string getNewMap(const GameMap* curMap, const Vec2& curPos, bool isStart) = 0;

protected:
    TMXTiledMap* _map;
    std::string _mapName;
};

#endif // __GAME_MAP_H__

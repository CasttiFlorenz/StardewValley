// NPCBase.h
#ifndef __NPC_BASE_H__
#define __NPC_BASE_H__

#include "cocos2d.h"
#include "Constant.h" 

USING_NS_CC;

class NPCBase : public Sprite
{
public:
    virtual bool init() override;
    virtual void playAnimation() = 0;
    virtual std::vector<std::string> getConversation(bool isFirstMet) = 0;

    // 基础属性设置
    void setNPCPosition(const Vec2& position);
    void setNPCScale(float scale);
    void setNPCName(const std::string& name) { _npcName = name; }
    std::string getNPCName() const { return _npcName; }

    // --- 好感度相关 ---

    // 接收礼物
    std::string receiveGift(Objects itemTag);

    // 获取当前好感度
    int getFriendship() const { return _friendship; }

protected:
    bool loadTexture(const std::string& path);
    void createAnimation(int totalRows, int totalCols, int animationRow, float frameDelay = 0.15f);

    virtual int checkGiftTaste(Objects itemTag);
    void increaseFriendship(int amount);
private:
    bool _isTextureLoaded;
    std::string _npcName;

    int _friendship = 0; // 好感度数值
};

#endif // __NPC_BASE_H__
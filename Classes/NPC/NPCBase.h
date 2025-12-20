// NPCBase.h
#ifndef __NPC_BASE_H__
#define __NPC_BASE_H__

#include "cocos2d.h"
#include"Constant.h"
USING_NS_CC;

class NPCBase : public Sprite
{
public:
    virtual bool init() override;
    virtual void playAnimation() = 0;  

    void setNPCPosition(const Vec2& position);
    void setNPCScale(float scale);
    void setNPCName(const std::string& name) { _npcName = name; }
    std::string getNPCName() const { return _npcName; }

    std::string getConversation(bool isFirstTalkToday); // 获取对话文本
    std::string receiveGift(Objects itemTag);    // 接收礼物
 
protected:
    bool loadTexture(const std::string& path);
    void createAnimation(int totalRows, int totalCols, int animationRow, float frameDelay = 0.15f);

private:
    bool _isTextureLoaded;
    std::string _npcName;

    int _friendship = 0; // 好感度 (0-2500)
    bool _hasGiftedToday = false; // 今天是否送过礼
    int checkGiftTaste(Objects item); // 返回好感度变化值
};

#endif // __NPC_BASE_H__
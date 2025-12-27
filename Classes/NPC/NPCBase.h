#ifndef __NPC_BASE_H__
#define __NPC_BASE_H__

#include "cocos2d.h"
#include "Constant.h" 
#include <vector>
#include <string>

class NPCBase : public cocos2d::Sprite
{
public:
    // 初始化 NPC 基础属性
    virtual bool init() override;

    // 播放 NPC 动画（纯虚函数，子类必须实现）
    virtual void playAnimation() = 0;

    // 获取 NPC 对话列表（纯虚函数，子类必须实现）
    virtual std::vector<std::string> getConversation(bool isFirstMet) = 0;

    // 设置 NPC 在地图上的位置
    void setNPCPosition(const cocos2d::Vec2& position);

    // 设置 NPC 的缩放比例
    void setNPCScale(float scale);

    // 设置 NPC 名字
    void setNPCName(const std::string& name) { _npcName = name; }

    // 获取 NPC 名字
    std::string getNPCName() const { return _npcName; }

    // 接收礼物并返回对应的对话文本
    std::string receiveGift(ItemType itemTag);

    // 获取当前好感度数值
    int getFriendship() const { return _friendship; }

    //设置好好感值
    void setNPCFriendship(int friendship) { _friendship = friendship; };

protected:
    // 加载纹理图片
    bool loadTexture(const std::string& path);

    // 创建并播放帧动画
    void createAnimation(int totalRows, int totalCols, int animationRow, float frameDelay = 0.15f);

    // 检查礼物喜好并返回分值（虚函数）
    virtual int checkGiftTaste(ItemType itemTag);

    // 增加好感度（包含数值范围限制）
    void increaseFriendship(int amount);

private:
    bool _isTextureLoaded;
    std::string _npcName;
    int _friendship;
};

#endif // __NPC_BASE_H__
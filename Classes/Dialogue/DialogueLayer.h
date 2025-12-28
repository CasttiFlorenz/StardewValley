/****************************************************************
 * Project Name:  StardewValley
 * File Name:     DialogueLayer.h
 * File Function: DialogueLayer类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#ifndef __DIALOGUE_LAYER_H__
#define __DIALOGUE_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include <vector>

class DialogueLayer : public cocos2d::Layer
{
public:
    // 创建 DialogueLayer 实例（多实例对象）
    static DialogueLayer* create();

    // 初始化层
    virtual bool init() override;

    // 显示普通对话（传入名字和对话列表）
    void showText(const std::string& name, const std::vector<std::string>& contentList);

    // 定义选择回调类型 (true=Yes, false=No)
    using ChoiceCallback = std::function<void(bool)>;

    // 显示带选项的对话（重载1：仅内容）
    void showChoice(const std::string& content, ChoiceCallback callback);

    // 显示带选项的对话（重载2：带名字和内容）
    void showChoice(const std::string& name, const std::string& content, ChoiceCallback callback);

private:
    // 创建 YES/NO 选择按钮
    void createChoiceButtons();

    // 处理按钮点击事件
    void handleChoiceClick(cocos2d::Ref* sender, bool isYes);

    // 执行选择后的逻辑分发
    void onChoiceMade(bool choice);

    // 根据 NPC 名字更新背景纹理
    void updateBackground(const std::string& name);

    // 从队列中取出并显示下一句文本
    void showNextSentence();

private:
    cocos2d::ui::Scale9Sprite* _bg;
    cocos2d::Label* _nameLabel;
    cocos2d::Label* _contentLabel;
    cocos2d::Node* _choiceNode;
    std::queue<std::string> _dialogueQueue;
    ChoiceCallback _choiceCallback = nullptr;
};

#endif // __DIALOGUE_LAYER_H__
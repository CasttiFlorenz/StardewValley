#ifndef __DIALOGUE_LAYER_H__
#define __DIALOGUE_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DialogueLayer : public cocos2d::Layer
{
public:
    static DialogueLayer* create();
    virtual bool init();

    void showText(const std::string& name, const std::vector<std::string>& contentList);
   
    using ChoiceCallback = std::function<void(bool)>;  
    void onChoiceMade(bool choice);
    void createChoiceButtons();
    void showChoice(const std::string& content, ChoiceCallback callback);
    void updateBackground(const std::string& name);
    void showChoice(const std::string& name, const std::string& content, ChoiceCallback callback);
private:
    cocos2d::ui::Scale9Sprite* _bg;
    cocos2d::Label* _nameLabel;
    cocos2d::Label* _contentLabel;
    cocos2d::Node* _choiceNode; 
    void showNextSentence();
    std::queue<std::string> _dialogueQueue;
    ChoiceCallback _choiceCallback = nullptr;
};

#endif
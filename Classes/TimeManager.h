#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "cocos2d.h"
#include "GameTime.h"
#include "WeatherManager.h"
#include <functional>

class TimeManager : public cocos2d::Node {
public: 
    static TimeManager* getInstance();
    virtual bool init();
    void update(float dt) override;

    void triggerPassOut();
    void startNextDay();

    bool isInputAllowed() const { return _isInputAllowed; }
    void setInputAllowed(bool val) { _isInputAllowed = val; }

    GameTime getCurrentTime() const { return currentTime; }

    std::function<void()> onDayStartCallback;

private:
    TimeManager();
    ~TimeManager();
    static TimeManager* instance;

    GameTime currentTime;
    float accumulatedTime;
    bool _isInputAllowed;
    bool _isTransitioning;

    cocos2d::Node* uiContainer;
    cocos2d::Sprite* clockBg;
    cocos2d::Sprite* clockHand;
    cocos2d::Sprite* weatherIcon; 
    cocos2d::Label* timeLabel;
    cocos2d::Label* dateLabel;
    cocos2d::Label* moneyLabel;

    void createUI();
    void refreshUI();
};

#endif // __TIME_MANAGER_H__

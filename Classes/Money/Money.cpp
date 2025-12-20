/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Money.cpp
 * File Function: Money类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/17
 * License:       MIT License
 ****************************************************************/
#include "Money.h"

USING_NS_CC;

Money* Money::_instance = nullptr;

Money* Money::getInstance() {
    if (!_instance) {
        _instance = new Money();
    }
    return _instance;
}

void Money::destroyInstance() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

bool Money::canAfford(int cost) {
    return money >= cost;
}

void Money::spendMoney(int cost) {
    if (money >= cost) {
        money -= cost;
    }
}

void Money::addMoney(int amount) {
    money += amount;
}
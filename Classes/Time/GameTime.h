/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameTime.h
 * File Function: GameTime类的声明
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/
#ifndef GAME_TIME_H
#define GAME_TIME_H

#include "cocos2d.h"
#include "Constant.h" 
#include <vector>

class GameTime {
public:
    // 默认构造函数
    GameTime();

    // 带参构造函数
    GameTime(int y, Season s, int d, int h, int m);

    // ==================== 核心逻辑 ====================
    // 增加分钟数，内部处理进位逻辑
    void addMinutes(int minutesToAdd);

    // 增加天数，内部处理季节更替
    void addDays(int daysToAdd);

    // ==================== 数据获取与格式化 ====================
    // 获取格式化时间字符串 (例如 "6:00 am")
    std::string getTimeString() const;

    // 获取格式化日期字符串 (例如 "Mon. 1")
    std::string getDateString() const;

    // 获取季节名称字符串
    std::string getSeasonString() const;

    // 获取完整调试信息字符串
    std::string getFullString() const;

    // ==================== 辅助计算 ====================
    // 计算当前是星期几
    DayOfWeek getDayOfWeek() const;

    // 判断是否为同一天
    bool isSameDay(const GameTime& other) const;

    // 判断是否为同一小时
    bool isSameHour(const GameTime& other) const;

    // ==================== Getters ====================
    int getYear() const { return _year; }
    Season getSeason() const { return _season; }
    int getDayOfMonth() const { return _dayOfMonth; }
    int getHour() const { return _hour; }
    int getMinute() const { return _minute; }


    // ====================设置时间信息 ====================
    void setSeason(Season season) { _season = season; }
    void setHour(int hour) { _hour = hour; }
    void setYear(int year) { _year=year; }
    void setHour(int dayOfMonth) { _dayOfMonth = dayOfMonth; }
    void setMinute(int minute) { _minute = minute; }
private:
    // 处理日期变更和季节轮替的内部逻辑
    void handleDayRollover();

private:
    int _year;
    Season _season;
    int _dayOfMonth;
    int _hour;
    int _minute;
};


#endif // GAME_TIME_H

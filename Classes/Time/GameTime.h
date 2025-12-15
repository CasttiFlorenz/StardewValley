/***************************************************************
 * Project Name : StardewValley
 * File Name : GameTime.h
 * File Function : GameTime类的实现
 * Author : 赵睿妍
 * Update Date : 2025 / 12 / 13
 * License : MIT License
* ***************************************************************/

#ifndef __GAME_TIME_H__
#define __GAME_TIME_H__

#include "cocos2d.h"
#include <string>

 // 季节枚举 (0:春, 1:夏, 2:秋, 3:冬)
enum class Season {
    Spring = 0,
    Summer = 1,
    Fall = 2,
    Winter = 3
};

// 星期枚举 (星露谷规则：每月1号固定为周一)
enum DayOfWeek {
    MON = 0, TUE = 1, WED = 2, THU = 3, FRI = 4, SAT = 5, SUN = 6
};

class GameTime {
public:
    int year;           // 年份 (从1开始)
    Season season;      // 季节
    int dayOfMonth;     // 日期 (1-28)
    int hour;           // 小时 (0-26, 其中24=0点, 26=凌晨2点)
    int minute;         // 分钟 (0-59)

    // 默认构造: Y1 Spring 1, 6:00
    GameTime();
    // 指定构造
    GameTime(int y, Season s, int d, int h, int m);

    // ==================== 核心操作 ====================

    // 增加分钟 (会自动处理小时进位)
    void addMinutes(int minutes);

    // 增加天数 (会自动处理季节、年份进位)
    void addDays(int days);

    // ==================== 数据获取 (UI显示用) ====================

    // 获取 12小时制 时间字符串 (例如 "6:10 am", "12:30 pm")
    // 会自动处理 25点、26点显示为 1am, 2am
    std::string getTimeString() const;

    // 获取 日期字符串 (例如 "Mon. 1", "Sun. 28")
    std::string getDateString() const;

    // 获取 季节名称字符串 (例如 "Spring")
    std::string getSeasonString() const;

    // 获取 完整调试信息 (例如 "Y1 Spring D1 - 6:00 am")
    std::string getFullString() const;

    // ==================== 逻辑辅助 ====================

    // 获取今天是星期几 (基于每月1号=周一的规则)
    DayOfWeek getDayOfWeek() const;

    // 判断两个时间是否在同一天
    bool isSameDay(const GameTime& other) const;

    // 判断两个时间是否在同一小时
    bool isSameHour(const GameTime& other) const;

private:
    // 内部处理月份超过28天的逻辑
    void handleDayRollover();
};

#endif // __GAME_TIME_H__

/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GameTime.cpp
 * File Function: GameTime类的实现
 * Author:        赵睿妍
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#include "GameTime.h"

USING_NS_CC;

// ==================== 构造函数 ====================

/**
 * 默认构造函数，初始化时间为游戏开始的默认值 (第1年春季1日 6:00)
 */
GameTime::GameTime()
    : _year(START_YEAR)
    , _season(Season::Spring)
    , _dayOfMonth(START_DAY)
    , _hour(START_HOUR)
    , _minute(0) {
}

/**
 * 带参构造函数，使用指定的年月日时分初始化对象
 */
GameTime::GameTime(int y, Season s, int d, int h, int m)
    : _year(y)
    , _season(s)
    , _dayOfMonth(d)
    , _hour(h)
    , _minute(m) {
}


// ==================== 核心进位逻辑 ====================

/**
 * 增加分钟数，处理小时进位以及凌晨时间的循环机制
 */
void GameTime::addMinutes(int minutesToAdd) {
    _minute += minutesToAdd;

    // 分钟进位处理：超过60分钟进位到小时
    while (_minute >= MINUTES_PER_HOUR) {
        _minute -= MINUTES_PER_HOUR;
        _hour++;
    }

    // 时间超过所允许的最晚时间，睡觉或强制昏迷
    if (_hour >= MAX_GAME_HOUR) {
        _hour -= HOURS_PER_DAY_STD;
    }
}

/**
 * 增加天数，并立即触发月份/季节/年份的更替检查
 */
void GameTime::addDays(int daysToAdd) {
    _dayOfMonth += daysToAdd;
    // 增加天数后立即检查是否需要切换季节或年份
    handleDayRollover();
}

/**
 * 处理日期更替逻辑，当日期超过月底时自动切换到下一季节或年份
 */
void GameTime::handleDayRollover() {
    // 检查是否超过当月最大天数
    while (_dayOfMonth > DAYS_PER_MONTH) {
        _dayOfMonth -= DAYS_PER_MONTH;

        // 季节更替逻辑
        int nextSeasonIndex = static_cast<int>(_season) + 1;

        // 如果超过冬季(3)，重置为春季(0)并增加年份
        if (nextSeasonIndex > static_cast<int>(Season::Winter)) {
            nextSeasonIndex = static_cast<int>(Season::Spring);
            _year++;
        }
        _season = static_cast<Season>(nextSeasonIndex);
    }
}

// ==================== 字符串格式化 ====================

/**
 * 获取格式化的 12 小时制时间字符串 (如 "6:00 am")，包含凌晨时间的特殊显示处理
 */
std::string GameTime::getTimeString() const {
    std::string period = STR_TIME_AM;
    int displayHour = _hour;

    // 格式化跨天时间显示 (例如 25:00 逻辑上对应次日 1:00)
    // 但在显示时通常减去24来计算 AM/PM
    if (displayHour >= HOURS_PER_DAY_STD) {
        displayHour -= HOURS_PER_DAY_STD;
    }

    // 12小时制转换逻辑
    if (displayHour >= 12) {
        period = STR_TIME_PM;
        if (displayHour > 12) {
            displayHour -= 12;
        }
    }

    // 特殊处理：0点显示为12点
    if (displayHour == 0) {
        displayHour = 12;
    }

    int displayMinute = (_minute / 10) * 10;
    return StringUtils::format("%d:%02d %s", displayHour, displayMinute, period.c_str());
}

/**
 * 获取格式化的日期字符串 (如 "Mon. 1")，包含星期几的计算
 */
std::string GameTime::getDateString() const {
    int dayIndex = static_cast<int>(getDayOfWeek());

    // 索引边界安全检查，防止访问越界
    if (dayIndex >= 0 && dayIndex < static_cast<int>(STR_DAYS_OF_WEEK.size())) {
        return StringUtils::format("%s. %d", STR_DAYS_OF_WEEK[dayIndex].c_str(), _dayOfMonth);
    }
    return StringUtils::format("Unknown. %d", _dayOfMonth);
}

/**
 * 获取当前季节的字符串名称 (Spring, Summer, Fall, Winter)
 */
std::string GameTime::getSeasonString() const {
    switch (_season) {
    case Season::Spring: return STR_SEASON_SPRING;
    case Season::Summer: return STR_SEASON_SUMMER;
    case Season::Fall:   return STR_SEASON_FALL;
    case Season::Winter: return STR_SEASON_WINTER;
    default: return STR_UNKNOWN;
    }
}

/**
 * 获取包含年、月、日、时的完整调试信息字符串
 */
std::string GameTime::getFullString() const {
    // 拼接完整的日期时间字符串，用于调试
    return StringUtils::format("Year %d %s Day %d - %s",
        _year, getSeasonString().c_str(), _dayOfMonth, getTimeString().c_str());
}

// ==================== 辅助计算 ====================

/**
 * 根据当前日期计算今天是星期几 (假设每月1号均为周一)
 */
DayOfWeek GameTime::getDayOfWeek() const {
    // 根据每月1号为周一的规则计算星期
    // 计算公式: (日期 - 1) % 7
    int dayIndex = (_dayOfMonth - 1) % 7;
    return static_cast<DayOfWeek>(dayIndex);
}

/**
 * 比较两个时间对象是否代表同一天 (忽略具体时分)
 */
bool GameTime::isSameDay(const GameTime& other) const {
    // 比较年月日是否完全一致
    return _year == other._year &&
        _season == other._season &&
        _dayOfMonth == other._dayOfMonth;
}

/**
 * 比较两个时间对象是否代表同一天的同一小时
 */
bool GameTime::isSameHour(const GameTime& other) const {
    // 比较是否为同一天的同一小时
    return isSameDay(other) && _hour == other._hour;

}


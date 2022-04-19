#pragma once

enum class FanStatus {
    automatic,
    night,
    low,
    medium,
    high,
    timer,
    unknown
};

constexpr const char* FanStatusToString(FanStatus fanStatus) noexcept {
    switch (fanStatus)
    {
    case FanStatus::automatic: return "Automatic";
    case FanStatus::night:     return "Night";
    case FanStatus::low:       return "Low";
    case FanStatus::medium:    return "Medium";
    case FanStatus::high:      return "High";
    case FanStatus::timer:     return "Timer";
    default: return "Unknown";
    }
}

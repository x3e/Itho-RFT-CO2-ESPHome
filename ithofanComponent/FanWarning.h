#pragma once

enum class FanWarning {
    none,
    fault,
    filter,
    defrost,
    unknown
};

constexpr const char* FanWarningToString(FanWarning fanWarning) noexcept {
    switch (fanWarning)
    {
    case FanWarning::none:     return "None";
    case FanWarning::fault:    return "Fault";
    case FanWarning::filter:   return "Filter dirty";
    case FanWarning::defrost:  return "Defrost";
    default: return "Unknown";
    }
}

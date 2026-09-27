#include "core/sentinel_scanner.h"
SentinelScanner::SentinelScanner(std::string sentinel)
    : sentinel_(sentinel), pending_("")
{
}
SentinelScanner::Out SentinelScanner::feed(std::string_view chunk)
{
    std::string combined = pending_ + std::string(chunk);

    std::size_t pos = combined.find(sentinel_);
        if (pos != std::string::npos) {
        std::string safe = combined.substr(0, pos);
        pending_.clear();

        return {safe, true};
    }

        std::size_t keep = sentinel_.size() - 1;

    if (combined.size() > keep) {
        std::size_t safe_count = combined.size() - keep;

        std::string safe = combined.substr(0, safe_count);
        pending_ = combined.substr(safe_count);

        return {safe, false};
    }
        pending_ = combined;

    return {"", false};
}
SentinelScanner::Out SentinelScanner::flush()
{
    std::string safe = pending_;
    pending_.clear();

    return {safe, false};
}
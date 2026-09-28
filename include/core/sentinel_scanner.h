#ifndef SENTINEL_SCANNER_H
#define SENTINEL_SCANNER_H

#include <string>
#include <string_view>
class SentinelScanner {
public:
    explicit SentinelScanner(std::string sentinel);

    struct Out {
        std::string safe_text;
        bool sentinel_found;
    };
        Out feed(std::string_view chunk);
    Out flush();
    std::size_t pending_size() const noexcept;
private:
    std::string sentinel_;
    std::string pending_;
};
#endif
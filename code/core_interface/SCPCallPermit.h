#pragma once

template <typename T>
struct SCPCallPermit {
    private:
    SCPCallPermit() = default;
    SCPCallPermit(const SCPCallPermit&) = default;
    SCPCallPermit(SCPCallPermit&&) = default;
    SCPCallPermit& operator=(const SCPCallPermit& Other) = default;
    SCPCallPermit& operator=(SCPCallPermit&& Other) = default;
    friend T;
};
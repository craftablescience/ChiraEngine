#pragma once

namespace chira {

struct NoCopy {
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
};

struct NoMove {
    NoMove() = default;
    NoMove(NoMove&&) = delete;
    NoMove& operator=(NoMove&&) = delete;
};

struct NoCopyOrMove : public NoCopy, public NoMove {
    NoCopyOrMove() = default;
};

} // namespace chira

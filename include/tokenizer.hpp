#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "token.hpp"

struct TokenizationError {
    int line;
    int column;
    std::string cause;
};

using TokenizationResult = std::expected<std::vector<Token>, TokenizationError>;

auto Tokenize(std::string_view source) -> TokenizationResult;
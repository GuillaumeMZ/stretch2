#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "token.hpp"

struct TokenizationContext {
    unsigned long line = 0;
    unsigned long column = 0;
    std::string_view source;
    std::vector<Token> tokens;
};

struct TokenizationError {
    unsigned long line;
    unsigned long column;
    std::string cause;
};

using TokenizationResult = std::expected<std::vector<Token>, TokenizationError>;

auto Tokenize(std::string_view source) -> TokenizationResult;
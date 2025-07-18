#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "token.hpp"

struct TokenizationContext {
    unsigned long line = 1;
    unsigned long column = 1;
    std::string_view source;
    std::vector<Token> tokens;
};

struct TokenizationError {
    unsigned long line;
    unsigned long column;
    std::string cause;
};

using TokenizationResult = std::expected<std::vector<Token>, TokenizationError>;

[[nodiscard]] auto Tokenize(std::string_view source) -> TokenizationResult;
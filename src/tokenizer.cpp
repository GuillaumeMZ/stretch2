#include <cctype>
#include <map>
#include <optional>

#include "tokenizer.hpp"

auto SkipComment(TokenizationContext& context) -> void {
    if (!context.source.starts_with("//")) {
        return;
    }

    if (const auto next_newline = context.source.find_first_of('\n'); next_newline == std::string_view::npos) {
        //we're at the last line; clear source and return
        //no need to modify line or column
        context.source.remove_prefix(context.source.length());
    } else {
        context.source.remove_prefix(next_newline);
        context.line++;
        context.column = 0;
    }
}

auto SkipNewline(TokenizationContext& context) -> void {
    if (context.source.starts_with("\n")) {
        context.source.remove_prefix(1);
        context.line++;
        context.column = 0;
    } else if (context.source.starts_with("\r\n")) {
        context.source.remove_prefix(2);
        context.line++;
        context.column = 0;
    }
}

auto SkipWhitespace(TokenizationContext& context) -> void {
    if (!context.source.starts_with(" ") && !context.source.starts_with("\t")) {
        return;
    }

    context.source.remove_prefix(1);
    context.column++;
}

auto TryTokenizeOperator(TokenizationContext& context) -> void {
    //order is specific to match the longest first
    //for instance, =/= and = : if = was first, =/= would never be matched
    const std::map<std::string, TokenType> operators = {
        {",", TokenType::Comma},
        {".", TokenType::Dot},
        {">=", TokenType::GreaterOrEqual},
        {">", TokenType::Greater},
        {"<=", TokenType::SmallerOrEqual},
        {"<", TokenType::Smaller},
        {"=/=", TokenType::NotEqual},
        {"=", TokenType::Equal},
        {"[", TokenType::LBracket},
        {"(", TokenType::LParen},
        {"-", TokenType::Minus},
        {"%", TokenType::Percentage},
        {"+", TokenType::Plus},
        {"]", TokenType::RBracket},
        {")", TokenType::RParen},
        {";", TokenType::Semicolon},
        {"/", TokenType::Slash},
        {"*", TokenType::Star},
    };

    for (const auto& [key, value]: operators) {
        if (!context.source.starts_with(key)) {
            continue;
        }

        context.tokens.push_back({
            .line = context.line,
            .column = context.column,
            .type = value,
            .token_data = std::monostate()
        });

        const auto length = key.length();
        context.source.remove_prefix(length);
        context.column += length;

        break;
    }
}

auto TryTokenizeKeyword(TokenizationContext& context) -> void {
    const std::map<std::string, TokenType> keywords = {
        {"and", TokenType::And},
        {"array", TokenType::Array},
        {"bool", TokenType::Bool},
        {"break", TokenType::Break},
        {"continue", TokenType::Continue},
        {"do", TokenType::Do},
        {"else", TokenType::Else},
        {"endenum", TokenType::EndEnum},
        {"endfunc", TokenType::EndFunc},
        {"endif", TokenType::EndIf},
        {"endstruct", TokenType::EndStruct},
        {"endwhile", TokenType::EndWhile},
        {"enum", TokenType::Enum},
        {"false", TokenType::False},
        {"func", TokenType::Func},
        {"if", TokenType::If},
        {"inout", TokenType::InOut}, //inout is before in so that "inout" is tokenized as "<inout>" and not "<in> <out>"
        {"in", TokenType::In},
        {"is", TokenType::Is},
        {"nothing", TokenType::Nothing}, //same as inout and in
        {"not", TokenType::Not}, //same as inout and in
        {"number", TokenType::NumberType},
        {"or", TokenType::Or},
        {"out", TokenType::Out},
        {"read", TokenType::Read},
        {"returns", TokenType::Returns}, //same as inout and in
        {"return", TokenType::Return}, //same as inout and in
        {"string", TokenType::StringType},
        {"struct", TokenType::Struct},
        {"then", TokenType::Then},
        {"true", TokenType::True},
        {"var", TokenType::Var},
        {"while", TokenType::While},
        {"write", TokenType::Write},
    };

    for (const auto& [key, value]: keywords) {
        if (!context.source.starts_with(key)) {
            continue;
        }

        if (context.source.length() == key.length()) {
            goto add_token;
        }

        if (const char separator = context.source[key.length()]; std::isalnum(separator) || separator == '_') {
            return;
        }

    add_token:
        context.tokens.push_back({.line = context.line, .column = context.column, .type = value, .token_data = std::monostate()});
        const auto length = key.length();
        context.source.remove_prefix(length);
        context.column += length;
    }
}

auto TryTokenizeNumber(TokenizationContext& context) -> void {
    const auto IsDigit = [](const char c) {
        return isalnum(c) && !isalpha(c);
    };

    bool dot_found = false;
    std::string tokenized_number;

    while (!context.source.empty() && (IsDigit(context.source.front()) || context.source.front() == '.')) {
        if (context.source.front() == '.' && dot_found) {
            break;
        }

        if (context.source.front() == '.') {
            if (tokenized_number.empty()) {
                return; //first char is a ., so it is not a valid number
            }

            dot_found = true;
        }

        tokenized_number.push_back(context.source.front());
        context.source.remove_prefix(1);
        context.column++;
    }

    if (tokenized_number.empty()) {
        return;
    }

    context.tokens.push_back({
        .line = context.line,
        .column = context.column - tokenized_number.length(),
        .type = TokenType::Number,
        .token_data = std::stod(tokenized_number)
    });
}

auto TryTokenizeIdentifier(TokenizationContext& context) -> void {
    const auto IsCharIdentifier = [](const char c) {
        return std::isalnum(c) || c == '_';
    };

    const auto IsCharFirstIdentifier = [](const char c) {
        return std::isalpha(c) || c == '_';
    };

    std::string tokenized_identifier;

    if (context.source.empty()) {
        return;
    }

    if (!IsCharFirstIdentifier(context.source.front())) {
        return;
    }

    while (!context.source.empty() && IsCharIdentifier(context.source.front())) {
        tokenized_identifier.push_back(context.source.front());
        context.source.remove_prefix(1);
        context.column++;
    }

    context.tokens.push_back({
        .line = context.line,
        .column = context.column - tokenized_identifier.length(),
        .type = TokenType::Identifier,
        .token_data = tokenized_identifier
    });
}

//returns a TokenizationError if the string isn't terminated before a newline/eof
auto TryTokenizeString(TokenizationContext& context) -> std::optional<TokenizationError> {
    if (context.source.empty()) {
        return std::nullopt;
    }

    if (context.source.front() != '"') {
        return std::nullopt;
    }

    //consume the first "
    //use a temporary string_view in case of an error
    std::string_view temporary_source = context.source;
    temporary_source.remove_prefix(1);

    const auto first_quote = temporary_source.find_first_of('"'); //ending quote
    const auto first_newline = temporary_source.find_first_of('\n');

    if (first_quote == std::string_view::npos || (first_newline != std::string_view::npos && first_quote > first_newline)) {
        return TokenizationError{
            .line = context.line,
            .column = context.column,
            .cause = "Unterminated string"
        };
    }

    //all good - we can work on the real source now
    //consume the first "
    context.source.remove_prefix(1);
    context.column++;

    std::string tokenized_string;

    while(context.source.front() != '"') { //no need to check for empty() because we know there is another " waiting for us
        tokenized_string.push_back(context.source.front());
        context.source.remove_prefix(1);
        context.column++;
    }

    //consume the terminating "
    context.source.remove_prefix(1);
    context.column++;

    context.tokens.push_back({
        .line = context.line,
        .column = context.column - (tokenized_string.length() + 2), //+2 for the two "
        .type = TokenType::String,
        .token_data = tokenized_string
    });

    return std::nullopt;
}

auto Tokenize(const std::string_view source) -> TokenizationResult {
    TokenizationContext context { .source = source };

    while (!context.source.empty()) {
        const auto begin_line = context.line;
        const auto begin_column = context.column;

        SkipComment(context); //comments must be tokenized first
        SkipNewline(context);
        SkipWhitespace(context);
        TryTokenizeNumber(context); //numbers are tokenized before operators to avoid ambiguity with the dot operator
        TryTokenizeOperator(context);
        TryTokenizeKeyword(context);
        TryTokenizeString(context);
        TryTokenizeIdentifier(context);

        const auto end_line = context.line;
        const auto end_column = context.column;

        if (begin_line == end_line && begin_column == end_column) {
            return std::unexpected<TokenizationError>({
                .line = end_line,
                .column = end_column,
                .cause = "Unknown token " + std::string{context.source.front()}
            });
        }
    }

    return context.tokens;
}

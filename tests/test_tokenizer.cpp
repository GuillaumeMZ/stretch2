#include <catch2/catch_test_macros.hpp>

#include "token.hpp"
#include "tokenizer.hpp"

TEST_CASE("Comments are ignored") {
    const std::string source = "// this is a comment with random chars: &é'(-è_çà$*'";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 0);
}

TEST_CASE("A newline followed by a comment are both ignored") {
    const std::string source = "\n//comment";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 0);
}

TEST_CASE("An integer is tokenized correctly") {
    const std::string source = "123";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::Number);
    REQUIRE(token.data == TokenData{123.00});
}

TEST_CASE("A double is tokenized correctly") {
    const std::string source = "123.456";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::Number);
    REQUIRE(token.data == TokenData{123.456});
}

TEST_CASE("A double without a decimal part is tokenized correctly") {
    const std::string source = "123.";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::Number);
    REQUIRE(token.data == TokenData{123.0});
}

TEST_CASE("The . operator is tokenized properly") {
    const std::string source = ".";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::Dot);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("The >= operator is tokenized properly") {
    const std::string source = ">=";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::GreaterOrEqual);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("A keyword (i.e nothing) is tokenized properly") {
    const std::string source = "nothing";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::Nothing);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("An identifier is tokenized properly") {
    const std::string source = "_my_var1able";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::Identifier);
    REQUIRE(token.data == TokenData{"_my_var1able"});
}

TEST_CASE("A string is tokenized properly") {
    const std::string source = "\"hello, world !\"";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 0);
    REQUIRE(token.column == 0);
    REQUIRE(token.type == TokenType::String);
    REQUIRE(token.data == TokenData{"hello, world !"});
}

TEST_CASE("A non-terminated string generates a tokenization error") {
    const std::string source = "\"hello, world !";

    const auto result = Tokenize(source);

    REQUIRE_FALSE(result);
    REQUIRE(result.error().cause == "Unterminated string");
}

TEST_CASE("A string closed on another line generates a tokenization error") {
    const std::string source = "\"hello, world !\n\"";

    const auto result = Tokenize(source);

    REQUIRE_FALSE(result);
    REQUIRE(result.error().cause == "Unterminated string");
}

TEST_CASE("Some Stretch2 code is tokenized properly") {
    const std::string source =
        "func say_hello returns nothing is\n"
        "write \"hello, world !\"\n"
        "endfunc\n"
        "//let's say hello !\n"
        "say_hello()";

    const auto result = Tokenize(source);

    REQUIRE(result);
}
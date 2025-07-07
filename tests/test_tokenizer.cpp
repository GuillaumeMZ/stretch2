#include <catch2/catch_test_macros.hpp>

#include "token.hpp"
#include "tokenizer.hpp"

TEST_CASE("Comments are ignored") {
    const std::string source = "// this is a comment with random chars: &é'(-è_çà$*'";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 0);
}

TEST_CASE("A newline is tokenized correctly") {
    const std::string source = "\n";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::LF);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("A newline followed by a comment are both ignored") {
    const std::string source = "\n//comment";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);
}

TEST_CASE("An integer is tokenized correctly") {
    const std::string source = "123";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::Number);
    REQUIRE(token.data == TokenData{123.00});
}

TEST_CASE("A double is tokenized correctly") {
    const std::string source = "123.456";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::Number);
    REQUIRE(token.data == TokenData{123.456});
}

TEST_CASE("A double without a decimal part is tokenized correctly") {
    const std::string source = "123.";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::Number);
    REQUIRE(token.data == TokenData{123.0});
}

TEST_CASE("The . operator is tokenized properly") {
    const std::string source = ".";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::Dot);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("The >= operator is tokenized properly") {
    const std::string source = ">=";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::GreaterOrEqual);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("A keyword (i.e nothing) is tokenized properly") {
    const std::string source = "nothing";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::Nothing);
    REQUIRE(token.data == TokenData{std::monostate()});
}

TEST_CASE("An identifier is tokenized properly") {
    const std::string source = "_my_var1able";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
    REQUIRE(token.type == TokenType::Identifier);
    REQUIRE(token.data == TokenData{"_my_var1able"});
}

TEST_CASE("A string is tokenized properly") {
    const std::string source = "\"hello, world !\"";

    const auto result = Tokenize(source);

    REQUIRE(result);
    REQUIRE(result->size() == 1);

    const auto token = result->at(0);

    REQUIRE(token.line == 1);
    REQUIRE(token.column == 1);
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
    REQUIRE(result->size() == 15);

    const auto first_token = result->at(0);
    REQUIRE(first_token.line == 1);
    REQUIRE(first_token.column == 1);
    REQUIRE(first_token.type == TokenType::Func);

    const auto second_token = result->at(1);
    REQUIRE(second_token.line == 1);
    REQUIRE(second_token.column == 6);
    REQUIRE(second_token.type == TokenType::Identifier);
    REQUIRE(second_token.data == TokenData{"say_hello"});

    const auto third_token = result->at(2);
    REQUIRE(third_token.line == 1);
    REQUIRE(third_token.column == 16);
    REQUIRE(third_token.type == TokenType::Returns);

    const auto fourth_token = result->at(3);
    REQUIRE(fourth_token.line == 1);
    REQUIRE(fourth_token.column == 24);
    REQUIRE(fourth_token.type == TokenType::Nothing);

    const auto fifth_token = result->at(4);
    REQUIRE(fifth_token.line == 1);
    REQUIRE(fifth_token.column == 32);
    REQUIRE(fifth_token.type == TokenType::Is);

    const auto sixth_token = result->at(5);
    REQUIRE(sixth_token.line == 1);
    REQUIRE(sixth_token.column == 34);
    REQUIRE(sixth_token.type == TokenType::LF);

    const auto seventh_token = result->at(6);
    REQUIRE(seventh_token.line == 2);
    REQUIRE(seventh_token.column == 1);
    REQUIRE(seventh_token.type == TokenType::Write);

    const auto eighth_token = result->at(7);
    REQUIRE(eighth_token.line == 2);
    REQUIRE(eighth_token.column == 7);
    REQUIRE(eighth_token.type == TokenType::String);
    REQUIRE(eighth_token.data == TokenData{"hello, world !"});

    const auto ninth_token = result->at(8);
    REQUIRE(ninth_token.line == 2);
    REQUIRE(ninth_token.column == 23);
    REQUIRE(ninth_token.type == TokenType::LF);

    const auto tenth_token = result->at(9);
    REQUIRE(tenth_token.line == 3);
    REQUIRE(tenth_token.column == 1);
    REQUIRE(tenth_token.type == TokenType::EndFunc);

    const auto eleventh_token = result->at(10);
    REQUIRE(eleventh_token.line == 3);
    REQUIRE(eleventh_token.column == 8);
    REQUIRE(eleventh_token.type == TokenType::LF);

    const auto twelfth_token = result->at(11);
    REQUIRE(twelfth_token.line == 4);
    REQUIRE(twelfth_token.column == 20);
    REQUIRE(twelfth_token.type == TokenType::LF);

    const auto thirteenth_token = result->at(12);
    REQUIRE(thirteenth_token.line == 5);
    REQUIRE(thirteenth_token.column == 1);
    REQUIRE(thirteenth_token.type == TokenType::Identifier);
    REQUIRE(thirteenth_token.data == TokenData{"say_hello"});

    const auto fourteenth_token = result->at(13);
    REQUIRE(fourteenth_token.line == 5);
    REQUIRE(fourteenth_token.column == 10);
    REQUIRE(fourteenth_token.type == TokenType::LParen);

    const auto fifteenth_token = result->at(14);
    REQUIRE(fifteenth_token.line == 5);
    REQUIRE(fifteenth_token.column == 11);
    REQUIRE(fifteenth_token.type == TokenType::RParen);
}
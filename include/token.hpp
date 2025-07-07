#pragma once

#include <string>
#include <variant>

enum class TokenType {
    //comments are ignored
    And,
    Array,
    // there is no arrow token because of the ambiguity with "less than minus"
    Bool,
    Break,
    Comma,
    Continue,
    Do,
    Dot,
    Else,
    EndEnum,
    EndFunc,
    EndIf,
    EndStruct,
    EndWhile,
    Enum,
    Equal,
    False,
    Func,
    Greater,
    GreaterOrEqual,
    Identifier,
    If,
    In,
    InOut,
    Is,
    LBracket, //square
    LF,
    LParen,
    Minus,
    Not,
    NotEqual,
    Nothing,
    Number,
    NumberType,
    Or,
    Out,
    Percentage,
    Plus,
    RBracket, //square
    Read,
    Return,
    Returns,
    RParen,
    Semicolon,
    Slash,
    Smaller,
    SmallerOrEqual,
    Star,
    String,
    StringType,
    Struct,
    Then,
    True,
    Var,
    While,
    Write,
};

using TokenData = std::variant<
    std::monostate,
    double,
    std::string
>;

struct Token {
    unsigned long line;
    unsigned long column;
    TokenType type;
    TokenData data;
};
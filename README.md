# Stretch2

A programming language for beginners.

## Language reference & examples

Look at [the reference](./REFERENCE.md) to learn how to use the Stretch2 programming language.

## Source code organization

This project is divided in four parts:

- The lexer (`include/token.hpp`, `include/tokenizer.hpp`, `src/tokenizer.cpp`)
- The parser
- The bytecode generator
- The virtual machine

Tests are in the `tests` directory and use the [Catch2 test framework](https://github.com/catchorg/Catch2).

## Known issues

The tokenizer currently stops its work at the first error it encounters.

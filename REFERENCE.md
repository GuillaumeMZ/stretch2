# The Stretch programming language reference

If a feature is not mentioned here, then it DOES NOT exist (or maybe I forgot something; in that case, open an issue).

## Comments

```stretch
//This is a single line comment.
//There are no other comment types than this one.
```

## Identifiers

```stretch
//An identifier must start with a letter and can only contain letters, numbers, and underscores (`_`).

my_variable //valid
1nteger //invalid, starts with a number
int3ger //valid
_temporary //valid
program$ //invalid, $ is not a valid character for identifiers
```

## Variables

```stretch
//Declaration:
var name: type <- value //specifying the type is MANDATORY

//Reassignment:
name <- new_value

//Variables can be declared without being initialized
var name: type

//however, using a non-initialized variable is an ERROR (at compile-time)
//so it is recommended to always initialize a variable when declaring it

//Shadowing is not allowed:
var my_variable: number <- 12
var my_variable: bool <- true //compilation error
```

## Primitive types

```stretch
//Primitive:
var my_number: number <- 42
var my_decimal_number: number <- 42.420
var my_string: string <- "hello, world !" // ' CANNOT be used as a delimiter
var my_bool: bool <- true
```

## Mathematical operations

```stretch
var left: number <- 123
var right: number <- 456.789
var result: number <- 0

result <- left + right //addition
result <- left - right //subtraction
result <- left * right //multiplication
result <- left / right //division (dividing by 0 generates a runtime error)
result <- left % right //modulus
result <- -result //minus
```

## Comparison operations

```stretch
var left: number <- 123
var right: number <- 456.789
var result: bool <- false

result <- left = right //equality check
result <- left =/= right //inequality check
result <- left < right //"smaller than" check
result <- left <= right //"smaller or equal" check
result <- left > right //"greater than" check
result <- left >= right //"greater or equal" check
```

## Logical operators

```stretch
var left: bool <- false
var right: bool <- true
var result: bool <- false

result <- left and right //logical and
result <- left or right //logical or
result <- not result //logical not
```

## Other operators

```stretch
//Parentheses allow to specify the precedence of an operation:
var result: number <- (2+3)*4 //20
```

## Control structures

### Conditions (if)

```stretch
if condition then
    //do something...
endif
```

```stretch
if condition then
    //...
else
    //...
endif
```

```stretch
if condition1 then
    //...
else if condition2 then //as many else ifs as you want
    //...
else //not mandatory
    //...
endif
```

### Loops (while)

```stretch
while condition do
    if condition then
        continue //jump straight to the next iteration
    endif

    if condition then break endif //break will prematurely exit the loop

    //some stuff...
endwhile
```

## Structures

```stretch
//Definition:
struct name is
    field1: type1
    field2: type2
    //...
endstruct

//Instantiation:
var instance: name
instance.field1 <- value
instance.field2 <- value
//all fields must be initialized before the instance can be used

//accessing fields is done with the . operator:
var result: number <- instance.field1 + instance.field2
```

## Enumerations

```stretch
enum name is
    variant1
    variant2
    //...
endenum

//Instantiation:
var instance: name <- name.variantX
//or
var instance: name <- variantX

//Comparing enums is possible with = and =/= :
if instance = name.variantX then
    //...
endif

//or

if instance =/= variantX then
    //...
endif
```

## Arrays

```stretch
var my_array: number array <- [1, 2, 3, 4, 5]
var first_number: number <- my_array[0] //indexing starts at 0
//Accessing an invalid cell of an array generates a runtime error:
my_array[100] //runtime error !
```

## Reading/writing from/to the terminal

```stretch
//Using the read instruction:

//Only these three types can be read from the terminal
var read_string: string
var read_number: number
var read_bool: bool

read read_string, read_number, read_bool
```

```stretch
//Using the write instruction:

//The types that can be written are:
- number
- bool
- string
- enums
- structs, as long as all fields are writable
- arrays, as long as they contain a writable type

var name: string <- "John"
var age: number <- 22
write "My name is ", name, ", and I am ", age, " years old."
```

## Functions

```stretch
//Procedure (no parameter, void return type (named "nothing" in Stretch))
func name returns nothing is
    print "hello world !"
endfunc
```

```stretch
//No parameter, returning something:
func name returns number is
    return 42
endfunc

//Invocation
var forty_two: number <- name()
```

```stretch
//With a single parameter and no returned value:
func say_my_name(name: string) returns nothing is
    print name
endfunc

//Invocation (function call):
say_my_name("Heisenberg")
```

```stretch
//With in, inout and out parameters:
func name(in param1: type1, inout param2: type2, out param3: type3, param4: type4) returns nothing is
    //not reading from an in parameter is an error
    //writing to an in parameter is an error
    print param1

    //not reading from an in parameter is an error
    //not writing to an in parameter is an error
    param2 <- some_value
    print param2

    //reading from an out parameter is an error
    //not writing to an in parameter is an error
    param3 <- some_value

    //if nothing is specified, then a parameter is in by default
    //in this example, param4 is in
```

## Appendixes

### Keywords list

`array` `bool` `break` `continue` `do` `else` `endenum` `endfunc` `endif` `endstruct` `endwhile` `enum` `false` `func` `if` `is` `number` `read` `return` `returns` `string` `struct` `then` `true` `var` `while` `write`

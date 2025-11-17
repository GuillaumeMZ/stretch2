type t = statement list

and statement =
| Print of expression

and expression =
| Number of int
| Negate of expression
| Parenthesized of expression
| Addition of expression * expression
| Subtraction of expression * expression
| Multiplication of expression * expression
| Division of expression * expression
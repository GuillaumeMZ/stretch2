open Ast

let rec evaluate_expression = function
| Number n -> n
| Negate e -> -(evaluate_expression e)
| Parenthesized e -> evaluate_expression e
| Addition (lhs, rhs) -> evaluate_expression lhs + evaluate_expression rhs
| Subtraction (lhs, rhs) -> evaluate_expression lhs - evaluate_expression rhs
| Multiplication (lhs, rhs) -> evaluate_expression lhs * evaluate_expression rhs
| Division (lhs, rhs) -> evaluate_expression lhs / evaluate_expression rhs

let evaluate_statement = function
| Print expression -> print_int (evaluate_expression expression); print_newline ()

let evaluate = List.iter evaluate_statement
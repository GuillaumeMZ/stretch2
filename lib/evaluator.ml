open Ast

module IdentifierMap = Map.Make(String)

let rec evaluate_expression context = function
| Number n -> n
| Negate e -> -(evaluate_expression context e)
| Parenthesized e -> evaluate_expression context e
| Addition (lhs, rhs) -> evaluate_expression context lhs + evaluate_expression context rhs
| Subtraction (lhs, rhs) -> evaluate_expression context lhs - evaluate_expression context rhs
| Multiplication (lhs, rhs) -> evaluate_expression context lhs * evaluate_expression context rhs
| Division (lhs, rhs) -> evaluate_expression context lhs / evaluate_expression context rhs
| Ident i -> evaluate_expression context (IdentifierMap.find i context)

let evaluate_statement context = function
| Print expression -> print_int (evaluate_expression context expression); print_newline (); context
| Var (name, value) -> IdentifierMap.add name value context

let evaluate statements =
  let rec evaluate' context = function
  | [] -> ()
  | hd :: tl -> let updated_context = evaluate_statement context hd in evaluate' updated_context tl
in evaluate' IdentifierMap.empty statements
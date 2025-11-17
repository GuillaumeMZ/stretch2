%{
  open Ast
%}

%token PRINT
%token LPAREN
%token RPAREN
%token SEMICOLON
%token EOF
%token STAR SLASH
%token PLUS MINUS
%token UNARYMINUS
%token <int> NUMBER

%left PLUS MINUS
%left STAR SLASH
%nonassoc UNARYMINUS

%start <Ast.t> parse
%%

let parse := 
  prints=print_instruction*; EOF; { prints }

let print_instruction :=
  PRINT; LPAREN; e=expression; RPAREN; SEMICOLON; { Print e }

let expression :=
  | LPAREN; e=expression; RPAREN; { Parenthesized e }
  | MINUS; e=expression; %prec UNARYMINUS { Negate e } 
  | lhs=expression; STAR; rhs=expression; { Multiplication (lhs, rhs) }
  | lhs=expression; SLASH; rhs=expression; { Division (lhs, rhs) }
  | lhs=expression; PLUS; rhs=expression; { Addition (lhs, rhs) }
  | lhs=expression; MINUS; rhs=expression; { Subtraction (lhs, rhs) }
  | n=NUMBER; { Number n }
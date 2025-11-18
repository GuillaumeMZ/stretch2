%{
  open Ast
%}

%token ARROW
%token EOF
%token <string> IDENT
%token LPAREN
%token MINUS
%token <int> NUMBER
%token PLUS 
%token PRINT
%token RPAREN
%token SEMICOLON
%token SLASH
%token STAR
%token UNARYMINUS
%token VAR

%left PLUS MINUS
%left STAR SLASH
%nonassoc UNARYMINUS

%start <Ast.t> parse
%%

let parse := 
  prints=instruction*; EOF; { prints }

let instruction :=
  | PRINT; LPAREN; e=expression; RPAREN; SEMICOLON; { Print e }
  | VAR; i=IDENT; ARROW; e=expression; SEMICOLON; { Var (i, e) }

let expression :=
  | i=IDENT; { Ident i }
  | LPAREN; e=expression; RPAREN; { Parenthesized e }
  | MINUS; e=expression; %prec UNARYMINUS { Negate e } 
  | lhs=expression; STAR; rhs=expression; { Multiplication (lhs, rhs) }
  | lhs=expression; SLASH; rhs=expression; { Division (lhs, rhs) }
  | lhs=expression; PLUS; rhs=expression; { Addition (lhs, rhs) }
  | lhs=expression; MINUS; rhs=expression; { Subtraction (lhs, rhs) }
  | n=NUMBER; { Number n }
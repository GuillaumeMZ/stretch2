{
  open Lexing
  open Parser
}

let white = [' ' '\t']+
let newline = '\n' | '\r' | "\r\n"

let semicolon = ';'

let number = ['0'-'9']+

let ident = ['a'-'z' 'A'-'Z' '_'] ['a'-'z' 'A'-'Z' '_' '0'-'9']*

rule read = parse
| white { read lexbuf }
| newline { new_line lexbuf; read lexbuf }
| "print" { PRINT }
| "var" { VAR }
| "<-" { ARROW }
| "(" { LPAREN }
| ")" { RPAREN }
| ";" { SEMICOLON }
| "+" { PLUS }
| "-" { MINUS }
| "/" { SLASH }
| "*" { STAR }
| number as n { NUMBER (int_of_string n) }
| ident as i { IDENT i }
| eof { EOF }
| _ as c { failwith (Printf.sprintf "Unexpected char: %C" c) }
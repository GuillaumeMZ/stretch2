{
  open Lexing
  open Parser
}

let white = [' ' '\t']+
let newline = '\n' | '\r' | "\r\n"

let semicolon = ';'

let number = ['0'-'9']+

rule read = parse
| white { read lexbuf }
| newline { new_line lexbuf; read lexbuf }
| "print" { PRINT }
| "(" { LPAREN }
| ")" { RPAREN }
| ";" { SEMICOLON }
| "+" { PLUS }
| "-" { MINUS }
| "/" { SLASH }
| "*" { STAR }
| number as n { NUMBER (int_of_string n) }
| eof { EOF }
| _ as c { failwith (Printf.sprintf "Unexpected char: %C" c) }
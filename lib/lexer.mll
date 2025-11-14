{
    open Lexing
    open Parser
}

let white = [' ' '\t']+
let newline = '\n' | '\r' | "\r\n"

let semicolon = ';'

let digit = ['0'-'9']
let sign = ['+' '-']
let number = sign? digit+

let identifier = ['a'-'z' 'A'-'Z' '_'] ['a'-'z' 'A'-'Z' '0'-'9' '_']*

rule read = parse
| white { read lexbuf }
| newline { new_line lexbuf; read lexbuf }
| "print" { PRINT }
| "(" { LPAREN }
| ")" { RPAREN }
| ";" { SEMICOLON }
| number { NUMBER (Lexing.lexeme lexbuf |> int_of_string) }
| eof { EOF }
| _ as c { failwith (Printf.sprintf "Unexpected char: %C" c) }
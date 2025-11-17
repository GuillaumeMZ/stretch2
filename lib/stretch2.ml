let ast_of_in_channel (in_channel: In_channel.t): Ast.t =
  Parser.parse Lexer.read (Lexing.from_channel in_channel)

let ast_of_string (string: string): Ast.t =
  Parser.parse Lexer.read (Lexing.from_string string) 

let evaluate = Evaluator.evaluate
let parse_stretch_source_code (in_channel: In_channel.t): int list =
  Parser.stretch Lexer.read (Lexing.from_channel in_channel)
%token PRINT
%token LPAREN
%token RPAREN
%token SEMICOLON
%token EOF
%token <int> NUMBER
%start <int list> stretch
%%

stretch: prints=print_instruction*; EOF { prints } ;

print_instruction: PRINT; LPAREN; i=NUMBER; RPAREN; SEMICOLON { i } ;
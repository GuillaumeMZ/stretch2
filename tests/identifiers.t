  $ cat > identifiers.stretch << EOF
  > var abc <- 123;
  > var def <- 456;
  > print(abc+(def*2));
  > EOF

  $ ../bin/stretchc.exe identifiers.stretch
  1035

  $ cat > nonexisting_identifier.stretch << EOF
  > var abc <- 123;
  > print(abcd);
  > EOF

  $ ../bin/stretchc.exe nonexisting_identifier.stretch
  Fatal error: exception Not_found
  [2]

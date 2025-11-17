  $ cat > arithmetic.stretch << EOF
  > print(1);
  > print(2+3);
  > print(2+3*4);
  > print((2+3)*4);
  > print((2+3)*(2+3));
  > print(-1);
  > print(--1);
  > print(-(-1));
  > print(1--1);
  > EOF

  $ ../bin/stretchc.exe arithmetic.stretch
  1
  5
  14
  20
  25
  -1
  1
  1
  2

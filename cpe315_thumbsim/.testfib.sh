make
echo ----- test -----
make run > my_fib.out
diff fib.out my_fib.out

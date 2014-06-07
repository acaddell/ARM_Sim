make
#echo "----- test (old) -----"
#make run > my_fib.out
#./thumbsim -c 256 -i -d -f inputs/fib.sim > my_fib.out
#diff fib.out my_fib.out

echo "----- test (new) -----"
./thumbsim -s -c 256 -i -d -f inputs/fib.sim > my_fib.out
diff fib.completeoutput my_fib.out

lab=lab1

./autograder/source/lc3tools/build/bin/lab1_private /autograder/submission/$lab.asm > /autograder/submission/private.out
./autograder/source/lc3tools/build/bin/lab1_public /autograder/submission/$lab.asm > /autograder/submission/public.out

python3 run_tests.py $lab
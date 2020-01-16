all:
	@make -s squeue_test

./squeue_test: ./squeue_test.c ../squeue.c
	@gcc -Wall -Wextra -O3 -I../ $^ -o $@

debug:
	@make -s distclean
	@make -s all
	@./squeue_test

distclean:
	@rm -rf ./squeue_test

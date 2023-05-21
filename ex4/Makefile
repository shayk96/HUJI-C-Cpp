#.PHONY: all, clean
#
#all: libhashmap.a libhashmap_tests.a
#
#libhashmap.a :hashmap.o vector.o pair.o
#	ar rcs libhashmap.a hashmap.o vector.o pair.o
#
#libhashmap_tests.a: test_suite.o hash_funcs.h test_pairs.h hashmap.o
#	ar rcs libhashmap_tests.a test_suite.o hashmap.o
#
#hashmap.o: hashmap.c hashmap.h vector.c vector.h pair.c pair.h
#	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 hashmap.c
#
#pair.o: pair.c pair.h
#	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 pair.c
#
#test_suite.o: test_suite.c test_suite.h hash_funcs.h test_pairs.h hashmap.h
#	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 test_suite.c
#
#
#clean:
#	rm *.o *.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
TEST_SOURCES = $(wildcard tests/*.check)
SRCS = $(wildcard s21_*.c) extra.c auxiliary_functions.c decimal.c
OBJS = $(SRCS:.c=.o)
LIB_NAME = s21_decimal.a
TEST_MAIN_SRC = s21_main_test.c
EXEC = test

LIBS = -lcheck
OPEN = open
OS := $(shell uname -s)
ifeq ($(OS),Linux)
LIBS += -lsubunit -lm -lrt -lpthread 
OPEN = xdg-open
endif



all: s21_decimal.a

s21_decimal.a: $(OBJS)
	ar rcs $(LIB_NAME) $^
	ranlib s21_decimal.a
	rm -rf *.o

test: $(LIB_NAME)
	checkmk clean_mode=1 $(TEST_SOURCES) > s21_main_test.c
	$(CC) $(CFLAGS) s21_main_test.c -o $(EXEC) -L. $(LIB_NAME) $(LIBS)
	rm -rf *.o
	./$(EXEC)

gcov_report: clean checkmk s21_decimal.a
	mkdir -p coverage
	$(CC) $(CFLAGS) -o $(EXEC) $(TEST_MAIN_SRC) -L. $(LIB_NAME) $(LIBS) $(GCOV_FLAGS) 
	./$(EXEC)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	rm -rf *.gcda *.gcno *.gcov *.info 
	$(OPEN) coverage/index.html
	

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC) $(LIB_NAME) $(TEST_MAIN_SRC)
	rm -rf coverage

checkmk:
	checkmk clean_mode=1 $(TEST_SOURCES) > s21_main_test.c


.PHONY: all clean test gcov_report checkmk
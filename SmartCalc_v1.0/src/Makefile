CC=gcc
GTK=`pkg-config --cflags --libs gtk4`
FLAGS=-Wall -Wextra -Werror -std=c11
REP=-fprofile-arcs -ftest-coverage

all: clean test

install:
	brew install gtk4
	$(CC) $(GTK) gtk.c smartcalc.c -o smartcalc
	./smartcalc

uninstall:
	rm -rf smartcalc

dvi:
	open info.html

dist:
	tar -c -v -f archive.tar *.c *.h Makefile

test:
	$(CC) test.c smartcalc.c -o test -lcheck
	./test

gcov_report:
	$(CC) $(REP) test.c smartcalc.c -o test -lcheck
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

open:
	open report/index.html

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h
	make test
	CK_FORK=no leaks --atExit -- ./test
	make clean

clean:
	rm -rf *.out *.gcda *.gcno *.info *.tar test smartcalc
	rm -rf report/
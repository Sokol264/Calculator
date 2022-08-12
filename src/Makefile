SRCS = 	calculation.c \
		polishManager.c \
		stackManager.c
		
PACKAGE   = smart_calc
VERSION   = ` date +"%Y.%m.%d" `
ARCHIVE   = $(PACKAGE)-$(VERSION)

OBJS = ${SRCS:.c=.o}
CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: clean smart_calc test

install: uninstall smart_calc
	@mkdir "/Users/$(USER)/Desktop/SmartCalc/"
	@install ./smartCalc builder* "/Users/$(USER)/Desktop/SmartCalc/"

uninstall:
	@rm -rf "/Users/$(USER)/Desktop/SmartCalc/"
	
dist:
	@tar -zcf $(ARCHIVE).tar *.c *.h Makefile *.glade

smart_calc:
	${CC} -c ${SRCS} 
	${CC} graphics.c ${SRCS} `pkg-config --cflags --libs gtk4` -o smartCalc
	
test: smart_calc unit_test

dvi:
	open dvi.html

unit_test:
	${CC} -c s21_smart_calc-test.c
	${CC} s21_smart_calc-test.o ${OBJS} -lcheck -o test
	./test

gcov_report: smart_calc
	@gcc --coverage s21_smart_calc-test.c -lcheck ${SRCS} -o unit-test
	@./unit-test
	@lcov -t "test" -o test.info -c -d .
	@genhtml -o report test.info
	
clean:
	@/bin/rm -rf *.o *.a test unit-test *.gcno *gcda report *.info smartCalc

checks: cppcheck leaks style

style:
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem . *.c

leaks:
	CK_FORK=no leaks --atExit -- ./test

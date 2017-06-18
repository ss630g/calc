#cs3350 cal.c
# to make chages to a K & R format

all: calc

calc: calc.c
		gcc calc.c

clean:
	rm -f lab1
	rm -f *.o

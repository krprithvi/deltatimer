all: driver timerprocess

driver: driver.c
timerprocess: timerprocess.c

.PHONY : clean

clean:
	rm driver timerprocess

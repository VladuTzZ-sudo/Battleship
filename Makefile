build:
	gcc -g joc.c -lncurses -o battleship
run:
	./battleship text.txt
clean:
	rm ./battleship

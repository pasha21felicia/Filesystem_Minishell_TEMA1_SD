build: 
	gcc minifilesystem.c file.h -o fsminishell -std=c99

run:
	./fsminishell
clean:
	rm fsminishell
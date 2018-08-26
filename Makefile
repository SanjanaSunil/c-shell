shell: main.c prompt.c interpret.c builtin.c
	gcc -o shell main.c prompt.c interpret.c builtin.c -I.

clean:
	rm -rf shell
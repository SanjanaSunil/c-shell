shell: main.c prompt.c interpret.c builtin_commands.c system_commands.c
	gcc -o shell main.c prompt.c interpret.c builtin_commands.c system_commands.c -I.

clean:
	rm -rf shell
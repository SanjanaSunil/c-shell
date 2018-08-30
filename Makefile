shell: main.c prompt.c interpret.c builtin_commands.c system_commands.c pinfo.c
	gcc -o shell main.c prompt.c interpret.c builtin_commands.c system_commands.c pinfo.c -I.

clean:
	rm -rf shell
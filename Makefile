shell: main.c prompt.c interpret.c builtin_commands.c system_commands.c user_commands.c bg.c signals.c
	gcc -o shell main.c prompt.c interpret.c builtin_commands.c system_commands.c bg.c user_commands.c signals.c -I.

clean:
	rm -rf shell
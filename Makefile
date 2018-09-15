shell: main.c prompt.c interpret.c builtin_commands.c system_commands.c user_commands.c bg.c 
	gcc -o shell main.c prompt.c interpret.c builtin_commands.c system_commands.c bg.c user_commands.c -I.

clean:
	rm -rf shell
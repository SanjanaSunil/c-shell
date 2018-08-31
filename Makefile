shell: main.c prompt.c interpret.c builtin_commands.c system_commands.c pinfo.c bg.c reminder.c clock_cmd.c
	gcc -o shell main.c prompt.c interpret.c builtin_commands.c system_commands.c pinfo.c bg.c reminder.c clock_cmd.c -I.

clean:
	rm -rf shell
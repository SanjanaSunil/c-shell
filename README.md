# C Shell

## Usage

To run the shell, follow these steps:

```
    make
    ./shell
```

## Features

- **main.c** - File from which the shell is invoked.

- **prompt.c** - Displays prompt consisting of user, system and current directory details. The directory from which the shell is invoked is the home shell.

- **interpret.c** - Reads the command and executes it. Support for background processes exists. 

- **builtin.c** - cd, ls, echo and pwd commands are implemented. ls supports -l and -a flags. 

- **system_commands.c** - System commands are executed.

- **bg.c** - All background process are stored and checked regularly whether they've exited or not.

- **pinfo.c** - _pinfo_ command is implemented that displays pid, process status, virtual memory and executable path of either the currently running process or any given pid.

- **reminder.c** - _remindme_ command that reminds a custom message after a given number of seconds elapse. 

- **clock.c** - _clock_ command that displays dynamic date and time in the shell with a fixed time interval.
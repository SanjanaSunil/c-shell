# C Shell

## Usage

To run the shell, follow these steps:

```
    make
    ./shell
```

## Features

* Builtin commands: ```ls```, ```cd```, ```pwd```, ```echo```
* Support for background and foreground processes
* Redirection and piping support
* Signal handling
* Additional commands

#### Additional commands:

* ```clock -t [time interval]```
Dynamic clock where the time interval is in seconds.

* ```pinfo``` 
Displays currect processes and their information.

* ```remindme [seconds] [message]```
Displays a given message after a given time interval.

* ```setenv [var] [value]```
Sets an environmental variable to a given value.

* ```unsetenv [var]```
Unsets a given environmental variable.

* ```overkill```
Kills all running processes.

* ```jobs```
Lists currently running processes.

* ```kjob [job number] [signal number]```
Sends a signal to a specified job.

* ```fg [job number]```
Puts a specified job in the foreground.

* ```bg [job number]```
Changes a stopped job number to a running background job.

# Small Shell

## Overview
This project is a simple shell implemented in C that provides a command-line interface for executing commands. It supports built-in commands, process execution, input/output redirection, background processes, and custom signal handling.

## Features
1. **Command Prompt:**
   - Displays a prompt for user input.

2. **Handling Blank Lines and Comments:**
   - Ignores empty input.
   - Ignores lines beginning with `#` (treated as comments).

3. **Built-in Commands:**
   - `exit`: Terminates the shell.
   - `cd [directory]`: Changes the current working directory.
   - `status`: Displays the exit status or termination signal of the last foreground process.

4. **Executing Other Commands:**
   - Uses functions from the `exec()` family to create new processes for executing external commands.

5. **Input and Output Redirection:**
   - Supports `>` for redirecting standard output to a file.
   - Supports `<` for redirecting standard input from a file.

6. **Foreground and Background Processes:**
   - Allows commands to run in the background by appending `&` to the command.
   - Foreground processes run normally and block further input until completion.

7. **Signal Handling:**
   - **SIGINT (Ctrl+C):** Custom handler to manage termination of foreground processes.
   - **SIGTSTP (Ctrl+Z):** Custom handler to toggle between normal mode and foreground-only mode.

## Compilation and Execution
### Compilation:
Use `gcc` to compile the program:
```sh
gcc -o my_shell my_shell.c
```

### Running the Shell:
Execute the compiled program:
```sh
./my_shell
```

## Usage Examples
- Run a command:
  ```sh
  ls -l
  ```
- Run a command in the background:
  ```sh
  sleep 10 &
  ```
- Redirect output to a file:
  ```sh
  ls > output.txt
  ```
- Redirect input from a file:
  ```sh
  sort < data.txt
  ```
- Change directory:
  ```sh
  cd /home/user
  ```
- Check the status of the last process:
  ```sh
  status
  ```
- Exit the shell:
  ```sh
  exit
  ```

## Notes
- Background processes print their process ID when started.
- In foreground-only mode (triggered by SIGTSTP), background execution is disabled.
- The `status` command reports exit codes or termination signals of the last foreground process.


## Author
Natalie Dettmer



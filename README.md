# DashShell

A small Unix shell written in C++20. It reads commands with GNU Readline, runs built-in commands itself, and starts everything else with `fork` and `execvp`. It handles pipes, I/O redirection, background jobs, command history, tab completion, and Ctrl-C / Ctrl-Z for foreground processes.

```
     _           _
  __| | __ _ ___| |__
 / _` |/ _` / __| '_ \
| (_| | (_| \__ \ | | |
 \__,_|\__,_|___/_| |_|
```

> **Platform:** macOS. `pinfo` uses macOS-only APIs (`libproc`, Mach `task_info`), and the Makefile points at Homebrew's Readline under `/opt/homebrew`.

## Features

- **Prompt** in the form `user@host:path > `, where the directory the shell was started from shows as `~`.
- **Built-in commands:** `cd`, `pwd`, `echo`, `ls`, `pinfo`, `search`, `history`, `exit`.
- **External commands** run through `execvp`, so anything on your `PATH` works.
- **Background jobs:** end a command with `&` and the shell prints its PID and returns to the prompt.
- **Pipes:** chain any number of commands with `|`. Built-ins work inside a pipeline too.
- **Redirection:** `<` for input, `>` to overwrite output, `>>` to append.
- **Command lists:** run several commands in one line, separated by `;`.
- **History:** the last 20 commands are saved to `.myshell_history` and reloaded on start. Up and down arrows walk through them.
- **Tab completion:** the first word completes against programs on `PATH`; later words complete against files in the current directory.
- **Signals:** Ctrl-C interrupts and Ctrl-Z stops the foreground process. At an empty prompt, Ctrl-C just clears the line. Ctrl-D exits.

## Requirements

- macOS with a C++20 compiler (`g++` or Apple Clang)
- GNU Readline from Homebrew:

  ```sh
  brew install readline
  ```

## Build and run

```sh
make          # builds bin/myshell
make run      # builds, then starts the shell
make clean    # removes obj/ and bin/
```

Or start the binary directly:

```sh
./bin/myshell
```

If Readline lives somewhere other than `/opt/homebrew/opt/readline` (for example on an Intel Mac, where it is under `/usr/local/opt/readline`), update the `-I` and `-L` paths in the `Makefile`.

## Usage

### Built-in commands

| Command | What it does |
| --- | --- |
| `cd [dir]` | Change directory. No argument or `~` goes to `$HOME`; `-` goes back to the previous directory. |
| `pwd` | Print the current directory. |
| `echo [args...]` | Print the arguments separated by spaces. |
| `ls [-a] [-l] [paths...]` | List files. `-a` includes hidden files and `-l` shows permissions, links, owner, group, size and modified time. Flags can be combined (`-la`). |
| `pinfo [pid]` | Show a process's PID, status (`+` marks the foreground group), resident memory and executable path. With no PID it describes the shell itself. |
| `search <name>` | Recursively look for a file or folder named `<name>` under the current directory. Prints `True` or `False`. |
| `history [n]` | Show the last `n` commands (10 by default, up to 20 are kept). |
| `exit` | Quit the shell. |

### Examples

```sh
# Pipes and redirection
cat notes.txt | grep todo | wc -l
sort < names.txt > sorted.txt
echo "one more line" >> log.txt

# Several commands on one line
cd src ; ls -l ; cd -

# Background job
sleep 30 &

# Process info for a given PID
pinfo 28540
```

`pinfo` output looks like this:

```
pid -- 28540
Process Status -- Sleeping+
memory -- 999424 bytes (Resident Memory)
Executable Path -- ~/code/IIIT/AOS/AS-2/bin/myshell
```

## Project structure

```
dashShell/
├── Makefile          # Build rules (also moves stray .cpp/.h files into src/ and lib/)
├── lib/              # Headers
├── src/
│   ├── shell.cpp     # Entry point: banner, Readline loop, history, splits on ';'
│   ├── parser.cpp    # Splits a line on '|' and pulls out <, >, >> redirections
│   ├── commands.cpp  # Built-ins: cd, pwd, echo, and dispatch to ls/pinfo/search
│   ├── fg.cpp        # Runs external commands in the foreground or background
│   ├── io.cpp        # Runs a single command with input/output redirection
│   ├── pipes.cpp     # Runs a pipeline of commands connected by pipes
│   ├── ls.cpp        # The ls built-in
│   ├── pinfo.cpp     # The pinfo built-in (macOS libproc / Mach APIs)
│   ├── search.cpp    # The recursive search built-in
│   ├── auto.cpp      # Tab completion for commands and file names
│   ├── signal.cpp    # Ctrl-C / Ctrl-Z handling for the foreground process
│   └── user.cpp      # Builds the user@host:path prompt
└── test/
    └── preShell.cpp  # Early single-file prototype of the shell
```

Build output goes to `obj/` (object files) and `bin/myshell` (the executable).

## How it works

1. `shell.cpp` prints the prompt and reads a line with Readline, saving it to history.
2. The line is split on `;`, and each part goes to the parser.
3. `Parser` splits the part on `|` into commands and records any `<`, `>` or `>>` file for each one.
4. A single command without redirection runs as a built-in or through `fork` + `execvp`. A command with redirection is handed to `IO`, which sets up the file descriptors with `dup2` before calling `execvp`. Two or more commands go to `Pipes`, which connects them with `pipe` and `dup2`.
5. While a foreground child runs, its PID is kept in `killpid` so the signal handler can pass Ctrl-C or Ctrl-Z on to it.

## Limitations

- Arguments are split on spaces and tabs only, so quotes and escapes are not supported (`echo "a b"` prints the quotes).
- A single command with redirection always runs the program from `PATH`, so built-ins are skipped there: `ls > f` uses the system `ls`, and `pinfo > f` fails. Inside a pipeline built-ins do work, so `pinfo | cat > output.txt` is a workaround.
- There are no `jobs`, `fg` or `bg` commands to manage background processes after they start.
- The build and `pinfo` are macOS-only.

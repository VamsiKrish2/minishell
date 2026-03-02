🐧 MiniShell – Linux Internals Project
📌 About the Project

MiniShell is a custom Linux command-line interpreter developed in C that replicates basic functionality of a Unix shell. The program accepts user commands, parses input, creates child processes, and executes commands using Linux system calls.

This project demonstrates core Linux Internals concepts including process creation, command execution, and job handling.

🎯 Objectives

Understand how Linux shells work

Implement process management

Execute commands using system calls

Learn command parsing techniques

Explore parent–child process interaction

✨ Features

✅ Execute Linux commands (ls, pwd, date, etc.)

✅ Command parsing and tokenization

✅ Process creation using fork()

✅ Program execution using execvp()

✅ Job handling support

✅ Continuous shell prompt

✅ Exit command functionality

✅ Error handling

🧠 Concepts Covered

Linux System Calls

Process Management

Shell Architecture

Command Interpreter Design

Parent & Child Processes

Job Control Basics

🗂️ Project Structure
MiniShell/
│
├── minishell.c      # Main shell program
├── input.c          # User input handling
├── execute.c        # Command execution logic
├── jobs.c           # Job/process management
├── minishell.h      # Header file
├── Documentation.txt
└── README.md
⚙️ Technologies Used

Language: C

Operating System: Linux / Ubuntu

Compiler: GCC

System Calls Used

fork()

execvp()

wait()

pipe()

dup2()

🚀 Compilation & Execution
Compile
gcc minishell.c input.c execute.c jobs.c -o minishell
Run
./minishell
💻 Example Usage
MiniShell$ pwd
/home/user

MiniShell$ ls
execute.c input.c minishell.c

MiniShell$ date
Mon Mar 2 21:30:00 IST

MiniShell$ exit
🔄 Working Flow

Display shell prompt

Read user input

Parse command arguments

Create child process using fork()

Execute command using exec()

Parent waits for completion

Repeat until exit

📘 Learning Outcomes

Practical understanding of Linux internals

Hands-on experience with system calls

Process lifecycle management

Shell implementation basics

🔮 Future Improvements

Pipe (|) implementation

I/O redirection (<, >)

Background execution (&)

Command history

Built-in commands (cd, history)

👨‍💻 Author

Vamshi Krishna
ECE Student | Linux Internals | Embedded Systems Enthusiast

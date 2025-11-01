# DSL task manager (C)

Welcome to the **DSL TASK MANAGER** a simple scripting program
that creates tasks with a `taskName` a `topic` and `time` to end this task.

---

## Features
- It uses one line syntax
- Easy to read because its like english
- It creates an executable and you can countdown the tasks
- You can create variables

### Examples
```bash
do study for 30 minutes about "Computer sciense"
do run for 20 minutes
do draw for 300 seconds

set marathon_time = 60
do marathon for marathon_time minutes
```
---

## Build Instructions
Make sure you have The `gcc` compiler and `Makefile` tools from gnu.

1. clone the reposetory
```bash
git clone https://github.com/skinwalker3654/TASK_DSL_MANAGER.git
```
2. compile the built-in compiler
```bash
make
```
3. use the executable to compile your programs
```bash
./compile yourprogram.txt
```
4. run the executable
```bash
./main  # This file is created after you compile your code
```
---
## Project Structure
```
TASK_DSL_MANAGER
   ├── compiler/
   |    └── compiler.c
   |
   ├─ headers/
   |    ├── task.c
   |    └── task.h
   |
   ├── README.md
   ├── Makefile
   └── LICENSE
```

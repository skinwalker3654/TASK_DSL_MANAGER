#ifndef TASK_H
#define TASK_H

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

typedef struct Task {
    char name[100];
    int time;
    char topic[100];
    int TopicOrNot;
} Task;

typedef struct List {
    Task tasks[100];
    int counter;
} List;

void addTask(List *ptr,char *name,char *topic,int time);
void printTasks(List *ptr);
int askYeNo(const char *messagge);
void countTasks(List *ptr);

#endif

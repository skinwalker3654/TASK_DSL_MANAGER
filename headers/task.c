#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "task.h"

void addTask(List *ptr,char *name,char *topic,int time) {
    if(ptr->counter >= 100) {
        printf(RED "Maximum task limit reached!\n" RESET);
        return;
    }

    strcpy(ptr->tasks[ptr->counter].name,name);
    if(strlen(topic)==0) {
        ptr->tasks[ptr->counter].TopicOrNot = 0;
    } else {
        strcpy(ptr->tasks[ptr->counter].topic,topic);
        ptr->tasks[ptr->counter].TopicOrNot = 1;
    }

    ptr->tasks[ptr->counter].time = time;
    ptr->counter++;
}

void printTasks(List *ptr) {
    if(ptr->counter == 0) {
        printf(YELLOW "No tasks available. Add some tasks first!\n" RESET);
        return;
    }

    printf(CYAN "\n📋 YOUR TASK LIST (%d tasks)\n" RESET, ptr->counter);
    printf(BLUE "┌─────┬──────────────────────┬──────────────────────┬───────┐\n" RESET);
    printf(BLUE "│ ID  │ Task Name            │ Topic                │ Time  │\n" RESET);
    printf(BLUE "├─────┼──────────────────────┼──────────────────────┼───────┤\n" RESET);

    for(int i = 0; i < ptr->counter; i++) {
        if(i >= 100) {
            printf("│ %d │",i);
        } else if(i >= 10 ) {
            printf("│ %d  │",i);
        } else {
            printf("│ %d   │",i);
        }

        printf(" %-20s │ ", ptr->tasks[i].name);
        if(ptr->tasks[i].TopicOrNot == 1) {
            printf("%-20s │ ",ptr->tasks[i].topic );
        } else if(ptr->tasks[i].TopicOrNot == 0) {
            printf("%-20s │ ","-");
        }

        printf("%-5d │\n",ptr->tasks[i].time);
    }

    printf(BLUE "└─────┴──────────────────────┴──────────────────────┴───────┘\n" RESET);
}

int askYesNo(const char* message) {
    char answer[10];
    while(1) {
        printf(YELLOW "%s (yes/no): " RESET, message);
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer,"\n")] = 0;
        if(strcmp(answer,"yes")==0) return 1;
        if(strcmp(answer,"no")==0) return 0;
        printf(RED "Please type 'yes' or 'no'.\n" RESET);
    }
}

void countTasks(List *ptr) {
    if(!askYesNo("Start countdown for all tasks?")) {
        printf(YELLOW "Exiting program...\n" RESET);
        exit(EXIT_SUCCESS);
    }

    for(int i=0; i<ptr->counter; i++) {
        Task t = ptr->tasks[i];
        printf(MAGENTA "\nStarting Task %d/%d\n" RESET, i+1, ptr->counter);
        printf(CYAN "   Name: %s\n" RESET, t.name);
        printf(CYAN "   Topic: %s\n" RESET, (t.TopicOrNot ? t.topic : "General"));
        printf(CYAN "   Duration: %d seconds\n\n" RESET, t.time);

        for(int j=t.time; j>=0; j--) {
            printf("\rTime remaining: %3d seconds", j);
            fflush(stdout);
            sleep(1);
        }

        printf("\r" GREEN "Task '%s' completed! ✓" RESET, t.name);
        sleep(1);
        
        if(i<ptr->counter-1) {
            printf(YELLOW "\n\nNext task: '%s'\n" RESET, ptr->tasks[i+1].name);
            if(!askYesNo("Continue to next task?")) {
                printf(YELLOW "Exiting program...\n" RESET);
                exit(EXIT_SUCCESS);
            }
        }
    }

    printf(MAGENTA "\n\nALL TASKS COMPLETED!\n" RESET);
}

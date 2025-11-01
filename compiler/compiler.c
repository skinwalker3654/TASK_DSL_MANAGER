#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    DO,
    FOR,
    SECONDS,
    MINUTES,
    ABOUT,
    STRING,
    NUMBER,
    SET,
    EQUAL,
    NAME,
    EOF_,
} TokenType;

typedef struct Token {
    TokenType type;
    char name[200];
} Token;

Token getNextToken(char **input) {
    while(isspace(**input)) (*input)++;

    if(isalpha(**input)) {
        Token token;
        int counter = 0;

        while(isalpha(**input) || **input == '_') {
            token.name[counter++] = **input;
            (*input)++;
        }

        token.name[counter] = '\0';
        if(strcmp(token.name,"do")==0) {
            token.type = DO;
            return token;
        } else if(strcmp(token.name,"for")==0) {
            token.type = FOR;
            return token;
        } else if(strcmp(token.name,"about")==0) {
            token.type = ABOUT;
            return token;
        } else if(strcmp(token.name,"seconds")==0) {
            token.type = SECONDS;
            return token;
        } else if(strcmp(token.name,"minutes")==0) {
            token.type = MINUTES;
            return token;
        } else if(strcmp(token.name,"set")==0) {
            token.type = SET;
            return token;
        } else {
            token.type = NAME;
            return token;
        }
    }

    if(isdigit(**input)) {
        Token token = {NUMBER,""};
        int counter = 0;

        while(isdigit(**input)) {
            token.name[counter++] = **input;
            (*input)++;
        }

        token.name[counter] = '\0';
        return token;
    }

    if(**input == '"') {
        Token token = {STRING,""};
        int counter = 0;

        (*input)++;
        while(**input != '"' && **input != '\0') {
            token.name[counter++] = **input;
            (*input)++;
        }

        token.name[counter] = '\0';
        (*input)++;

        return token;
    }

    char operation = *(*input)++;
    if(operation == '=') return (Token){EQUAL,"="};

    return (Token){EOF_,""};
}

typedef struct Symbol {
    struct {
        char name[200][200];
        int value[200];
        int counter;
    } Variables;
    char name[200][200];
    int time[200];
    int TopicOrNot[200];
    int SecondsOrMinutes[200];
    char topic[200][200];
    int counter;
} Symbol;

int parseTokens(Symbol *ptr,char **input) {
    Token token = getNextToken(input);
    if(token.type == EOF_) return 0;

    if(token.type == SET) {
        token = getNextToken(input);
        if(token.type != NAME) {
            printf("Error: Invalid variable name '%s'\n",token.name);
            return -1;
        }

        strcpy(ptr->Variables.name[ptr->Variables.counter],token.name);
        token = getNextToken(input);
        if(token.type != EQUAL) {
            printf("Error: Forgot equal '='\n");
            return -1;
        }

        token = getNextToken(input);
        if(token.type != NUMBER) {
            printf("Error: Invalid value '%s' only numbers allowed\n",token.name);
            return -1;
        }

        char *endPtr;
        int num = strtol(token.name,&endPtr,10);
        if(*endPtr != '\0') {
            printf("Error: Invalid number '%s'\n",token.name);
            return -1;
        }

        token = getNextToken(input);
        if(token.type != EOF_) {
            printf("Error: Invalid arguments passed\n");
            return -1;
        }

        ptr->Variables.value[ptr->Variables.counter] = num;
        ptr->Variables.counter++;

        return 0;
    }

    if(token.type != DO) {
        printf("Error: Tasks must start with 'do'\n");
        return -1;
    }
    
    token = getNextToken(input);
    if(token.type != NAME) {
        printf("Error: Invalid task name '%s'\n",token.name);
        return -1;
    }

    strcpy(ptr->name[ptr->counter],token.name);
    token = getNextToken(input);
    if(token.type != FOR) {
        printf("Error: Forgot to assing the 'for'\n");
        return -1;
    }

    token = getNextToken(input);
    if(token.type != NUMBER && token.type != NAME) {
        printf("Error: Invalid value '%s'\n",token.name);
        return -1;
    }

    if(token.type == NUMBER) {
        char *endPtr;
        int num = strtol(token.name,&endPtr,10);
        if(*endPtr != '\0') {
            printf("Error: Invalid number '%s'\n",token.name);
            return -1;
        }

        ptr->time[ptr->counter] = num;
    }

    if(token.type == NAME) {
        int found = -1;
        for(int i=0; i<ptr->Variables.counter; i++) {
            if(strcmp(token.name,ptr->Variables.name[i])==0) {
                found = i;
                break;
            }
        }

        if(found == -1) {
            printf("Error: Variable '%s' does not exists\n",token.name);
            return -1;
        }

        ptr->time[ptr->counter] = ptr->Variables.value[found];
    }

    token = getNextToken(input);
    if(token.type != SECONDS && token.type != MINUTES) {
        printf("Error: Forgot to assing 'seconds/minutes'\n");
        return -1;
    }
    
    if(token.type == SECONDS) ptr->SecondsOrMinutes[ptr->counter] = 0;
    else if(token.type == MINUTES) ptr->SecondsOrMinutes[ptr->counter] = 1;

    token = getNextToken(input);
    if(token.type == EOF_) {
        ptr->TopicOrNot[ptr->counter] = 0;
        ptr->counter++;
        return 0;
    }
    
    if(token.type == ABOUT) {
        token = getNextToken(input);
        if(token.type != STRING) {
            printf("Error: Invalid topic must be inside of \"\"\n");
            return -1;
        }

        char buff[200];
        strcpy(buff,token.name);

        token = getNextToken(input);
        if(token.type != EOF_) {
            printf("Error: Invalid arguments passed\n");
            return -1;
        }

        strcpy(ptr->topic[ptr->counter],buff);
        ptr->TopicOrNot[ptr->counter] = 1;
        ptr->counter++;

        return 0;
    } else {
        printf("Error: Invalid keyword '%s'\n",token.name);
        return -1;
    }
}

void codeGen(Symbol *ptr,FILE *file) {
    for(int i=0; i<ptr->counter; i++) {
        if(ptr->TopicOrNot[i] == 1) {
            if(ptr->SecondsOrMinutes[i] == 0)
                fprintf(file,"    addTask(&list,\"%s\",\"%s\",%d);\n",ptr->name[i],ptr->topic[i],ptr->time[i]);
            else if(ptr->SecondsOrMinutes[i] == 1)
                fprintf(file,"    addTask(&list,\"%s\",\"%s\",%d);\n",ptr->name[i],ptr->topic[i],ptr->time[i]*60);
        } else {
            if(ptr->SecondsOrMinutes[i] == 0)
                fprintf(file,"    addTask(&list,\"%s\",\"\",%d);\n",ptr->name[i],ptr->time[i]);
            else if(ptr->SecondsOrMinutes[i] == 1)
                fprintf(file,"    addTask(&list,\"%s\",\"\",%d);\n",ptr->name[i],ptr->time[i]*60);
        }
    }

    fprintf(file,"    printTasks(&list);\n");
    fprintf(file,"    countTasks(&list);\n");
}

int main(int argc,char **argv) {
    if(argc != 2) {
        printf("Error: Invalid usage\n");
        printf("Correct usage: ./compile <fileName>\n");
        return 1;
    }
    
    Symbol values = {.counter=0,.Variables.counter=0};
    FILE *input = fopen(argv[1],"r");
    if(!input) {
        printf("Error: Failed to open the file\n");
        return 1;
    }

    FILE *file = fopen("output.c","w");
    fprintf(file,"#include <stdio.h>\n#include \"headers/task.h\"\n");
    fprintf(file,"int main(void) {\n");
    fprintf(file,"    List list = {.counter=0};\n");

    char line[256];
    while(fgets(line,sizeof(line),input)) {
        char *ptr = line;
        int check = parseTokens(&values,&ptr);
        if(check == -1) { remove("output.c"); return 1; }
    }

    codeGen(&values,file);
    fclose(input);
    fprintf(file,"    return 0;\n}\n");
    fclose(file);

    system("gcc output.c headers/task.c -o main");
    system("rm output.c");

    return 0;
}

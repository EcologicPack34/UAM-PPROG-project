#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PROGRAM_NAME "anthill"
#define SAVE_DIR "./AntAmnesia/saves"
#define DATA_DIR "./AntAmnesia/data"

int main(){
    int seed;
    bool procedural;
    int loadSave;
    int input;
    char cmd[500];
    char save[500];
    char c;

    FILE *file;
    int i;
    int n_files = 0;

    printf("Welcome to " PROGRAM_NAME "game launcher\n");

    do{
        printf("What do you want to do?\n\
    1. Create game from .dat\n\
    2. Load game from .dat\n\
    3. Exit launcher\n");
        printf("input> ");
        if(scanf("%d", &loadSave) != 1 || loadSave < 1 || loadSave > 3){
            for (int i = 0; i < 5; i++)
            {
                printf("\033[2K");
                printf("\r\033[1A");
            }
        }
    }while(loadSave < 1 || loadSave > 3);
    if(loadSave == 3){
        return 0;
    }

    if(loadSave == 1){
        sprintf(cmd, "find " DATA_DIR " -maxdepth 1 -name \"*.dat\"");
    }
    if(loadSave == 2){
        sprintf(cmd, "find " SAVE_DIR " -maxdepth 1 -name \"*.dat\"");
    }
    for (int i = 0; i < 6; i++)
    {
        printf("\r\033[1A");
        printf("\033[2K");
    }

    file = popen(cmd, "r");
    if(!file){
        printf("Error finding files");
        return -1;
    }
    
    printf("Your files are:\n ");
    while(fgets(cmd, 500, file)){
        n_files++;
        printf("%d. %s", n_files , 1 + cmd + strlen((loadSave == 1) ? DATA_DIR : SAVE_DIR));
    }
    do{
        printf("input> ");
        if(scanf("%d", &input) != 1 || input < 1 || input > n_files){
            printf("\r\033[1A");
            printf("\033[2K");
        }
    }while(input < 1 || input > n_files);
    
    rewind(file);
    for (i = 0; i < input; i++)
    {
        fgets(save, 500, file);
    }
    save[strlen(save) - 1] = 0;

    if(loadSave == 1){
        do{
            printf("\nGenerate procedural(y/n): ");
            scanf("%c", &c);
            if((c != 'y' && c != 'n')){
                for (int i = 0; i < 1; i++)
                {
                    printf("\r\033[1A");
                    printf("\033[2K");
                }               
            }
        }while(c != 'y' && c != 'n');
        procedural = c == 'y';
    }

    pclose(file);
    return 0;
}
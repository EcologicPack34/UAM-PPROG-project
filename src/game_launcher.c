#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define PROGRAM_NAME "AntHell/anthill"
#define SAVE_DIR "./AntHell/saves"
#define DATA_DIR "./AntHell/data"

int main(){
    int seed;
    bool procedural;
    int loadSave;
    int input;
    char cmd[1500];
    char save[500];
    char aux[1500];
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
    
    printf("Your files are:\n");
    while(fgets(save, 500, file)){
        n_files++;
        printf(" %d. %s", n_files , save + 1 + ((loadSave == 1) ? strlen(DATA_DIR) : strlen(SAVE_DIR)));
    }
    do{
        printf("input> ");
        if(scanf("%d", &input) != 1 || input < 1 || input > n_files){
            printf("\r\033[1A");
            printf("\033[2K");
        }
    }while(input < 1 || input > n_files);
    
    pclose(file);
    file = popen(cmd, "r");
    if(!file){
        printf("Error finding files");
        return -1;
    }


    for (i = 0; i < input; i++)
    {
        fgets(save, 500, file);
    }
    save[strcspn(save, "\n")] = 0;
    if(loadSave == 1){
        do{
            printf("\nGenerate procedural(y/n): ");
            while((c = getchar()) == '\n');
            if((c != 'y' && c != 'n')){
                for (int i = 0; i < 1; i++)
                {
                    printf("\r\033[1A");
                    printf("\033[2K");
                }               
            }
        }while(c != 'y' && c != 'n');
        procedural = c == 'y';
        printf("\nIntroduce a seed (0 for random): ");
        scanf("%d", &seed);
        if(seed == 0){
            srand((unsigned int)time(NULL));
            seed = rand();
        }

        printf("Compiling game..\n");

        system("make");

        printf("\nLaunching game...");

        strcpy(cmd, "./");
        strcat(cmd, PROGRAM_NAME);
        sprintf(aux ," %s -s %d %s", save, seed, (procedural == true) ? "-proced" : "");
        strcat(cmd, aux);
    }else{
        strcpy(cmd, "./");
        strcat(cmd, PROGRAM_NAME);
        sprintf(aux ," %s", save);
        strcat(cmd, aux);
    }
    system(cmd);
    pclose(file);
    return 0;
}
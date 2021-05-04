#include "../../headers/handlers/historyHandler.h"

int current_index = 0; /** Used to determin the current index of the cmdLine*/
int counter = 0;

char *getUp() {
    fpos_t position;
    int s = sizeof(*hist);
    printf("siz: %d\n", s);
    fseek(hist, 0, SEEK_END);
    // fseek(hist, -4, SEEK_CUR);
    fgetpos(hist, &position);
    printf("pos: %ld\n", position.__pos);
    char *cmdLine = calloc(1, MAX_PATH_SIZE);

    cmdLine = fgets(cmdLine, MAX_PATH_SIZE, hist);
    printf("2-: %s\n", cmdLine);

    // fscanf(hist, "%s", cmdLine);
    // printf("3-: %s\n", cmdLine);

    return cmdLine;
}

char *getDown() {
}

void put(char *cmdLine) {
    fprintf(hist, "%s\n", cmdLine);
    // update();
}

void openHistory(char *type) {
    current_index = 0;
    char histPath[MAX_PATH_SIZE];
    sprintf(histPath, HISTORY_PATH);
    hist = fopen(histPath, type);
    if (!hist) {
        perror("Open history");
        return;
    }
}

void closeHistory() {
    fclose(hist);
}

void update() {
    counter++;
    closeHistory();
    openHistory("w");
    char *temp = calloc(sizeof(char), MAX_PATH_SIZE);
    sprintf(temp, "LENGTH: %d", counter);
    fprintf(hist, "%s", temp);
    free(temp);
    closeHistory();
    openHistory("a");
}
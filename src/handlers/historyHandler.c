#include "../../headers/handlers/historyHandler.h"

int current_index = 0; /** Used to determine the current index of the cmdLine*/
int histLength = 0;

char *get(int index) {
    openHistory("r");
    char *cmdLine = calloc(sizeof(char), MAX_PATH_SIZE);
    char *temp = calloc(sizeof(char), MAX_PATH_SIZE);

    int counter = 0;
    while ((temp = fgets(temp, MAX_PATH_SIZE, hist)) != NULL) {
        cmdLine = temp;

        if ((counter == (histLength - index - 1)) && (counter <= histLength)) {
            cmdLine[strlen(cmdLine) - 1] = '\0';
            closeHistory();
            return cmdLine;
        }
        counter++;
    }
    return NULL;
}

char *getDown() {
}

void put(char *cmdLine) {
    if (((!histLength) || strcmp(get(0), cmdLine)) && (strlen(cmdLine) > 0)) {
        openHistory("a");
        fprintf(hist, "%s\n", cmdLine);
        closeHistory();
        update();
    }
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
    char dataPath[MAX_PATH_SIZE];
    sprintf(dataPath, HISTORY_DATA_PATH);
    FILE *temp = fopen(dataPath, "w");
    if (!hist) {
        perror("Open history data");
        return;
    }

    histLength++;

    char *strtemp = calloc(sizeof(char), 64);
    sprintf(strtemp, "LENGTH: %d", histLength);
    fputs(strtemp, temp);
    free(strtemp);
    fclose(temp);
}

void initHistory() {
    char dataPath[MAX_PATH_SIZE];
    sprintf(dataPath, HISTORY_DATA_PATH);
    FILE *temp = fopen(dataPath, "r");
    if (!temp) {
        perror("Open history data");
        return;
    }
    char *strtemp = calloc(sizeof(char), MAX_PATH_SIZE);
    int len = 0;
    if (fscanf(temp, "%s %d", strtemp, &len)) {
        if (strcmp(strtemp, "LENGTH: ")) histLength = len;
    }
    free(strtemp);
    fclose(temp);
}

int getLength() {
    return histLength;
}

OBJ_MAIN = main.c
HANDLERS = handlers/inputHandler.c handlers/historyHandler.c handlers/commandsHandler.c
COMMANDS = commands/psCommand.c commands/pwdCommand.c commands/cdCommand.c commands/killCommand.c
CC = gcc
OBJ_NAME = ../out/a
all : $(OBJ_MAIN) $(HANDLERS) $(COMMANDS)
	$(CC) $(OBJ_MAIN) $(HANDLERS) $(COMMANDS) -o $(OBJ_NAME)
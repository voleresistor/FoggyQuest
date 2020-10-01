# OBJS specifies which files to compile as part of the project
OBJS = dq_main.c dq_locator.c dq_log.c dq_sdl.c dq_world.c dq_tile.c dq_map.c dq_actor.c dq_hero.c lib/util.c

# CC specifies which compiler we're using
CC = gcc

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w supresses all warnings
COMPILER_FLAGS = -Wl,--allow-multiple-definition -g #-v 

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image #-lm -lpthread

# OBJ_NAME specifies the name of our executable
OBJ_NAME = dq

# This is the target that compiles our executables
all : $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)

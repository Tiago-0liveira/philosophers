NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -pthread

SRC = main.c \
		fork.c \
		philo.c \
		utils.c \
		philo_utils.c

includefolder = include
INCLUDES = -I $(includefolder)
OBJ_DIR = obj
SRCS = $(addprefix src/, $(SRC))
OBJS = $(patsubst src/%, $(OBJ_DIR)/%, $(SRCS:%.c=%.o))

# Reset

Color_Off='\033[0m'       # Text Reset

IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
ICyan='\033[0;96m'        # Cyan

MSG1 = @echo ${IGreen}"Compiled Successfully ✔︎"${Color_Off}
MSG2 = @echo ${IYellow}"Cleaned Successfully ✔︎"${Color_Off}
MSG3 = @echo ${ICyan}"Cleaned ${NAME} Successfully ✔︎"${Color_Off}

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(NAME)
	$(MSG1)

$(OBJ_DIR)/%.o: $(SRCS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@/bin/rm -rf $(OBJ_DIR)
	$(MSG2)

fclean: clean
	@/bin/rm -rf $(NAME) ${OBJ_DIR}
	$(MSG3)

re: fclean all
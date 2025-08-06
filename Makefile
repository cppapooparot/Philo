NAME = philosophers

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCDIR = philo
SOURCES = main.c \
          init.c \
          check_args.c \
          routine.c \
          monitor.c \
          utils.c

SRCS = $(addprefix $(SRCDIR)/, $(SOURCES))
OBJECTS = $(SRCS:.c=.o)

# Colors for pretty output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(RESET)"

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/philo.h
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@$(RM) $(OBJECTS)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@$(RM) $(NAME)

re: fclean all

test: $(NAME)
	@echo "$(GREEN)Running test cases...$(RESET)"
	@echo "Test 1: Basic test (should not die)"
	@./$(NAME) 4 410 200 200
	@echo "\nTest 2: With meal limit"
	@./$(NAME) 4 310 200 100 5
	@echo "\nTest 3: Should die quickly"
	@./$(NAME) 4 200 205 200

debug: CFLAGS += -g -fsanitize=thread
debug: re

valgrind: $(NAME)
	valgrind --tool=helgrind ./$(NAME) 4 410 200 200

.PHONY: all clean fclean re test debug valgrind
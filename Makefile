RM = rm -f

INCDIR = inc
SRCDIR = src
OBJDIR = obj
OBJDIRS = $(shell find $(SRCDIR) -type d | sed 's/src/$(OBJDIR)/')

INC = $(shell find $(INCLUDEDIR) -name "*.h")
SRC = $(shell find $(SRCDIR) -name "*.c")
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC = gcc
CCFLAGS = -g -I$(INCDIR) --std=c99 -Wall -Wextra -Wpedantic #-Werror
LDFLAGS = -L$(LIBFTDIR) -lft

NAME = tar

all: prebuild $(NAME)

prebuild:
	@mkdir -vp $(OBJDIR)
	@for subdir in $(OBJDIRS); do mkdir -vp $$subdir; done

$(NAME): $(OBJ)
	@echo "Linking: $@"
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC)
	@echo "Compiling: $@"
	@$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	@echo "Removing objects"
	@$(RM) -r $(OBJDIR)

fclean:
	@echo "Removing objects"
	@$(RM) -r $(OBJDIR)
	@echo "Removing exectable"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all prebuild clean fclean re

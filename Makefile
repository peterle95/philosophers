# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 14:38:20 by pmolzer           #+#    #+#              #
#    Updated: 2024/08/13 16:24:24 by pmolzer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers 
# This line defines a variable NAME and sets its value to "philosophers". This will be the name of the executable that the Makefile will create.
SRCS =  $(wildcard sources/*.c)
# This line uses the wildcard function to find all .c files in the sources/ directory and assigns them to the SRCS variable. 
# This is a convenient way to automatically include all source files without listing them manually.
# The wildcard function is a built-in function in GNU Make. It's not a standard shell command or a custom function defined in the Makefile itself. 
# Rather, it's a feature provided by Make to help with file name expansion.
OBJS = ${SRCS:.c=.o}
# This line creates a list of object files (OBJS) by replacing the .c extension with .o for each file in SRCS. 
# This is done using a substitution reference.

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -pthread -g
# This line sets the compiler flags:

# -Wall, -Wextra, -Werror: Enable warnings and treat them as errors
# -Iincludes: Add the includes/ directory to the include path
#				specifies an additional directory for the compiler to search for header files
#						The -I flag tells the compiler to add the specified directory to its list of places to look for header files.
#				Why it's needed:

#				By default, the C compiler looks for header files in standard system directories and the current directory.
#				If you have your own header files in a separate directory 
#				(which is a common practice for organizing code), you need to tell the compiler where to find them.
# -pthread: Enable POSIX threads support
# -g: Include debugging information

RM = rm -rf
# This defines the command used for removing files, set to rm -rf (remove files and directories recursively, force removal).

all: ${NAME}
# This is the default target. It depends on ${NAME}, so building all will build the philosophers executable.
${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
# This rule specifies how to build the philosophers executable. It depends on the object files (${OBJS}). 
# The command compiles and links the object files into the final executable. 
# The @ at the beginning of the line suppresses the echoing of the command.

clean: 
	${RM} ${OBJS}
# This target removes all object files. The @ suppresses command echoing.

fclean: clean
	${RM} ${NAME}
# This target does a "full clean" - it first calls the clean target to remove object files, then removes the executable.
re: fclean all
# This target does a "full rebuild" by first calling fclean to remove all generated files, then calling all to rebuild everything from scratch.

.PHONY: all clean fclean re
# This line declares the targets all, clean, fclean, and re as phony targets. This means that these targets do not represent actual files, 
# preventing conflicts with any files that might have the same names and ensuring these targets always run when called.
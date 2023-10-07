NAME		= server
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
SRCS_DIR	= srcs
OBJS_DIR	= objs
INCLUDE_DIR	= include/
SRCS		= ${shell find ${SRCS_DIR} -name "*.c"}
OBJS		= ${subst ${SRCS_DIR}, ${OBJS_DIR}, ${SRCS:.c=.o}}

all: ${NAME}
${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c
	@mkdir -p ${OBJS_DIR}
	${CC} ${CFLAGS} -I ${INCLUDE_DIR} -o $@ -c $<

clean:
	rm -rf ${OBJS_DIR}

fclean: clean
	rm -f ${NAME}

re: fclean ${NAME}

.PHONY: clean fclean re

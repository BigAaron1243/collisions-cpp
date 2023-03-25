OBJS = main.o
CFLAGS = -Wall
INCLUDES = -I./glad/include/ -I./glfw/include/
CC = g++
LIBS = -L./glfw/lib-mingw-w64/ -lglfw3 -lgdi32

main:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}
	
clean:
	-rm -f *.o core *.core
	
.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $< ${LIBS}
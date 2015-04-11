COMPILE = g++
DEPEND = -lGL -lGLU -lGLEW -lglut -lm -Wno-write-strings -o
MAIN = heatmap
MOO = getbmp
all:
	${COMPILE} ${MAIN}.cpp ${MOO}.cpp ${DEPEND} ${MAIN}

clean:
	rm -rf ${MAIN}

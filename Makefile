CC=gcc
CFLAGS=-Ofast -shared -Wl,-soname,

all: allfive pokerlib

allfive: test/allfive.o src/libeval5.a src/libeval5fast.a
	${CC} ${CFLAGS}$@ -o test/allfive.so -fPIC $^

pokerlib: src/pokerlib.o src/libeval5.a src/libeval5fast.a
	${CC} ${CFLAGS}$@ -o test/pokerlib.so -fPIC $^
	
src/libeval5.a: src/pokerlib.o
	ar rcs $@ $^
	
src/libeval5fast.a: src/fast_eval.o
	ar rcs $@ $^

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

clean:
	rm -f src/*.o test/*.o *.o src/*.a allfive


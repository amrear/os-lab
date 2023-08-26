all: integrate integrate_sequential

integrate: integrate.c utils.c utils.h
	gcc -o integrate integrate.c utils.c -l m

integrate_sequential: integrate_sequential.c utils.c utils.h
	gcc -o integrate_sequential integrate_sequential.c utils.c -l m

clean:
	rm -f integrate integrate_sequential
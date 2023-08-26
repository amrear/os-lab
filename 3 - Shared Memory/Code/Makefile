all: integrate integrate_vfork

integrate: integrate.c utils.c utils.h
	gcc -o integrate integrate.c utils.c -l m -pthread

integrate_vfork: integrate_vfork.c utils.c utils.h
	gcc -o integrate_vfork integrate_vfork.c utils.c -l m -pthread
clean:
	rm -f integrate integrate_vfork
all: writemap.c memmap.c
	gcc -o writemap writemap.c
	gcc -o memmap memmap.c
clean:
	rm memmap writemap

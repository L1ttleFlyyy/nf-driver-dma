#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAPPED_SIZE 4096//place the size here
#define DDR_RAM_PHYS 0x2000//place the physical address here

unsigned int buff[] = {
	0x000423bb, 0x15ac004e,
	0x46324303, 0x08004500,
	0x00320000, 0x40004011,
	0x26b50a01, 0x00020a01,
	0x00035000, 0x5000001e,
	0xc24c4865, 0x6c6c6f20,
	0x20202020, 0x30313233,
	0x34353637, 0x38392022
};

int main(){
    int _fdmem;
    unsigned int *map = NULL;
    const char memDevice[] = "/dev/mem";

    /* open /dev/mem and error checking */
    _fdmem = open( memDevice, O_RDWR | O_SYNC );

    if (_fdmem < 0){
        perror("Failed to open the /dev/mem !\n");
    return 0;
    }
    else{
        printf("open /dev/mem successfully !\n");
    }

    /* mmap() the opened /dev/mem */
    map= (unsigned int *)mmap(NULL,MAPPED_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,_fdmem,DDR_RAM_PHYS);
    if(!map){
        perror("mmap failed");
        close(_fdmem);
        return -1;
    }
    /* use 'map' pointer to access the mapped area! */
    int i;
    for (i=0;i<(sizeof(buff)/sizeof(unsigned int));i++){
        *(map+i) = htonl(buff[i]);
        printf("0x%08x\n", *(map+i));
    }

    /* unmap the area & error checking */
    if (munmap(map,MAPPED_SIZE)==-1){
        perror("Error un-mmapping the file");
    }

    /* close the character device */
    close(_fdmem);
    return 0;
}


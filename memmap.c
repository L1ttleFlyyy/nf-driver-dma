#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#define MAPPED_SIZE 4096//place the size here
#define DDR_RAM_PHYS 0x2000//place the physical address here
int main(){
    int _fdmem;
    int *map = NULL;
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
    map= (int *)mmap(NULL,MAPPED_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,_fdmem,DDR_RAM_PHYS);
    if(!map){
        perror("mmap failed");
        close(_fdmem);
        return -1;
    }
    /* use 'map' pointer to access the mapped area! */
    int i;
    int len = 20;
    memset(map, 0, len * 4);
    for (i=0;i<len;i++){
        printf("0x%08x: 0x%08x\n",(unsigned int)(map+i),htonl(*(map+i)));
    }
    printf("press enter to continue...\n");
    getchar();
    for (i=0;i<len;i++){
        printf("0x%08x: 0x%08x\n",(unsigned int)(map+i),htonl(*(map+i)));
    }
    /* unmap the area & error checking */
    if (munmap(map,MAPPED_SIZE)==-1){
        perror("Error un-mmapping the file");
    }

    /* close the character device */
    close(_fdmem);
    return 0;
}


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <malloc_np.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	//printf("%s", argv[1]);
    int fp = open(argv[1], O_RDONLY); //open file
	if (fp == -1){
		printf("open error");
	}
	int size = lseek(fp, 0, SEEK_END); // get offset at end of file
    lseek(fp, 0, SEEK_SET); // seek back to beginning
    void * buff = malloc(size+1); // allocate enough memory.
    read(fp, buff, size);  // read in the file




}

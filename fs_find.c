#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <malloc_np.h>
#include <fcntl.h>
#include "fs.h"




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



    /*
    From fs.h:

        * Depending on the architecture and the media, the superblock may
        * reside in any one of four places. For tiny media where every block
        * counts, it is placed at the very front of the partition. Historically,
        * UFS1 placed it 8K from the front to leave room for the disk label and
        * a small bootstrap. For UFS2 it got moved to 64K from the front to leave
        * room for the disk label and a bigger bootstrap, and for really piggy
        * systems we check at 256K from the front if the first three fail. In
        * all cases the size of the superblock will be SBLOCKSIZE. All values are
        * given in byte-offset form, so they do not imply a sector size. The
        * SBLOCKSEARCH specifies the order in which the locations should be searched.
    We used fsdump ada1p1 to read the super block info for this partition. We
    found that the magic number is 19540119 (424935705 in decimal). We casted
    location 0 and 8k to struct fs but none of them generated the same magic
    number. When casting the 64k element of the .img file into a struct fs, it
    had the same magic number we are looking for. Thus, the super block for
    ada1p1 starts at 64k. 
    */
    struct fs* super_block = (struct fs*)&buff[65536];

    printf("%d", super_block->fs_magic);





}

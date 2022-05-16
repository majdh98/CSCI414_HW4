#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <malloc_np.h>
#include <fcntl.h>
#include "fs.h"
#include "dinode.h"
#include "dir.h"




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
    int frag_size = super_block -> fs_fsize;// in bytes
    int block_size = super_block ->fs_bsize;
    int num_cgroup = super_block -> fs_ncg; // number of cylindrical groups
    int size_cgroup = super_block -> fs_cgsize; //size of cylindrical group

    /*an array that holds the starting location for the cylindar's inode table*/
    uint64_t cs_inode_table [num_cgroup];
    for (int i = 0; i< num_cgroup; i++){
    	cs_inode_table[i] = cgimin(super_block, i)*frag_size;
    }


	printf("block size: %d\n", block_size);
    printf("frag size: %d\n", frag_size);
    printf("inode size: %d\n", 256);
    printf("super block: %d\n", 65536);
    printf("c0: %ld\n", cgbase(super_block, 0));
	/*for this test file, the number of cylinders is 4. The parent directory is located in inode 2 of c group 0.
	// from dinode.h:

		 * The root inode is the root of the filesystem.  Inode 0 can't be used for
		 * normal purposes and historically bad blocks were linked to inode 1, thus
		 * the root inode is 2.  (Inode 1 is no longer used for this purpose, however
		 * numerous dump tapes make this assumption, so we are stuck with it).
	*/
	int c0_inode_table = cgimin(super_block, 0)*frag_size;
	int inode_2_loc = c0_inode_table + 2*256;
    struct ufs2_dinode* inode_2 = (struct ufs2_dinode*)&buff[inode_2_loc];

    int root_dir_loc = inode_2->di_db[0]*frag_size;
    struct	direct* root_dir = ( struct direct *)&buff[root_dir_loc];




    // write(1, "\n", 1);
    // write(1, (buff+root_dir_loc), 512);
    // write(1, "\n", 1);

    // printf("root_dir inode num: %d\n",root_dir->d_ino);
    // printf("root_dir record length: %d\n",root_dir->d_reclen);
    // printf("root_dir file type: %d\n",root_dir->d_type);
    // printf("length of string in dname: %d\n",root_dir->d_namlen);
    write(1, root_dir->d_name, root_dir->d_namlen);
    write(1, "\n", 1);

    root_dir_loc = root_dir_loc+root_dir->d_reclen;
    root_dir = ( struct direct *)&buff[root_dir_loc];
    // printf("root_dir inode num: %d\n",root_dir->d_ino);
    // printf("root_dir record length: %d\n",root_dir->d_reclen);
    // printf("root_dir file type: %d\n",root_dir->d_type);
    // printf("length of string in dname: %d\n",root_dir->d_namlen);
    write(1, root_dir->d_name, root_dir->d_namlen);
    write(1, "\n", 1);

    root_dir_loc = root_dir_loc+root_dir->d_reclen;
    root_dir = ( struct direct *)&buff[root_dir_loc];
    // printf("root_dir inode num: %d\n",root_dir->d_ino);
    // printf("root_dir record length: %d\n",root_dir->d_reclen);
    // printf("root_dir file type: %d\n",root_dir->d_type);
    // printf("length of string in dname: %d\n",root_dir->d_namlen);
    write(1, root_dir->d_name, root_dir->d_namlen);
    write(1, "\n", 1);

	// this is file1 directory entry. Its inode is 3
    root_dir_loc = root_dir_loc+root_dir->d_reclen;
    root_dir = ( struct direct *)&buff[root_dir_loc];
    // printf("root_dir inode num: %d\n",root_dir->d_ino);
    // printf("root_dir record length: %d\n",root_dir->d_reclen);
    // printf("root_dir file type: %d\n",root_dir->d_type);
    // printf("length of string in dname: %d\n",root_dir->d_namlen);
    write(1, root_dir->d_name, root_dir->d_namlen);
    write(1, "\n", 1);

    // //inode 3 must exists within cylindar 0:
    // int inode_file1_loc = cs_inode_table[0] + root_dir->d_ino*256;
    // struct ufs2_dinode* file1_inode = (struct ufs2_dinode*)&buff[inode_file1_loc];
    // uint64_t data_loc = file1_inode->	di_db[0]*frag_size;
    //
    // write(1, "\n", 1);
    // write(1, (buff+data_loc), file1_inode->di_size);
    // write(1, "\n", 1);


    root_dir_loc = root_dir_loc+root_dir->d_reclen;
    root_dir = ( struct direct *)&buff[root_dir_loc];
    // printf("root_dir inode num: %d\n",root_dir->d_ino);
    // printf("root_dir record length: %d\n",root_dir->d_reclen);
    // printf("root_dir file type: %d\n",root_dir->d_type);
    // printf("length of string in dname: %d\n",root_dir->d_namlen);
    write(1, root_dir->d_name, root_dir->d_namlen);
    write(1, "\n", 1);





    root_dir_loc = root_dir_loc+root_dir->d_reclen;
    root_dir = ( struct direct *)&buff[root_dir_loc];
    // printf("root_dir inode num: %d\n",root_dir->d_ino);
    // printf("root_dir record length: %d\n",root_dir->d_reclen);
    // printf("root_dir file type: %d\n",root_dir->d_type);
    // printf("length of string in dname: %d\n",root_dir->d_namlen);
    write(1, root_dir->d_name, root_dir->d_namlen);
    write(1, "\n", 1);





























	// int c0_base = cgbase(super_block, 0);
	// int c0_sb = cgsblock(super_block, 0);
	// int c0_start = cgstart(super_block, 0);
	// int c0_block = cgtod(super_block, 0);
	// printf("c0 start: %d\n", c0_start );
	// printf("base of c0: %d\n", c0_base) ;
	// printf("location of sb in c0: %d\n", c0_sb);
	// printf("c0 cg block: %d\n", c0_block);
	// printf("location of first inode table: %d\n", c0_inode_table);
    //
    //
    //
    // int c1_super = cgsblock(super_block, 0);
    // printf("sssss%dssssss\n", c1_super*4096);
    // printf("fragment number of sb in c1: %d\n", c1_super);
    // struct fs* super_block1 = (struct fs*)&buff[c1_super*4096];
    // printf("%d\n", super_block1->fs_magic);
	// printf("fragment number of c1: %ld\n", cgbase(super_block, 1));
    // printf("fragment size: %d\n", frag_size);
    //
    //
    // printf("%d", super_block->fs_magic);





}

#include "so2ext2.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


#define BLOCK_SIZE 1024

#define EXT2_FT_UNKNOWN		0	//Unknown File Type
#define EXT2_FT_REG_FILE	1	//Regular File
#define EXT2_FT_DIR		2	//Directory File
#define EXT2_FT_CHRDEV		3	//Character Device
#define EXT2_FT_BLKDEV		4	//Block Device
#define EXT2_FT_FIFO		5	//Buffer File
#define EXT2_FT_SOCK		6	//Socket File
#define EXT2_FT_SYMLINK		7	//Symbolic Link


void printSuperBlock(struct ext2_super_block *superBlock)
{
	printf("\n============SUPERBLOCK==============\n");
	printf("Inodes count: %u\n", superBlock->s_inodes_count);
	printf("Blocks count: %u\n", superBlock->s_blocks_count);
	printf("Reserved blocks count: %u\n", superBlock->s_r_blocks_count);
	printf("Free blocks count: %u\n", superBlock->s_free_blocks_count);
	printf("Free inodes count: %u\n", superBlock->s_free_inodes_count);
	printf("First Data Block: %u\n", superBlock->s_first_data_block);
	printf("Block size: %u\n", superBlock->s_log_block_size);
	printf("Fragment size: %u\n", superBlock->s_log_frag_size);
	printf("Blocks per group: %u\n", superBlock->s_blocks_per_group);
	printf("Fragments per group: %u\n", superBlock->s_frags_per_group);
	printf("Inodes per group: %u\n", superBlock->s_inodes_per_group);
	printf("Mount time: %u\n", superBlock->s_mtime);
	printf("Write time: %u\n", superBlock->s_wtime);
	printf("Mount count: %u\n", superBlock->s_mnt_count);
	printf("Maximal mount count: %u\n", superBlock->s_max_mnt_count);
	printf("Magic signature: %u\n", superBlock->s_magic);
	printf("File system state: %u\n", superBlock->s_state);
	printf("Behaviour when detecting errors %u\n", superBlock->s_errors);
	printf("Padding?: %u\n", superBlock->s_pad);
	printf("Time of last check: %u\n", superBlock->s_lastcheck);
	printf("Max time between checks: %u\n", superBlock->s_checkinterval);
	printf("Creator OS: %u\n", superBlock->s_creator_os);
	printf("Revision level: %u\n", superBlock->s_rev_level);
	printf("Default uid for reserved blocks: %u\n", superBlock->s_def_resuid);
	printf("Default gid for reserved blocks: %u\n", superBlock->s_def_resgid);
	//printf("Padding to the end of the block: %ui\n", superBlock->s_reserved);
}

void printGroupDesc(struct ext2_group_desc *groupDesc)
{
	printf("\n============GROUPDESC==============\n");
	printf("Block bitmap block: %u\n", groupDesc->bg_block_bitmap);
	printf("Inodes bitmap block: %u\n", groupDesc->bg_inode_bitmap);
	printf("Inodes table block: %u\n", groupDesc->bg_inode_table);
	printf("Free blocks count: %u\n", groupDesc->bg_free_blocks_count);
	printf("Free inodes count: %u\n", groupDesc->bg_free_inodes_count);
	printf("Directories count: %u\n", groupDesc->bg_used_dirs_count);
	printf("Padding?: %u\n", groupDesc->bg_pad);
	//printf("Reserved: %u\n", groupDesc->bg_reserved);
}


void printINode(struct ext2_inode *iNode)
{
	printf("\n==============INODE=================\n");
	printf("File mode: %u\n", iNode->i_mode);
	printf("File type: %u\n", iNode->i_mode >> 12);
	printf("Owner Uid: %u\n", iNode->i_uid);
	printf("Size in bytes: %u\n", iNode->i_size);
	printf("Access time: %u\n", iNode->i_atime);
	printf("Creation time: %u\n", iNode->i_ctime);
	printf("Modification time: %u\n", iNode->i_mtime);
	printf("Deletion time: %u\n", iNode->i_dtime);
	printf("Group Id: %u\n", iNode->i_gid);
	printf("Links count: %u\n", iNode->i_links_count);
	printf("Blocks count: %u\n", iNode->i_blocks);
	printf("File flags: %u\n", iNode->i_flags);
	//Pointers to blocks iNode->i_block[EXT2_N_BLOCKS]
	printf("==Blocks containing data: ==\n");
	for(int i=0; i<EXT2_N_BLOCKS && iNode->i_block[i] != 0; ++i)
		printf("%d: %u\n", i, iNode->i_block[i]);
	printf("==END==\n");
	printf("File version for NFS: %u\n", iNode->i_version);
	printf("File ACL: %u\n", iNode->i_file_acl);
	printf("Directory ACL: %u\n", iNode->i_dir_acl);
	printf("Fragment address: %u\n", iNode->i_faddr);
	printf("Modification time: %u\n", iNode->i_mtime);
}


void printDirEntry(struct ext2_dir_entry *dirEntry)
{
	printf("=============DIRENTRY===============\n");
	printf("Inode number: %u\n", dirEntry->inode);
	printf("Directory entry length: %u\n", dirEntry->rec_len);
	printf("Name length: %u\n", dirEntry->name_len);
	printf("File type: %u\n", dirEntry->file_type);
	printf("File name:: %s\n", dirEntry->name);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("%s Error: Invalid argument count\nUsage: %s path_to_ext2_image\n", argv[0], argv[0]);
		return 1;
	}
	int fileDescriptor = open(argv[1], O_RDONLY);
	if( fileDescriptor == -1)
	{
		printf("Error: Cannot open file:  %s\n", argv[1]);
		return 1;
	}
	//1-st 1024 bytes is boot sector
	if( lseek(fileDescriptor, BLOCK_SIZE, SEEK_SET) == -1 )
	{
		printf("Error in lseek function!\n");
		return 1;
	}

	struct ext2_super_block superBlock;
	
	if( sizeof(superBlock) != read(fileDescriptor, &superBlock, sizeof(superBlock)) )
	{
		printf("Error while reading superblock!\n");
		return 1;
	}
	//printSuperBlock(&superBlock);
	struct ext2_group_desc groupDesc;
	if( sizeof(groupDesc) != read(fileDescriptor, &groupDesc, sizeof(groupDesc)) )
	{
		printf("Error while reading groupDesc!\n");
		return 1;
	}
	//printGroupDesc(&groupDesc);
	//set file pointer to 2nd iNode
	lseek(fileDescriptor, BLOCK_SIZE * groupDesc.bg_inode_table + sizeof(struct ext2_inode), SEEK_SET);
	
	struct ext2_inode rootINode;
	if( sizeof(rootINode) != read(fileDescriptor, &rootINode, sizeof( rootINode)) )
	{
		printf("Error while reading iNode!\n");
		return 1;
	}
	//printINode(&rootINode);

	
	printf("Type of file\t%7s\t\tPriv\tName\n", " Size");
	
	//set to data block
	size_t directoryPosition = BLOCK_SIZE * rootINode.i_block[0];
	size_t dirEntryBytesRead = 0;
	struct ext2_dir_entry dirEntry;
	while(dirEntryBytesRead < rootINode.i_size)
	{
		lseek(fileDescriptor, directoryPosition, SEEK_SET);
		if( sizeof(dirEntry) != read(fileDescriptor, &dirEntry, sizeof( dirEntry)) )
		{
			printf("Error while reading dirEntry!\n");
			return 1;
		}
		dirEntry.name[dirEntry.name_len]='\0';
		//printDirEntry(&dirEntry);
		
		
		switch(dirEntry.file_type)
		{
			default:
				printf("Unknown File Type");
				break;
			case EXT2_FT_REG_FILE:
				printf("Regular File");
				break;
			case EXT2_FT_DIR:
				printf("Directory");
				break;
			case EXT2_FT_CHRDEV:
				printf("Character Device");
				break;
			case EXT2_FT_BLKDEV:
				printf("Block Device");
				break;
			case EXT2_FT_FIFO:
				printf("Buffer File");
				break;
			case EXT2_FT_SOCK:
				printf("Socket File");
				break;
			case EXT2_FT_SYMLINK:
				printf("Symbolic Link");
				break;
		}

		//find block group and local iNode
		int blockGroup = (dirEntry.inode -1) / superBlock.s_inodes_per_group;
		int groupINode = (dirEntry.inode -1) % superBlock.s_inodes_per_group;
		//find inodesTable in block group
		lseek(fileDescriptor, BLOCK_SIZE + blockGroup * superBlock.s_blocks_per_group * BLOCK_SIZE + sizeof(struct ext2_super_block), SEEK_SET);
		read(fileDescriptor, &groupDesc, sizeof(groupDesc));
		//printGroupDesc(&groupDesc);
		//read dir entry iNode
		struct ext2_inode fileINode;
		//set iNode position
		lseek(fileDescriptor, blockGroup * superBlock.s_blocks_per_group * BLOCK_SIZE + BLOCK_SIZE * groupDesc.bg_inode_table + sizeof(struct ext2_inode) * groupINode , SEEK_SET);
		read(fileDescriptor, &fileINode, sizeof(fileINode));
		int userPrivilages, groupPrivilages, otherPrivilages;
		otherPrivilages = fileINode.i_mode & 0x7; // 7 = 00000111, so mask to check privilages
		groupPrivilages = (fileINode.i_mode >> 3) & 0x7;
		userPrivilages = (fileINode.i_mode >> 6) & 0x7;
		//printINode(&fileINode);
		printf("\t%6dB\t\t%d%d%d\t%s\n", fileINode.i_size, userPrivilages, groupPrivilages, otherPrivilages, dirEntry.name);
		
		directoryPosition += dirEntry.rec_len;
		dirEntryBytesRead+= dirEntry.rec_len;
	}
	
	//printf("Rozmiar superbloku: %lu", sizeof(superBlock));
	//printf("Rozmiar groupDesc: %lu", sizeof(struct ext2_group_desc));
	//printf("Rozmiar dirEntry: %lu\n", sizeof(rootINode));
	close(fileDescriptor);
	return 0;
}

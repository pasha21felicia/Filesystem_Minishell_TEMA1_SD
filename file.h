#ifndef __FILE_H__

typedef struct Directory {
	char *name;
	struct File_list *childFile_list;
	struct Directory_list *childDir_list;
	struct Directory *parentDir;
} Directory;

typedef struct File {
	char *name;
	int size;
	char *data;
	Directory *dir;
} File;

typedef struct File_list {
	struct File_list *prev;
	struct File_list *next;
	struct File node_File;
} File_list;

typedef struct Directory_list {
	struct Directory_list *prev;
	struct Directory_list *next;
	struct Directory node_Dir;
} Directory_list;
#endif

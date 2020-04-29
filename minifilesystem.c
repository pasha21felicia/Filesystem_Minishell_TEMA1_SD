#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

//creeaza si aloca memorie directorului radacina
Directory *create_fs(char *name)
{
	Directory *root = (Directory *)malloc(sizeof(Directory));

	root->name = (char *)malloc(sizeof(char)*strlen(name));

	root->childDir_list = NULL;
	root->childFile_list = NULL;
	root->parentDir = NULL;

	strcpy(root->name, name);
	return root;
}

//functiile de printare a liste de directoare si fisiere
void printDir(Directory_list *dir)
{
	Directory_list *last;

	while (dir != NULL) {
		if (dir->next == NULL)
			printf("%s", dir->node_Dir.name);
		else
			printf("%s ", dir->node_Dir.name);
		last = dir;
		dir = dir->next;
	}
}
void printFile(File_list *file, Directory *current_Dir)
{
	File_list *last;

	while (file != NULL) {
		if (file->next == NULL && current_Dir->childDir_list == NULL)
			printf("%s", file->node_File.name);
		else if (file->next == NULL &&
				current_Dir->childDir_list != NULL)
			printf("%s ", file->node_File.name);
		else
			printf("%s ", file->node_File.name);
		last = file;
		file = file->next;
	}
}

//functiile de creare a nodului director si nodului fisier din liste
Directory_list *getDir(char *name, Directory *current_Dir)
{
	Directory_list *newNode =
			(Directory_list *)malloc(sizeof(Directory_list));
	Directory *node_Dir = (Directory *)malloc(sizeof(Directory));

	newNode->node_Dir.name = (char *)malloc(sizeof(char) * strlen(name));

	strcpy(newNode->node_Dir.name, name);
	newNode->node_Dir.childDir_list = NULL;
	newNode->node_Dir.childFile_list = NULL;
	newNode->node_Dir.parentDir = current_Dir;
	newNode->prev = newNode->next = NULL;
	return newNode;
}

File_list *getFile(char *name, char *data, Directory *current_Dir)
{

	File_list *newNode = (File_list *)malloc(sizeof(File_list));

	newNode->node_File.name = (char *)malloc(sizeof(char)*strlen(name));

	newNode->node_File.data = (char *)malloc(sizeof(char)*strlen(data));

	strcpy(newNode->node_File.name, name);
	strcpy(newNode->node_File.data, data);
	newNode->node_File.size = strlen(data);
	newNode->node_File.dir = current_Dir;
	newNode->prev = newNode->next = NULL;
	return newNode;
}

//face mkdir-ul. adauga un nod Directory in lista de directoare
// a directorului curent
void add_Dir(Directory_list **head_ref,
			Directory_list *newNode, Directory **current_Dir)
{
	Directory_list *temp;

	//daca lista era nula, nodul devine capul listei
	if (*head_ref == NULL)
		*head_ref = newNode;
	//daca nodul va trebui asezat la inceputul listei
	else if (strcmp((*head_ref)->node_Dir.name,
				newNode->node_Dir.name) >= 0) {
		newNode->next = *head_ref;
		newNode->next->prev = newNode;
		*head_ref = newNode;
	} else {
		temp = *head_ref;
		//gasesc nodul dupa care ar trebui sa inserez
		while (temp->next != NULL &&
				strcmp(temp->next->node_Dir.name,
						newNode->node_Dir.name) < 0)
			temp = temp->next;
		newNode->next = temp->next;

		if (temp->next != NULL)
			newNode->next->prev = newNode;

		temp->next = newNode;
		newNode->prev = temp;
	}

	//adaug lista respectiva in lista de dir copii a lui current_Dir
	(*current_Dir)->childDir_list = *head_ref;
}

//face touch-ul, adauga un nod file in lista de fisiere a direct curent
//acceasi functionalitate ca la add_Dir
void add_File(File_list **head_ref, File_list *newNode, Directory **current_Dir)
{
	struct File_list *temp;

	newNode->node_File.dir = *current_Dir;

	if (*head_ref == NULL)
		*head_ref = newNode;
	else if (strcmp((*head_ref)->node_File.name,
						newNode->node_File.name) >= 0) {
		newNode->next = *head_ref;
		newNode->next->prev = newNode;
		*head_ref = newNode;
	} else {
		temp = *head_ref;
		while (temp->next != NULL &&
				strcmp(temp->next->node_File.name,
						newNode->node_File.name) < 0)
			temp = temp->next;
		newNode->next = temp->next;

		if (temp->next != NULL)
			newNode->next->prev = newNode;
		temp->next = newNode;
		newNode->prev = temp;
	}

	(*current_Dir)->childFile_list = *head_ref;
}


void LS(Directory *current_Dir)
{
	printFile(current_Dir->childFile_list, current_Dir);
	printDir(current_Dir->childDir_list);
}


void PWD(Directory *current_Dir)
{
	Directory *initial = current_Dir;

	//cazul daca suntem in root
	if (strcmp(current_Dir->name, "/") == 0) {
		printf("%s\n", current_Dir->name);
		return;
	}

	//un vector auxiliar in care pastrez doar numele
	//fiecarui parinte director
	char **name_of_Dirs = malloc(50 * sizeof(char *));
	int i;

	for (i = 0; i < 50; ++i)
		name_of_Dirs[i] = (char *)malloc(sizeof(char) * 10 + 1);

	int k = 0, nr = 0;

	//traversez lista de parinti a directorului curent
	//inserez numele in acel vector auxiliar
	while (current_Dir->parentDir != NULL) {
		strcpy(name_of_Dirs[k], current_Dir->name);
		current_Dir = current_Dir->parentDir;
		nr++;
		k++;
	}
	//afisez calea
	for (int i = nr; i > 0; i--)
		printf("%s/", name_of_Dirs[i]);
	printf("%s\n", initial->name);
}

Directory *CD(Directory *current_Dir, char *name)
{

	Directory *father_Dir;
	Directory *child_Dir = (Directory *)malloc(sizeof(Directory));

	Directory_list *head_ref = current_Dir->childDir_list;
	Directory_list *temp = head_ref;

	//daca avem .. ne directorul curent devine parintele
	//fostului director curent
	if (strcmp(name, "..") == 0) {
		father_Dir = current_Dir->parentDir;
		return father_Dir;
	}

	if (head_ref == NULL) {
		printf("Cannot move to '%s': No such directory!\n", name);
		return current_Dir;
	}
	//parcurg lista de copii si directorul curent devine directorul gasit
	//dupa nume
	while (temp != NULL) {
		if (strcmp(temp->node_Dir.name, name) == 0) {
			*child_Dir = temp->node_Dir;
			return child_Dir;
		}
		if (temp->next != NULL) {
			temp = temp->next;
		} else {
			printf("Cannot move to '%s': No such directory!\n",
					name);
			return current_Dir;
		}
	}
}

void printFile_tree(Directory *current_Dir)
{
	File_list *last;

	File_list *file = current_Dir->childFile_list;

	while (file != NULL) {
		printf("    %s\n", file->node_File.name);
		last = file;
		file = file->next;
	}
}
void printDir_tree(Directory *current_Dir)
{
	Directory_list *last;

	Directory_list *dir = current_Dir->childDir_list;

	while (dir != NULL) {
		printf("    %s\n", dir->node_Dir.name);
		last = dir;
		dir = dir->next;
	}
}
void TREE(Directory *current_Dir)
{
	//printf("%s\n", current_Dir->name);
	Directory_list *last_dir;

	File_list *head_File = current_Dir->childFile_list;
	Directory_list *head_Dir = current_Dir->childDir_list;

	if (current_Dir->childDir_list == NULL) {
		printFile_tree(current_Dir);
	} else {
		while (head_Dir != NULL) {
			printFile_tree(current_Dir);
			*current_Dir = head_Dir->node_Dir;

			printf("curent = %s\n", current_Dir->name);
			printDir_tree(current_Dir);
			if (current_Dir->childDir_list != NULL) {
				printf("nu e null\n");
				head_Dir = current_Dir->childDir_list;
			} else {
				last_dir = head_Dir;
				head_Dir = head_Dir->next;
			}
		}
	}
}
//functia returneaza pozitia la care se afla fisierul de sters
int GetFilePosition(Directory *current_Dir, char *name)
{

	File_list *head_ref = current_Dir->childFile_list;
	File_list *temp = head_ref;
	int n = 0;

	if (head_ref == NULL) {
		printf("Cannot remove '%s': No such file!\n", name);
		return 0;
	}
	while (temp != NULL) {
		if (strcmp(temp->node_File.name, name) == 0)
			return n+1;
		if (temp->next != NULL) {
			temp = temp->next;
			n++;
		} else {
			printf("Cannot remove '%s': No such file!\n", name);
			return 0;
		}
	}
}

//functia de stergere a unui element de lista
void rm_File(File_list **head_ref, File_list *elem)
{

	if (*head_ref == NULL || elem == NULL)
		return;
	if (*head_ref == elem)
		*head_ref = elem->next;
	if (elem->next != NULL)
		elem->next->prev = elem->prev;
	if (elem->prev != NULL)
		elem->prev->next = elem->next;
	free(elem);
}
//functia de stergere a elementului exact dupa pozitie
void rm(File_list **head_ref, int n)
{
	if (*head_ref == NULL || n <= 0)
		return;
	File_list *temp = *head_ref;

	for (int i = 1; temp != NULL && i < n; i++)
		temp = temp->next;

	if (temp == NULL)
		return;
	rm_File(head_ref, temp);
}

//functioneaza la fel ca la fisiere
int GetDirPosition(Directory *current_Dir, char *name)
{

	Directory_list *head_ref = current_Dir->childDir_list;
	Directory_list *temp = head_ref;
	int n = 0;

	if (head_ref == NULL) {
		printf("Cannot remove '%s': No such directory!\n", name);
		return 0;
	}
	while (temp != NULL) {
		if (strcmp(temp->node_Dir.name, name) == 0)
			return n+1;
		if (temp->next != NULL) {
			temp = temp->next;
			n++;
		} else {
			printf("Cannot remove '%s': No such directory!\n",
				name);
			return 0;
		}
	}
}
void rm_Dir(Directory_list **head_ref, Directory_list *elem)
{

	if (*head_ref == NULL || elem == NULL)
		return;
	if (*head_ref == elem)
		*head_ref = elem->next;
	if (elem->next != NULL)
		elem->next->prev = elem->prev;
	if (elem->prev != NULL)
		elem->prev->next = elem->next;
	free(elem->node_Dir.childDir_list);
	free(elem->node_Dir.childFile_list);
	free(elem);
}
void rmdir(Directory_list **head_ref, int n)
{
	if (*head_ref == NULL || n <= 0)
		return;

	Directory_list *temp = *head_ref;
	int i;

	for (int i = 1; temp != NULL && i < n; i++)
		temp = temp->next;

	if (temp == NULL)
		return;
	rm_Dir(head_ref, temp);
}

int main(int argc, char const *argv[])
{

	char comanda[20];
	char filename[20];
	char content[50];
	char dirname[20];
	char *token;
	char *aux[50];

	Directory *root = NULL;
	Directory *current_Dir;
	Directory_list *head_Dir = NULL;
	File_list *head_File = NULL;

	int i = 0, k;

	do {
		fgets(comanda, 20, stdin);
		comanda[strlen(comanda)-1] = '\0';

		if (strcmp(comanda, "create fs") == 0) {
			root = create_fs("/");
			current_Dir = root;
		}
		if (strcmp(comanda, "delete fs") == 0) {
			free(root);
			return 0;
		}
		if (strstr(comanda, "mkdir") != NULL) {
			token = strtok(comanda, " ");
			k = 0;
			while (token != NULL) {
				aux[k++] = token;
				token = strtok(NULL, " ");
			}

			strcpy(dirname, aux[1]);

			Directory_list *newDir = getDir(dirname, current_Dir);

			head_Dir = current_Dir->childDir_list;

			add_Dir(&head_Dir, newDir, &current_Dir);

		}
		if (strstr(comanda, "touch") != NULL) {
			token = strtok(comanda, " ");
			k = 0;
			while (token != NULL) {
				aux[k++] = token;
				token = strtok(NULL, " ");
			}
			strcpy(filename, aux[1]);
			strcpy(content, aux[2]);
			File_list *newFile =
					getFile(filename, content, current_Dir);

			add_File(&head_File, newFile, &current_Dir);

		}
		if (strstr(comanda, "ls") != NULL) {
			LS(current_Dir);
			printf("\n");
		}
		if (strstr(comanda, "pwd") != NULL)
			PWD(current_Dir);

		if (strstr(comanda, "cd") != NULL) {
			token = strtok(comanda, " ");
			k = 0;
			while (token != NULL) {
				aux[k++] = token;
				token = strtok(NULL, " ");
			}
			strcpy(dirname, aux[1]);
			current_Dir =  CD(current_Dir, dirname);

		}
		// if (strstr(comanda, "tree") != NULL)
		//	TREE(current_Dir);

		if (strstr(comanda, "rm") != NULL) {
			token = strtok(comanda, " ");
			k = 0;
			while (token != NULL) {
				aux[k++] = token;
				token = strtok(NULL, " ");
			}
			if (strcmp(aux[0], "rmdir") == 0) {
				strcpy(dirname, aux[1]);
				int pos_dir =
					GetDirPosition(current_Dir, dirname);

				rmdir(&current_Dir->childDir_list, pos_dir);
			} else if (strcmp(aux[0], "rm") == 0) {
				strcpy(filename, aux[1]);
				int pos =
					GetFilePosition(current_Dir, filename);

				rm(&current_Dir->childFile_list, pos);
			}
		}
	} while (strcmp(comanda, "delete fs") != 0);

}

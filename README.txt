The goal of this theme is to implement a minimal file system with lots of
operations associated with the CLI, very similar to those on the terminal of a UNIX system, to
manipulate that file system. Its structure will be similar to that of a system
UNIX-based, by dividing into files and directories, starting from the root directory [ ​ 0]
The program reads the commands touch, mkdir, cd, ls, pwd, rm and rmdir until delete
f.s.

Filesystem Minishell Tema 1 - SD - DIGORI PARASCOVIA - 313CC

Tema este incompleta ruleaza pe 65 de puncte doar.

Scopul acestei teme​ este de a implementa un sistem de fişiere minimal, cu o mulțime de
operaţii asociate de CLI, foarte similare cu cele de pe terminalul unui sistem UNIX, pentru a
manipula respectivul sistem de fişiere. Structura acestuia va fi similară cu cea de pe un sistem
UNIX-based, prin împărţirea în fişiere şi directoare, plecându-se de la directorul rădăcină [ ​ 0]


Programul citeste comenzile touch, mkdir, cd, ls, pwd, rm si rmdir pana la delete 
fs. Am functia create fs in care aloc memorie pentru directorul root si setez 
directorul curent (current_Dir = root). Prin functiile getDir si getFile creez
nodul care apoi se adauga in lista respectiva comenzii mkdir (functia add_Dir) 
sau touch (functia add_File). Ambele functii adauga un nod in lista in oridine 
lexicografica, prin pozitionare nodului inainte punctului de intersectie. Adaug 
lista directoare/files in lista respectiva de copii a directorului curent.

Functia LS primeste ca parametru current_Dir si apeleaza 2 functii auxiliare de 
printare a listei de directoare si listei de fisiere, ce sunt copii directorului 
curent. 

Functia CD identifica daca parametrul comenzii cd este ".."  sau altceva(denumirea 
directorului) si in functie de asta actioneaza. Daca  e ".." directorul current_Dir 
devine egal cu parintele fostului director curent si in asa mod mergem in spate, iar 
pentru a merge in fata, se face o cautare dupa nume in toata lista de copii directoare, 
daca se gaseste atunci current_Dir devine directorul cu acel nume din lista sa de copii. 
Aici exista o maricica problema dupa consecutive apeluri cd .. si apoi reintoarcerea cu 
mai multe nivele in sus cd x si tot asa, ca apoi sa revin dinnou in jos mi se pierd 
legaturile cu copii directoarelor curente, astfel nu trece testul 9 si nici nu pot 
implementa tree, care depinde total de functionalitatea asta. Chiar nu mai stiu cum sa 
repar asta.

Functia PWD parcurge toti parintii directorului curent si pastreasa numele lor intr-un 
array name_of_Dirs char si apoi (pentru a le afisa in ordine de la root) parcurg de 
la finalul arrayului inapoi.

Functiile de rm si rmdir sunt asemanatoare. Voi explica pe exemplul rm: cu functia 
GetFilePosition parcurg toata lista de fisiere copii a directorului curent si gasesc 
pozitia la care se afla fisierul dorit sau afisez mesaj de eroare. In functia rm 
traversez pana la pozitia gasita a nodului in lista si apelez functia rm_File(care 
sterge nodul si elibereaza memoria). La rmidir eliberez memoria si in lista de fisiere 
si directoare copii a directorului care vreau sa-l sterg.

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define N 3

int main(){

    /* CREATING DIRECTORY 'TMP' */
    mkdir("./tmp", 0777);

    /* ALL FILES WE WORK WITH*/
    char* filenames[] = {
        "./tmp/file1",
        "./tmp/file2",
        "./tmp/link1"
    };
    
    /* CREATING file1, file2; ADDING LINK TO file1 */
    FILE* file0 = fopen(filenames[0], "w");
    FILE* file1 = fopen(filenames[1], "w");
    link(filenames[0], filenames[2]);
    fclose(file0);
    fclose(file1);

    /* GETTING INFO ABOUT FILES */
    int fd[N];
    for (int i = 0; i < N; i++){
        fd[i] = open(filenames[i], O_RDONLY);
    }
    struct stat fileStat[N];
    for (int i = 0; i < N; i++){
        fstat(fd[i], &fileStat[i]);
    }

    /* 
    IF FILE WAS CHECKED FOR EXISTANCE OF LINKS TO IT THEN
    checked[i] = 1 ELSE 0
    */
    int checked[N];
    for (int i = 0; i < N; i++){
        checked[i] = 0;
    }

    FILE* ex4 = fopen("ex4.txt", "w"); // OUTPUT FILE
    
    /* 
    CHECKING FILES ONE BY AND 
    COMPARING ITS INUM WITH OTHER FILES 
    */
    for (int i = 0; i < N - 1; i++){
        if (checked[i] == 1) continue;

        char* res[N] = {filenames[i]}; // FILES WITH THE SAME INUM
        int idx = 1;
        checked[i] = 1;
        for (int j = i + 1; j < N; j++){
            if (fileStat[i].st_ino == fileStat[j].st_ino){
                checked[j] = 1;
                res[idx] = filenames[j];
                idx++;
            }
        }

        if (idx > 1){
            fprintf(ex4, "%ld ", fileStat[i].st_ino);
            for (int i = 0; i < idx; i++) fprintf(ex4, "%s ", res[i]);
        }
    }
    fclose(ex4);

    return 0;
}
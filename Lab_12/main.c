#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char * argv[]) {
    // В текущей папке создать папки с именами, указанными в качестве параметров командной
    // строки. Создать текстовый файл, в который поместить информацию о списке подпапок
    // текущей папки.

    FILE *file;
    file = fopen("list_of_dirs.txt", "w");

    int i;
    for( i = 1 ; i < argc; i++) {
        mkdir(argv[i], 0777);
    }


    // Copied from net
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            // Debug
            // printf("%s     type: ", dir->d_name);
            // printf("%d\n", dir->d_type == DT_DIR);
            if (dir -> d_type == DT_DIR) {
                fprintf(file, "%s\n", dir -> d_name);
            }
        }
        closedir(d);
    }

    fclose(file);
    return 0;
}
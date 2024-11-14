#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>




// File utils
void read_file(char* file_path, char* content);
void write_file(char* file_path, char* content);

void incr_patch_version(char* file_contents, char* res){
    char* token = strtok(file_contents, "\n");
    while(token != NULL) {
        if (strncmp(token, "patch=", 6) == 0) {
            char* s = strrchr(token, '=');
            s++;
            int v = atoi(s);
            v++;
            sprintf(res + strlen(res), "patch=%d\n", v);
            printf("patch = %d 🡒 %d\n", v-1,v);
        } else {
            // Add to result
            strcat(res, token);
            strcat(res, "\n");
        }

        // Find next token
        token = strtok(NULL, "\n");
    }
}

void incr_minor_version(char* file_contents, char* res){
    char* token = strtok(file_contents, "\n");
    while(token != NULL) {
        if (strncmp(token, "minor=", 6) == 0) {
            char* s = strrchr(token, '=');
            s++;
            int v = atoi(s);
            v++;
            sprintf(res + strlen(res), "minor=%d\n",v);
            printf("minor = %d 🡒 %d\n", v-1,v);
        } else if (strncmp(token, "patch=", 6) == 0) {
            char* s = strrchr(token, '=');
            s++;
            int v = atoi(s);
            if (v != 0) {
                printf("patch = %d 🡒 0\n", v);
                v = 0;
            }
            sprintf(res + strlen(res), "patch=%d\n",v);
        } else {
            // Add to result
            strcat(res, token);
            strcat(res, "\n");
        }

        // Find next token
        token = strtok(NULL, "\n");
    }

}


int main(int argc, char* argv[]) {

    if (argc > 2) {
        printf("Usage: %s [file_path]\n", argv[0]);
        return 1;
    }


    // Try to determine path to version.txt
    char* file_path;
    if (argc == 1) {
        file_path = "version.txt";    
        if (access(file_path, R_OK) != 0) {
            printf("version.txt not found in current directory.\n");
            exit(1);
        }
    } else {
        file_path = argv[1];
        if (access(file_path, R_OK) != 0) {
            printf("path='%s' not accessible\n", file_path);
            exit(1);

        }
    }

    // Print file path
    char actualpath [100];
    realpath(file_path, actualpath);
    printf("Version file: %s\n", actualpath);


    // Get some memory
    char* content = malloc(100);
    memset(content, 0, 100);


    // Read in file into memory
    read_file(file_path, content);


    // Allocate memory for final result
    char* res = malloc(100);
    memset(res, 0, 100);


    // Increment version
    incr_patch_version(content, res);


    // Write final contents to file
    write_file(file_path, res);


    // free allocated mem
    free(content);
    free(res);
}




void read_file(char* file_path, char* content){

    FILE *fptr;
    char buff[100];

    fptr = fopen(file_path, "r");
    while(fgets(buff, 100, fptr)) {
        strcat(content, buff);
    }
    fclose(fptr);
}

void write_file(char* file_path, char* content){
    FILE *fptr;
    fptr = fopen(file_path, "w");
    fprintf(fptr, "%s", content);
    fclose(fptr);
}
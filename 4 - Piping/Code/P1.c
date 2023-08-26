#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "f.h"

int main(void)
{
    // Make a pipe.
    mkfifo("Pipe1", 0666);

    // Open the pipe for writing to it.
    int pipe1_fd = open("Pipe1", O_WRONLY);

    // Open the `/home` directory to read the files in it.
    DIR *d = opendir("/home");

    // Check for validity of the pointer returned by `opendir`.
    if (!d)
    {
        fprintf(stderr, "Wasn't able to read the content of the `/home` directory.\n");
        return EXIT_FAILURE;
    }

    // Start reading the files in the `/home` path.
    // Open the ones that have a name that starts with a dollar sign.
    // Read the contents of those files and send them to the pipe along with their name. 
    text_file f;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_name[0] == '$')
        {
            sprintf(f.fpath, "/home/%s", dir->d_name);

            FILE *input_file = fopen(f.fpath, "r");
            fgets(f.contents , BUFFER_SIZE, input_file);
            
            write(pipe1_fd, &f, sizeof(text_file));

            printf("Sent the contents of %s to `P2`.\n", f.fpath);

            fclose(input_file);
        }
    }

    // When we were done reading files, send a dummy struct containing an `fpath`
    // with its first element set to NUL character.
    // This is propegated through all of the successor programs and is checked for
    // as a condition for ending the program.
    f.fpath[0] = '\0';
    write(pipe1_fd, &f, sizeof(text_file));


    closedir(d);

    close(pipe1_fd);

    return EXIT_SUCCESS;
}
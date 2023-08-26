#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "f.h"

int main(void)
{
    // Make a pipe.
    mkfifo("Pipe3", 0666);

    // Open the pipe that comes from `P2` and the one that goes to `P4`.
    int pipe2_fd = open("Pipe2", O_RDONLY),
        pipe3_fd = open("Pipe3", O_WRONLY);

    // Read the stuff that are comming from the pipe.
    // Swap the case of the alphabetic characters.
    // Break out of the loop in case the terminating struct is recieved.
    text_file f;
    while (true)
    {
        read(pipe2_fd, &f, sizeof(text_file));

        if (f.fpath[0] == '\0')
            break;

        swap_case(f.contents, 0, strlen(f.contents));

        printf("%s: %s", f.fpath, f.contents);

        write(pipe3_fd, &f, sizeof(text_file));
    }

    // Send the terminating struct to the `P3`.
    f.fpath[0] = '\0';
    write(pipe3_fd, &f, sizeof(text_file));

    close(pipe2_fd);
    close(pipe3_fd);

    return EXIT_SUCCESS;
}
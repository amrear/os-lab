#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "f.h"

int main(void)
{
    // Make a pipe.
    mkfifo("Pipe2", 0666);

    // Open the pipe that comes from `P1` to read its contents 
    // and also open a pipe that sends data to `P3`.
    int pipe1_fd = open("Pipe1", O_RDONLY),
        pipe2_fd = open("Pipe2", O_WRONLY);

    // Read the stuff that are comming from the pipe.
    // Swap the asterisks and the full stops and vice versa.
    // Break out of the loop in case the terminating struct is recieved.
    text_file f;
    while (true)
    {
        read(pipe1_fd, &f, sizeof(f));

        if (f.fpath[0] == '\0')
            break;

        for (int i = 0; i < BUFFER_SIZE; i++)
        {            
            if (f.contents[i] == '*')
                f.contents[i] = '.';
            else if (f.contents[i] == '.')
                f.contents[i] = '*';
        }
        
        printf("%s: %s", f.fpath, f.contents);

        write(pipe2_fd, &f, sizeof(text_file));
    }

    // Send the terminating struct to the `P3`.
    f.fpath[0] = '\0';
    write(pipe2_fd, &f, sizeof(text_file));

    close(pipe2_fd);
    close(pipe1_fd);

    return EXIT_SUCCESS;
}
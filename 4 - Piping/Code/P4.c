#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "f.h"

int main(void)
{
    // Open the pipe that comes from `P3`.
    int pipe3_fd = open("Pipe3", O_RDONLY);


    // Read the stuff that are comming from the pipe.
    // Write the data to the output file with appropriate name.
    // Break out of the loop in case the terminating struct is recieved.
    text_file f;
    while (true)
    {
        read(pipe3_fd, &f, sizeof(text_file));

        if (f.fpath[0] == '\0')
            break;

        // Find the last forward-slash in the input file path.
        int i;
        for (i = strlen(f.fpath) - 1; f.fpath[i] != '/'; i--)
            ;

        // Swap the case of alphabetic characters of the path after
        // the last forward-slash designates the filename.
        swap_case(f.fpath, i, strlen(f.fpath));


        // Find the last dot in the file path this signifies the file extension
        // and the content after it must be replaced by `new`.
        for (i = strlen(f.fpath) - 1; f.fpath[i] != '.' && i != 0; i--)
            ;

        if (i)
            strcpy(f.fpath + i + 1, "new");
        else
            strcpy(f.fpath + strlen(f.fpath), ".new");

        printf("File was written to %s.\n", f.fpath);

        FILE *output_file = fopen(f.fpath, "w");
        fprintf(output_file, "%s", f.contents);
        fclose(output_file);
    }

    close(pipe3_fd);

    return EXIT_SUCCESS;
}
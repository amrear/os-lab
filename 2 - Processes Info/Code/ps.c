#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Padding in the table.
#define PID_PADDING "15"
#define EXENAME_PADDING "50"
#define STATUS_PADDING "10"
#define PPID_PADDING "10"
#define MEM_DEMAND_PADDING "15"
#define MEM_ASSIGN_PADDING "15"

// Filenames.
#define PROCS_TMP_FILE "p.txt"
#define OUTPUT_FILE "procs.txt"

void read_exename(FILE *f, char *d);

int main(void)
{
    // Run `ls` command on the `/procs` directory and store the results in a temporary file.
    // This temporary file will be usefull in the future as we probe every process one by one.
    system("ls /proc > " PROCS_TMP_FILE);
    
    FILE *procs = fopen(PROCS_TMP_FILE, "r");
    FILE *results = fopen(OUTPUT_FILE, "w");

    fprintf(results, "%-" PID_PADDING "s", "pid");
    fprintf(results, "%-" EXENAME_PADDING "s", "exename");
    fprintf(results, "%-" STATUS_PADDING "s", "status");
    fprintf(results, "%-" PPID_PADDING "s", "ppid");
    fprintf(results, "%-" MEM_DEMAND_PADDING "s", "mem_demand");
    fprintf(results, "%-" MEM_ASSIGN_PADDING "s", "mem_assigned");
    fprintf(results, "\n");

    for (int i = 0; i < 115; i++)
        fprintf(results, "-");

    fprintf(results, "\n");

    while (!feof(procs))
    {
        char pid[243];

        fscanf(procs, "%s", pid);
        
        // For every file in `/procs` check if it's for a process or not.
        // the filename must entirely consist of digits if it's related to a process.
        // If true, attempt to extract its information.
        if (atoi(pid))
        {
            char stat_path[255], statm_path[255];

            sprintf(stat_path, "/proc/%s/stat", pid);
            sprintf(statm_path, "/proc/%s/statm", pid);

            FILE *stat = fopen(stat_path, "r");            
            FILE *statm = fopen(statm_path, "r"); 

            // Check for the validity of the pointers returned by `fopen`.
            // if they are `NULL`, the process was closed before we could scrape it's information.
            if (stat == NULL && statm == NULL)
            {
                fprintf(results, "%-15s", pid);
                fprintf(results, "Process has been terminated.\n");

                continue;
            }

            char s[63];

            // Extract the information regarding each process and write it to the output file in a nice format.
            fscanf(stat, "%s", s);
            fprintf(results, "%-" PID_PADDING "s", s);
            read_exename(stat, s);
            fprintf(results, "%-" EXENAME_PADDING "s", s);
            fscanf(stat, "%s", s);
            fprintf(results, "%-" STATUS_PADDING "s", s);
            fscanf(stat, "%s", s);
            fprintf(results, "%-" PPID_PADDING "s", s);
            fscanf(statm, "%s", s);
            fprintf(results, "%-" MEM_DEMAND_PADDING "s", s);
            fscanf(statm, "%s", s);
            fprintf(results, "%-" MEM_ASSIGN_PADDING "s", s);
            fprintf(results, "\n");    

            fclose(stat);  
            fclose(statm);  
            
        }

    }

    fclose(procs);
    fclose(results);

    system("rm -f " PROCS_TMP_FILE);
}

// Read the name of process in the `stat` file.
// It's not possible to use `scanf` for the executable names
// since some of them contain whitespaces and are designated with parentheses.
void read_exename(FILE *f, char *d)
{
    char c;
    while ((c = fgetc(f)) != '(')
        ;

    int i = 0;
    while ((c = fgetc(f)) != ')')
        d[i++] = c;

    d[i] = '\0';
}
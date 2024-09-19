#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Function Prototypes
void read_file(const char *input_file, char *buffer);
void transform(const char *filter, char *buffer);
void write_file(const char *output_file, const char *buffer);

// Program which transforms data from an input file and writes it onto an output file
int main(int argc, char *argv[])
{
    int         opt;                           // Captures the result of getopt
    const char *input_file          = NULL;    // Stores location of input file
    const char *output_file         = NULL;    // Stores location of output file
    const char *filter              = NULL;    // User-selected transformation filter
    char        buffer[BUFFER_SIZE] = {0};     // Buffer for storing the read data to transform and write

    printf("***\tFILE TRANSFORMER\t***\n\n");

    // Parse Arguments from the Command Line
    while((opt = getopt(argc, argv, "i:o:f:")) != -1)
    {
        switch(opt)
        {
            case 'i':
                input_file = optarg;
                printf("Input file accepted: \t%s\n", input_file);
                break;
            case 'o':
                output_file = optarg;
                printf("Output file accepted: \t%s\n", output_file);
                break;
            case 'f':
                filter = optarg;
                printf("Chosen filter: \t\t%s\n", filter);
                break;
            default:
                printf("Error: values not accepted\n");
                return EXIT_FAILURE;
        }
    }

    // Check if the input and output files loaded correctly
    if(input_file == NULL || output_file == NULL || filter == NULL)
    {
        return EXIT_FAILURE;
    }

    printf("\nProgram started... \n\n");

    // Read the input file and copy contents into a buffer
    read_file(input_file, buffer);
    printf("Data from input file: \n%s\n", buffer);
    // Apply the selected transformation onto the data in the buffer
    transform(filter, buffer);
    // Write the transformed buffer data onto the output file
    write_file(output_file, buffer);
    printf("Data to be output: \n%s\n", buffer);

    return EXIT_SUCCESS;
}

// Open input file and copy data into buffer
void read_file(const char *input_file, char *buffer)
{
    ssize_t bytes_read;
    int     fd = open(input_file, O_RDONLY | O_CLOEXEC);

    if(fd == -1)
    {
        exit(EXIT_FAILURE);
    }

    bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
    if(bytes_read == -1)
    {
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    close(fd);
}

// Converts buffer text to upper or lowercase if either is chosen, otherwise do nothing
void transform(const char *filter, char *buffer)
{
    if(strcmp(filter, "upper") == 0)
    {
        for(int i = 0; buffer[i] != '\0'; ++i)
        {
            buffer[i] = (char)toupper((unsigned char)buffer[i]);
        }
    }
    else if(strcmp(filter, "lower") == 0)
    {
        for(int i = 0; buffer[i] != '\0'; ++i)
        {
            buffer[i] = (char)tolower((unsigned char)buffer[i]);
        }
    }
    else
    {
     	printf("\n\n***\tWARNING: No filter applied. Output text same as input text.\t***\n\n");
}

// Open input file and copy data into buffer
void write_file(const char *output_file, const char *buffer)
{
    ssize_t bytes_written;
    int     fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC);

    if(fd == -1)
    {
        exit(EXIT_FAILURE);
    }

    bytes_written = write(fd, buffer, strlen(buffer));
    if(bytes_written == -1)
    {
        exit(EXIT_FAILURE);
    }
    close(fd);
}

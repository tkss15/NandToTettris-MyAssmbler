#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum instructions
{
    // creating menu options with enum.
    dest, // 0
    jump, // 1
    comp, // 2
    MAX_INSTRUCTIONS
} instructions;

typedef struct string_int
{
    // creating menu options with enum. 
    char string[4];
    int postion;
} string_int;

int get_index_of_element(char* string, int len, char a);
char* strsub(char* string, int start, int end);
void remove_whitespace(char* string);
void Error_Msg(char* msg);
void Assmbler(FILE* input, FILE* output);
void Bit_Print(unsigned int ch, FILE* output);

int main()
{
    FILE* in, * out;
    int find_dot;
    char fileName[20], outputfile[20], hack[5] = "hack\0";

    printf("Please Enter assembly file name:");
    scanf("%s", &fileName);

    in = fopen(fileName, "rt");
    if (in == NULL)
        Error_Msg("input file is wrong");

    /*
    Changing the end of file name from .asm to .hack
    */
    find_dot = get_index_of_element(fileName, strlen(fileName), '.') + 1;
    strcpy(outputfile, fileName);
    for (int i = 0; i < strlen(hack); i++)
    {
        outputfile[i + find_dot] = hack[i];
    }
    outputfile[strlen(hack) + find_dot] = '\0';

    out = fopen(outputfile, "w");
    Assmbler(in, out);
    printf("Output file %s is now available.", outputfile);
}

void Assmbler(FILE* input, FILE* output)
{
    /*
        function getting 2 files. input file and ouput file.
        the function decoding the input file and wirtes the binary values in the output file.
    */
    int AInsturction, counter, Tempostion;
    int i, k;
    char CInsturction[18] = "111", * Tempstring, stringfile[80];
    string_int main_array[3]; // stores all the information about (dest,comp,jump)

    // creating a 3 dimensional array for both dest,jump
    char* arrayInstrctions[2][8][2] = {
        {// Matrix for values of dest
         // First Row - Command     Second Row - Value
         {"", "000"},
         {"M", "001"},
         {"D", "010"},
         {"DM", "011"},
         {"A", "100"},
         {"AM", "101"},
         {"AD", "110"},
         {"ADM", "111"}},
        {// Matrix for values of Jump
         // First Row - Command     Second Row - Value
         {"", "000"},
         {"JGT", "001"},
         {"JEQ", "010"},
         {"JGE", "011"},
         {"JLT", "100"},
         {"JNE", "101"},
         {"JLE", "110"},
         {"JMP", "111"}} };
    // creating a 2 dimensional array for comp
    char* arr[28][2] = {
        // First Row - Command, Second Row - Binnary Value.
        {"0", "0101010"},
        {"1", "0111111"},
        {"-1", "0111010"},
        {"D", "0001100"},
        {"A", "0110000"},
        {"M", "1110000"},
        {"!D", "0001101"},
        {"!A", "0110001"},
        {"!M", "1110001"},
        {"-D", "0001111"},
        {"-A", "0110011"},
        {"-M", "1110011"},
        {"D+1", "0011111"},
        {"A+1", "0110111"},
        {"M+1", "1110111"},
        {"D-1", "0001110"},
        {"A-1", "0110010"},
        {"M-1", "1110010"},
        {"D+A", "0000010"},
        {"D+M", "1000010"},
        {"D-A", "0010011"},
        {"D-M", "1010011"},
        {"A-D", "0000111"},
        {"M-D", "1000111"},
        {"D&A", "0000000"},
        {"D&M", "1000000"},
        {"D|A", "0010101"},
        {"D|M", "1010101"}
    };

    // Looping until the file is ended. each loop storing a 80 chars into stringfile.
    while (fgets(stringfile, 80, input) != NULL)
    {
        remove_whitespace(stringfile); // calling the function to remove spaces in the string.
        counter = 3;               // reseting the counter to 3
        // Checking if encountering a comment or an empty line. then move to next line.
        if (stringfile[0] == '/' && stringfile[1] == '/' || stringfile[0] == '\n')
        {
            continue;
        }
        // Checking if encountering an A instruction.
        else if (stringfile[0] == '@')
        {
            AInsturction = atoi(stringfile + 1); // Converting the string into number
            Bit_Print(AInsturction, output);     // printing the binary value to the file.
        }
        else
        { // encountering C instruction
            // Setting all values of dest comp and jump to be empty("").
            for (int i = 0; i < MAX_INSTRUCTIONS; i++)
                strcpy(main_array[i].string, ""); // Using strcpy in order to set them to "".

            // Setting the postion of both dest and jump to be the returned value of the searched index.
            main_array[dest].postion = get_index_of_element(stringfile, strlen(stringfile), '=');
            main_array[jump].postion = get_index_of_element(stringfile, strlen(stringfile), ';');

            // Saving the end of line postion with Tempostion.
            Tempostion = get_index_of_element(stringfile, strlen(stringfile), '\n');

            // Copying the dest from the string. only if there is '=' sign in the main string.
            if (main_array[dest].postion != -1) // if equal to -1 there is no '=' in the string.
            {
                // copying the substring from the postion of index 0( beginning ) to the postion of dest.
                Tempstring = strsub(stringfile, 0, main_array[dest].postion);
                strcpy(main_array[dest].string, Tempstring);
            }
            // Copying the jump from the string. only if there is ';' sign in the main string.
            if (main_array[jump].postion != -1)
            {
                // copying the substring from the postion of jump + 1(not including ';') to the end of line.
                Tempstring = strsub(stringfile, main_array[jump].postion + 1, Tempostion);
                strcpy(main_array[jump].string, Tempstring);
            }
            else
            {
                // setting the value of jump postion in case of not found.
                main_array[jump].postion = Tempostion;
            }
            // copying the substring from the postion of dest + 1(not including '=') to the end of line.
            Tempstring = strsub(stringfile, main_array[dest].postion + 1, main_array[jump].postion);
            strcpy(main_array[comp].string, Tempstring);

            // Looping through all commands of comp
            for (i = 0; i < 28; i++)
                // checking if a command is eqaul to our comp string.
                if (strcmp(main_array[comp].string, arr[i][0]) == 0)
                    // copy the binnary value of the curret comp to its currect postion.
                    strcpy(CInsturction + counter, arr[i][1]);
            counter = counter + 7;

            // looping thorugh all commands of dest and jump togather( k = 0 dest, k = 1 jump )
            for (k = 0; k < 2; k++)
            {
                for (i = 0; i < 8; i++)
                {
                    if (strcmp(main_array[k].string, arrayInstrctions[k][i][0]) == 0)
                    { // checking if a command is equal to our dest\jump string.
                        strcpy(CInsturction + counter, arrayInstrctions[k][i][1]);
                        counter = counter + strlen(arrayInstrctions[k][i][1]); // updating the counter
                    }
                }
            }
            CInsturction[17] = '\0';
            fprintf(output, "%s\n", CInsturction);
        }
    }
    fclose(output);
}

void remove_whitespace(char* string)
{
    /*
        function getting a string and removes the spaces in the string.
        using 2 pointer to a string and matching the value of the pointers
        if one pointer is a space then we increse it immidiatly.
    */
    char* p_string = string;
    do
    {
        while (*p_string == ' ')// if found white space we wont to avoid
        {
            ++p_string;// increse the pointer
        }
    } while (*string++ = *p_string++);
}

void Error_Msg(char* msg)
{
    /*
        function getting a string and prints the string and then exit the program
    */
    printf("\n%s", msg);
    exit(1);
}

int get_index_of_element(char* string, int len, char a)
{
    /*
        function getting a 3 parameters string the length of the string and a char.
        function searchs for the char if the char is inside the string the postion of the char is returned
        else returns -1
    */
    for (int i = 0; i < len; i++)
    {
        if (string[i] == a) // if we found the char
            return i;       // return postion
    }
    return -1;
}

void Bit_Print(unsigned int x, FILE* output)
{
    /*
        function getting an unsigned intger the fucntion will print the intger in a binary values.
    */
    int i;
    unsigned int mask = 1;                // setting mask to be 1
    mask = mask << (sizeof(int) * 4 - 1); // changing the max to be the MSB 1.....0
    // looping from 0 to the size of max intger size
    for (i = 0; i < sizeof(int) * 4; i++)
    {
        fprintf(output, "%d", (x & mask) ? 1 : 0); // checking if x AND mask is true if it is print 1 if not print 0
        mask = mask >> 1;                          // moving the mask 1 time to the right
    }
    fprintf(output, "\n");
}


char* strsub(char* string, int start, int end)
{
    /*
        function getting a string and 2 postions( start and end )
        the function returns a string between the start and end postion
    */
    char substring[4]; // creating the substring( maximum size is 3 ).
    int counter = 0, i;
    if (start == -1) // checking if there is no start postion entered.
        start = 0;

    if (end == -1) // checking if there is no end postion entered
        end = strlen(string);

    for (i = start; i < end; i++)
    {
        substring[counter] = string[i];
        counter = counter + 1;
    }
    substring[counter] = '\0'; // ending the string.
    return substring;
}

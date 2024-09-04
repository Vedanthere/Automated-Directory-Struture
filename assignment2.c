#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void createDirectories(char *line, FILE *logInfo) //line reads from the file and log file to write in
{
    char* position = strtok(line, ",");  //pointer to the first index in the line
    char root[256] = "/home/vedant/Downloads";  
    char directoryPath[256] = "";     //stores the path of each direcotories
    int counter = 0;
    char username[50] = "";

    strcpy(directoryPath, root);     //copy the root path 

    time_t current_time; //declaration of current_time variable
    time(&current_time); //get the current system time

    while (position != NULL)     //go through each word from the line
    {
        while (*position == ' ' || *position == '\t')   //tracking the whitespace from the user.txt file
        {
            position++;
        }

        char* end = position;  //end pointer to point to the same position 
        while (*end != '\0')   
        {
            end++;   //next string
        }

        while (end > position && end[-1] <= ' ')    //-1 moves back one character in the string
        {
            end--;     //backwards until it reaches the position pointer (next character)
        }

        *end = '\0';   //terminate the index/word

        if(counter == 1) 
        {
            strcat(username, position);   //combinning the index to the username string. 
        }

        if (strcmp(position, "profile") != 0)   //checks for profile file, if not then create directories
        {
            strcat(directoryPath, "/");       //appends with "/" to the string
            strcat(directoryPath, position);                       //append index to directory path
            if (access(directoryPath, F_OK) != 0)               // Check if any of the directory already exists like admin, user
            {
                if (mkdir(directoryPath, 0777) == -1)            //creating directory with all permissions 
                {
                    perror("mkdir error");
                }
                else 
                {
                    char* dirName = strrchr(directoryPath, '/');    //take the directory name from the path
                    if (dirName != NULL) 
                    {
                        dirName++;                              //moves to next one after the '/'
                        fprintf(logInfo, "%s %s directory created!\n\n", asctime(localtime(&current_time)), dirName);
                        printf("%s directory created!\n", dirName);      // Print on the terminal
                    }
                }
            }
            counter++;
        } 
        else 
        {
            strcat(directoryPath, "/profile.txt");   //checks if profile text encountered
            FILE* file = fopen(directoryPath, "w");   //writing permission for the profile.txt file
            if (file != NULL) 
            {
                struct tm *local_time = localtime(&current_time);    //for local system time
                fprintf(file, "%s profile created on %s\n\n", username, asctime(local_time));  //write the profile creation deatils in the file
                fclose(file);   
                if (chmod(directoryPath, S_IRUSR | S_IRGRP) == -1)      //change file to read only 
                {
                    perror("permission error");
                }
                fprintf(logInfo, "%s %s profile text file created!\n\n", asctime(localtime(&current_time)), username);
                printf("%s profile text file created!\n", username);            //profile txt file message on terminal
                printf("Permission for file at %s set to read only!\n", directoryPath);  //permission message on terminal
            } 
            break;
        }
        
        position = strtok(NULL, ",");   //go to the next one, based onn the commas
    }
}

void readFile(const char *filename, FILE *logInfo) 
{
    FILE *file = fopen(filename, "r");   //read only
    if (file == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        createDirectories(buffer, logInfo);   //loop through each line
    }

    fclose(file);
}

int main() 
{
    const char *filename = "users.txt";    //initialize the users.txt file
    FILE *logInfo = fopen("logs.txt", "a"); //append mode
    if (logInfo == NULL) 
    {
        printf("Error creating log file!\n");
        return 1;
    }

    time_t current_time;       //system date and time
    struct tm *local_time;
    time(&current_time);
    local_time = localtime(&current_time);

    fprintf(logInfo, "%s Reading data from the text file...\n\n", asctime(local_time));    //print in the log file
    readFile(filename, logInfo);                 
    fprintf(logInfo, "%s Done reading data from the text file...\n\n", asctime(local_time));  

    fclose(logInfo);  //close the log file
    return 0;
}
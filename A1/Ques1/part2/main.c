#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>


int acnt = 0;
int bcnt = 0;
struct Student {
    int id;
    char section;
    int marks[6];
};

int parseStudentValues(struct Student *student, char *ptr_str, float arr[]) {
    char *ptr_token;
    ptr_token = strtok(ptr_str, ",");
    if(ptr_token == NULL){
        return -1;
    }
    
    student->id = atoi(ptr_token);
    ptr_token = strtok(NULL, ",");

    if(ptr_token == NULL){
        return -1;
    }

    student->section = ptr_token[0];

    for(int i=0; i<5; ++i) {
        
        ptr_token = strtok(NULL, ",");
        
        if(ptr_token == NULL){
            return -1;
        }
        student->marks[i] = atoi(ptr_token);
        //arr[i] = arr[i] + (float)student->marks[i];
    }
    int len_str = strlen(ptr_token)+1;
    student->marks[5] = atoi(ptr_token + len_str);
    //arr[5] = arr[5] + (float)student->marks[5];
    return 0;
}
void printString(char *str) {
    int len = strlen(str);
    // printSize(str, len);
    syscall(1, STDOUT_FILENO, str, len);
}

void printInt(int number) {
    char buffer[20];
    sprintf(buffer, "%d", number);
    printString(buffer);
}

void printFloat(float input) {
    char buffer[100];
    sprintf(buffer, "%f", input);
    printString(buffer);
}


void printStudentDetails(struct Student *student, float arr[]) {
    // to be hold in buffer
    //printString("Student ID: ");
    //printInt(student->id);
    //printString("\t");
    char temparr[2];
    temparr[1] = '\0';
    temparr[0] = student->section;
    //printString("Section: ");
    //printString(temparr);
    //printString("\t");
    float avg = 0;
    //printString("Marks in : ");
    for(int i=0; i<6; ++i) 
    {
        //printInt(student->marks[i]);
        //printString("\t");
        avg = avg + (float)student->marks[i];
        arr[i] = arr[i] + (float)student->marks[i];
       
    }
    //printString("Average Marks: ");
    //printFloat(avg/6);
    //printString("\n");
}
void printAssignmentDetails(float arr[], int total_number, char *section){
    for(int i=0; i<6; ++i) 
    {
        float temp = (float)(arr[i] / total_number);
        printString("Average marks in (Section - ");
        printString(section);
        printString(")");
        printString(" Assignment: ");
        printInt(i+1);
        printString(" = ");
        printFloat(temp);
        printString("\n");
    }
}

int readFileByLine(int fd, char *buffer_str, int buffer_size) {
    int cnt=0;
    memset(buffer_str, 0, 100);
    while(read (fd, &buffer_str[cnt], 1) >= 0 && cnt < buffer_size-1 && errno != EINTR) {
        if(buffer_str[cnt] == '\n') {
            buffer_str[cnt] = '\0';
            return 1;
        }
        cnt++;
        errno = 0;
    }
    return 0;
}
void * ThreadFuncForSectionA(void * arr){
    
    int fd  = open("student_record.csv", O_RDONLY);
    char buffer_str[100];
    //opening of the csv file failed.
    if(fd < 0) 
    {
        fprintf(stderr, "Error while opening file! (check if file exists in the right directory)\n");
        exit(0);
    }

    readFileByLine(fd, buffer_str, 100);
    //float arr[6] = {0};

    while(readFileByLine(fd, buffer_str, 100) > 0) 
    {
        
        struct Student student;
        parseStudentValues(&student, buffer_str, arr);
        if(student.section == 'A'){
            printStudentDetails(&student, arr);
            acnt++;
        }
    }
    printString("\n");
    printAssignmentDetails(arr, acnt, "A");
    // closing the file descriptor 'fd'.
    close(fd);

}  
void printCommonAssignments(float arr[], float brr[]){
    for(int i=0; i<6; ++i) 
    {
        printString("Average marks in");
        printString(" Assignment: ");
        printInt(i+1);
        printString(" = ");
        float temp = (float)((arr[i]+ brr[i]) / (acnt + bcnt));
        printFloat(temp);
        printString("\n");
    }
}
int main(){
    // variable inside pthread
    pthread_t tid0;
    float arr[6] = {0};
    pthread_create(&tid0, NULL, ThreadFuncForSectionA,(void *)&arr);
    printf("in main\n");
    int fd  = open("student_record.csv", O_RDONLY);
    char buffer_str[100];
    // printString("\nChild process completed execution, Parent process started!\n\n");
    if(fd < 0) {
        fprintf(stderr, "Error opening file!\n");
        return 0;
    }
    float brr[6]={0};
    readFileByLine(fd, buffer_str, 100);
    while(readFileByLine(fd, buffer_str, 100) > 0) {
        struct Student student;
        parseStudentValues( &student, buffer_str, brr);
        if(student.section == 'B'){
            printStudentDetails(&student, brr);
            bcnt++;
        }
    }
    // closing the file descriptor 'fd'.
    printString("\n");
    printAssignmentDetails(brr, bcnt, "B");
    pthread_join(tid0, NULL);
    printString("\n");
    printCommonAssignments(arr, brr);
    close(fd);
    return 0; 
}    
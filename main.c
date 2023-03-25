/* TASK:
In Hungary people have to ask vaccination officially from the authorities. 

The Operating  Committee (parent process) has two vaccination buses (children).  The  Operating  Committee  has a meeting each day early in the morning and they decide the daily work then.  The capacity of a vaccination bus is only 5 person. (You need long time for a slow work ;) )  If there is enough person in the morning who asked vaccination (and they are not vaccinated yet), one of the  vaccination bus or both of them start to work.   (One vaccination bus starts if there are more than 4 person who want vaccination, two buses start if there are at least 10 person who wait for vaccination. 

When a vaccination bus starts  it sends a  "FIGHT UP" signal to the Operating Committee that they need the list of persons (5 names) to be vaccinated.  The Operation Committee getting the signal reads the next persons data (5 or 10), writes them on the screen (it means in real world to send  SMS to the patients)  and send them also to the bus(es) using pipe. The bus reads the data and write them on the screen. The patients arrive in time to the vaccination spot with the rate of 90% (use random numbers).  After vaccination the bus(es) sends back the vaccinated persons data to the Committee through pipe.  The Committee add a "VACCINATED" remark to everybody' data in the file who were vaccinated. 

Next day starts similarly - may be there are newly added names in the list.  The Operating  Committee starts the vaccination bus(es) if there are more then 4 person who asked vaccination and not vaccinated yet.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h> // for pipe()
#include <sys/time.h>  //for random()
#include <time.h>


struct Line{
    char name[20];
    char dateOfBirth[5];
    char phoneNumber[12];
    char isFree[4];
    char isVaccinated[4];
};


struct Line parseData(char str[]);
void addData(struct Line[],int index);
void showData(struct Line[],int index);
void printLine(struct Line line);
int deleteData(struct Line*);
void modifyData(struct Line*);
void handler(int signumber);
int homework2(struct Line * data,int i,FILE * f);

int main()
{
    struct Line data[50];
    int i = 0;

    FILE * f;
    f=fopen("data.txt","r");
    if (f==NULL){
        perror("File opening error\n"); exit(1);
    }
    char line[160];
    while (!feof(f)){//This function returns a non-zero value when End-of-File indicator associated with the stream is set, else zero is returned.
        fgets(line,sizeof(line),f);
        struct Line newLine = parseData(line);
        data[i] = newLine;
        i++;
    }
    fclose(f);
    printf("\n");

    char menu_input;
    int delindex;

    while(menu_input !='e'){
    printf("Main Menu:\n");
    printf("a. Add data.\n");
    printf("b. Modify data.\n");
    printf("c. Delete data.\n");
    printf("d. Show data.\n");
    printf("e. Save and quit.\n");
    printf("f. Simulate to next day and quit.\n");
    printf("Please enter a letter from main menu: \n");
    scanf(" %c",&menu_input); 

    switch(menu_input){

        case 'a':
            addData(data,i);
            i++;
            break;

        case 'b':
            printf("Please write the number of the line you want to modify.");
            scanf(" %d",&delindex);
            modifyData(&data[delindex-1]);
            break;

        case'c':
            printf("Please write the number of the line you want to delete.");
            scanf(" %d",&delindex);
            deleteData(&data[delindex-1]);
            break;

        case'd':
            showData(data,i);
            break;

        case'e':
            f=fopen("data.txt","w"); //clears the file
            int compareResult;
            for(int k=0; k<i; k++){
                compareResult = strcmp(data[k].name,"deleted");
                if(compareResult != 0 && k==i-1){
                    fprintf(f,"%s,%s,%s,%s,%s",data[k].name,data[k].dateOfBirth,data[k].phoneNumber,data[k].isFree,data[k].isVaccinated);// so the final \n do not occur.
                    return 0;
                }
                if(compareResult != 0){//data[k].name=="deleted")
                    fprintf(f,"%s,%s,%s,%s,%s\n",data[k].name,data[k].dateOfBirth,data[k].phoneNumber,data[k].isFree,data[k].isVaccinated);
                }
            }
            return 0;
            break;
        case'f':
            homework2(data,i,f);
            return 0;
        default:
            printf("invalid input");
            break;
        }
    }
}
void printLine(struct Line line){
    printf( "Name : %s\n", line.name);
    printf( "Date of Birth : %s\n", line.dateOfBirth);
    printf( "Phone number : %s\n", line.phoneNumber);
    printf( "Payment : %s\n", line.isFree);
    printf( "Vaccinated : %s\n", line.isVaccinated);
    printf("----------------------------------------------------- \n");
}

struct Line parseData(char str[]){
   struct Line newLine;

   char * token = strtok(str, ",");
   strcpy(newLine.name, token);

   token = strtok(NULL, ",");
   strcpy(newLine.dateOfBirth, token);

   token = strtok(NULL, ",");
   strcpy(newLine.phoneNumber, token);

   token = strtok(NULL, ",");
   strcpy(newLine.isFree, token);

   token = strtok(NULL, "\n");
   strcpy(newLine.isVaccinated, token);

   return newLine;
  }

void addData(struct Line data[],int index){
    printf("Name: ");
    char name[20];
    scanf("%s",name);

    printf("Date of Birth: ");
    char date[5];
    scanf("%s",date);

    printf("Phone number: ");
    char number[12];
    scanf("%s",number);

    printf("Payment: ");
    char free[4];
    scanf("%s",free);

    printf("Is vaccinated: ");
    char isVaccinated[4];
    scanf("%s",isVaccinated);

    struct Line newLine;
    strcpy(newLine.name, name);
    strcpy(newLine.dateOfBirth, date);
    strcpy(newLine.phoneNumber, number);
    strcpy(newLine.isFree, free);
    strcpy(newLine.isVaccinated, isVaccinated);

    data[index] = newLine;
}

void showData(struct Line data[], int index){
    int result;
    for(int i = 0; i<index; i++){
        result = strcmp(data[i].name, "deleted");
        if(result != 0){
            printLine(data[i]);
        }
        else{
            printf("\n------------------DeletedLine----------------- \n");
            printf("\n");
            }
    }
}

int deleteData(struct Line* linePointer){
    strcpy(linePointer->name,"deleted");
    strcpy(linePointer->dateOfBirth," ");
    strcpy(linePointer->phoneNumber," ");
    strcpy(linePointer->isFree," ");
    strcpy(linePointer->isVaccinated," ");
    return 0;
}

void modifyData(struct Line* linePointer){
    printf("Name: ");
    char name[20];
    scanf("%s",name);

    printf("Date of Birth: ");
    char date[5];
    scanf("%s",date);

    printf("Phone number: ");
    char number[12];
    scanf("%s",number);

    printf("Payment: ");
    char free[4];
    scanf("%s",free);

    printf("Is vaccinated: ");
    char isVaccinated[4];
    scanf("%s",isVaccinated);

    strcpy(linePointer->name,name);
    strcpy(linePointer->dateOfBirth, date);
    strcpy(linePointer->phoneNumber, number);
    strcpy(linePointer->isFree, free);
    strcpy(linePointer->isVaccinated, isVaccinated);

}
int homework2(struct Line * data,int i,FILE * f){
    int pipefd[2];
    int pipefd2[2];// unnamed pipe file descriptor array
    pid_t pid;

    if (pipe(pipefd) == -1 || pipe(pipefd2))
    {
       perror("Opening error!");
       exit(EXIT_FAILURE);
    }

    int unvaccinatedList1[5]={-1,-1,-1,-1,-1};
    int unvaccinatedList2[5]={-1,-1,-1,-1,-1};

    int unvaccinatedCount = 0;
    for(int k = 0;k<i;k++){
        int result = strcmp(data[k].isVaccinated, "No");
        if(result==0){
            unvaccinatedCount++;
        }
    }
    if(unvaccinatedCount<4){
        printf("Unvaccinated count:%d.",unvaccinatedCount);
        printf("Not enough unvaccinated patient for 1 car.");
        return 0;
    }
    else if(unvaccinatedCount<10){  //1 car scenario
        signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), //handler = SIG_DFL - back to default behavior
        pid_t child=fork();

        if (child>0){  //Operating Committee process
            pause(); //waits till a signal arrive
            printf("Fight Up Signal arrived\n",SIGTERM);
            //int status;
            //wait(&status);

            int unvaccinatedIndex = 0;
            printf("\nThe patient list for the vaccination by Operating Committee : \n\n");
            for (int k = 0; k < i; k++ ){                    // create the list for the bus
                if(strcmp(data[k].isVaccinated, "No") == 0){
                    printLine(data[k]);
                    unvaccinatedList1[unvaccinatedIndex] = k;
                    unvaccinatedIndex++;
                }
                if(unvaccinatedIndex>=5){
                    break;
                }
            }

            //close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], unvaccinatedList1,sizeof(int)*5);
            close(pipefd[1]); // Closing write descriptor
            printf("Operating Committee wrote the message to the pipe!\n");


            fflush(NULL); 	// flushes all write buffers (not necessary)
            sleep(3);		// waiting for child process (not necessary)
            printf("Operating committee starts to read from the pipe!");
            read(pipefd[0],unvaccinatedList1,sizeof(int)*5);
              //Marking as Vaccinated
            for(int j=0;j<5;j++){
                if(unvaccinatedList1[j]!=-1){
                    strcpy(data[unvaccinatedList1[j]].isVaccinated,"Yes");
                    //printf("%d\n",unvaccinatedList1[j]);
                }
            }
            close(pipefd[0]);
            f=fopen("data.txt","w"); //clears the file
                int compareResult;
                for(int k=0; k<i; k++){
                    compareResult = strcmp(data[k].name,"deleted");
                    if(compareResult != 0 && k==i-1){
                        fprintf(f,"%s,%s,%s,%s,%s",data[k].name,data[k].dateOfBirth,data[k].phoneNumber,data[k].isFree,data[k].isVaccinated);// so the final \n do not occur.
                        printf("Operating Committee process finished!\n");
                        return 0;
                    }
                    if(compareResult != 0){//data[k].name !="deleted")
                        fprintf(f,"%s,%s,%s,%s,%s\n",data[k].name,data[k].dateOfBirth,data[k].phoneNumber,data[k].isFree,data[k].isVaccinated);
                    }
                }
            return 0;

        }




        else { //Bus process
            printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
            sleep(3);
            kill(getppid(),SIGTERM);
            sleep(3);	// sleeping a few seconds, not necessary
            printf("Child starts to read from the pipe!\n");
            read(pipefd[0],unvaccinatedList1,sizeof(int)*5); 

            printf("\nBus screen1:\n");
            srand(time(NULL));
            for(int j =0;j<5;j++){
                if(unvaccinatedList1[j]==-1){
                    continue;
                }
                printLine(data[unvaccinatedList1[j]]);
                int ranInt = rand()%10;
                if(ranInt<9){//probaility of vaccination is %90
                    strcpy(data[unvaccinatedList1[j]].isVaccinated,"Yes");
                    //printLine(data[unvaccinatedList1[j]]);
                }
                else{
                    unvaccinatedList1[j]=-1;
                }
                //printf("%d",unvaccinatedList1[j]); ----------------
            }
            printf("\n");
            write(pipefd[1], unvaccinatedList1,sizeof(int)*5);
            close(pipefd[1]); // Closing write descriptor
            close(pipefd[0]); // finally we close the used read end
            wait();
            printf("Bus process ended\n");
        }
    }




    else{    //2 car scenario
        signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), //handler = SIG_DFL - back to default behavior
        pid_t child=fork();
        if (child>0){
            pid_t child2=fork();
            if(child2 > 0 ){
                pause(); //waits till a signal arrive
                printf("Signal 1 arrived\n",SIGTERM);
                pause();
                printf("Signal 2 arrived\n",SIGTERM);
                printf("\nThe patient list for the vaccination by Operating Committee : \n\n");
                int unvaccinatedIndex = 0;
                for (int k = 0; k < i; k++ ){                    // create the list for the bus
                    if(strcmp(data[k].isVaccinated, "No") == 0){
                        if(unvaccinatedIndex<5){
                            printLine(data[k]);
                            unvaccinatedList1[unvaccinatedIndex] = k;
                            unvaccinatedIndex++;
                        }
                        else{
                            printLine(data[k]);
                            unvaccinatedList2[unvaccinatedIndex-5] = k;
                            unvaccinatedIndex++;
                        }
                    }
                    if(unvaccinatedIndex>=10){
                        break;
                    }
                }
                write(pipefd[1], unvaccinatedList1,sizeof(int)*5);

                //sleep(3); //waiting bus 1 to read;
                write(pipefd2[1], unvaccinatedList2,sizeof(int)*5);
                fflush(NULL);
                sleep(3);		// waiting for child process (not necessary)
                read(pipefd[0],unvaccinatedList1,sizeof(int)*5);
                 //Marking as Vaccinated
                for(int j=0;j<5;j++){
                    if(unvaccinatedList1[j]!=-1){
                        strcpy(data[unvaccinatedList1[j]].isVaccinated,"Yes");
                    }
                }
                close(pipefd[0]);
                read(pipefd2[0],unvaccinatedList2,sizeof(int)*5);
                 //Marking as Vaccinated
                for(int j=0;j<5;j++){
                    if(unvaccinatedList2[j]!=-1){
                        strcpy(data[unvaccinatedList2[j]].isVaccinated,"Yes");
                    }
                }
                close(pipefd2[0]);
                for(int k = 0; k<i; k++){
                    printLine(data[k]);
                }
                 f=fopen("data.txt","w"); //clears the file
                int compareResult;
                for(int k=0; k<i; k++){
                    compareResult = strcmp(data[k].name,"deleted");
                    if(compareResult != 0 && k==i-1){
                        fprintf(f,"%s,%s,%s,%s,%s",data[k].name,data[k].dateOfBirth,data[k].phoneNumber,data[k].isFree,data[k].isVaccinated);// so the final \n do not occur.
                        printf("Parent process ended\n");
                        return 0;
                    }
                    if(compareResult != 0){//data[k].name !="deleted")
                        fprintf(f,"%s,%s,%s,%s,%s\n",data[k].name,data[k].dateOfBirth,data[k].phoneNumber,data[k].isFree,data[k].isVaccinated);
                    }
                }
                //return 0;

            }
            else{//Bus 2
                sleep(5);
                printf("Bus 2 waits 5 seconds, then send a SIGTERM %i signal\n",SIGTERM);
                kill(getppid(),SIGTERM);
                printf("Bus2 starts to read from the pipe!\n");
                read(pipefd2[0],unvaccinatedList2,sizeof(int)*5); //just 5 didnt work for some reason.
                sleep(1);
                printf("\nBus screen2:\n");
                srand(time(NULL));
                for(int j =0;j<5;j++){
                    if(unvaccinatedList2[j]==-1){
                        continue;
                    }
                    printLine(data[unvaccinatedList2[j]]);
                    int ranInt = rand()%10;

                    if(ranInt<9){//probaility of vaccination is %90
                        strcpy(data[unvaccinatedList2[j]].isVaccinated,"Yes");
                        //printLine(data[unvaccinatedList1[j]]);

                    }
                    else{
                        unvaccinatedList2[j]=-1;
                    }
                    //printf("%d",unvaccinatedList1[j]); ----------------
                }
                printf("\n Bus 2 is writing to the pipe.\n");
                write(pipefd2[1], unvaccinatedList2,sizeof(int)*5);
                close(pipefd2[1]); // Closing write descriptor
                close(pipefd2[0]); // finally we close the used read end
                printf("Bus 2 process ended\n");

            }
        }
        else {  //Bus 1
        printf("Bus 1 waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
        sleep(2);
        kill(getppid(),SIGTERM);
        printf("Child starts to read from the pipe!\n");
        read(pipefd[0],unvaccinatedList1,sizeof(int)*5); 


        printf("\nBus screen1:\n");
        srand(time(NULL));

        for(int j =0;j<5;j++){
            if(unvaccinatedList1[j]==-1){
                continue;
            }
            printLine(data[unvaccinatedList1[j]]);
            int ranInt = rand()%10;

            if(ranInt<9){//probaility of vaccination is %90
                strcpy(data[unvaccinatedList1[j]].isVaccinated,"Yes");
                //printLine(data[unvaccinatedList1[j]]);
            }
            else{
                unvaccinatedList1[j]=-1;
            }

        }
        printf("\n Bus 1 is writing to the pipe.\n");
        write(pipefd[1], unvaccinatedList1,sizeof(int)*5);
        close(pipefd[1]); // Closing write descriptor
        close(pipefd[0]); // finally we close the used read end


        printf("Bus 1 process ended\n");
        }

    }
    fclose(f);
}


void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

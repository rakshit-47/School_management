#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include <time.h>
#include <unistd.h>
#include "library.h"
#include<libpq-fe.h>
#include <cjson/cJSON.h>

struct student *head_student=NULL, *temp_student;
struct teacher *head_teacher=NULL , *temp_teacher;
int change_student = 0 , change_teacher =0;
time_t start_time;


int new_table_db(PGconn *conn)
{
   /*Creates tables in database if not exist*/
   char query[256];

   PGresult *res;
   if (PQstatus(conn) != CONNECTION_OK) 
   {
      fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
   
      return 1;
   }
   
   snprintf(query, sizeof(query), "select * from student");
   res = PQexec(conn, query);
   if (PQresultStatus(res) != PGRES_TUPLES_OK) 
   {
      snprintf(query, sizeof(query), "CREATE table student(class int , data json)");
      PGresult *create_res = PQexec(conn, query);
      if (PQresultStatus(create_res) != PGRES_COMMAND_OK) 
      {
          printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      }
      printf("Student table is created\n");
      PQclear(create_res);
   }
   PQclear(res);
   snprintf(query, sizeof(query), "select * from teacher");
   res = PQexec(conn, query);
   if (PQresultStatus(res) != PGRES_TUPLES_OK) 
   {
      snprintf(query, sizeof(query), "CREATE TABLE teacher (teacher_name TEXT, teacher_number TEXT, teacher_birthdate TEXT, teacher_address TEXT, teacher_age INT, subject TEXT, joining_date TEXT, teacher_class INT)");
      PGresult *create_res = PQexec(conn, query);
      if (PQresultStatus(create_res) != PGRES_COMMAND_OK) 
      {
          printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      }
      printf("Teacher table is created\n");
      PQclear(create_res);
   }
   PQclear(res);
   snprintf(query, sizeof(query), "select * from password");
   res = PQexec(conn, query);
   if (PQresultStatus(res) != PGRES_TUPLES_OK) 
   {
      snprintf(query, sizeof(query), "CREATE TABLE password (pass TEXT UNIQUE, name TEXT)");
      PGresult *create_res = PQexec(conn, query);
      if (PQresultStatus(create_res) != PGRES_COMMAND_OK) 
      {
          printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      }
      printf("Password table is created\n");
      PQclear(create_res);
   }
   PQclear(res);

   return 0;
}

int time_check()
{
   /*Used as timer in exam it will close the exam after certein time*/
   usleep(100000); 
   time_t current_time = time(NULL);
   time_t elapsed = current_time - start_time;

   int minutes = (elapsed % 3600) / 60;
   int seconds = elapsed % 60;

   printf("Current Time %02d:%02d\n", minutes, seconds);
   if (elapsed >= 60)
   {
      return 0;
   }
   return 1;  
}

int valid_address(const char *address)
{
      /*It checks if the typed address by user is valid address or not*/
   if(address[0] == 0)
   {
      printf("Enter something\n");
      return 0;
   }
   if(isspace(address[0]))
   {
      printf("Dont enter space in first\n");
      return 0;
   }
   for (int i = 0; address[i] != '\0'; i++) 
   {
      if (!isalpha(address[i]) && !isspace(address[i])) 
      {
         printf("Don't enter number\n");
         return 0;
      }
   }
   return 1;
}

int valid_pin_code(const char *pin)
{
      /*It checks if the typed pin code is valid or not 
   the pincode should be exact 6 numbers and only numbers*/
   int length = strlen(pin);
   if(length != 6) 
   {
      return 0;
   }
   for(int i = 0; pin[i] != '\0'; i++) 
   {
      if (!isdigit(pin[i])) 
      {
         return 0;
      }
   }
   return 1;
}
int valid_name(const char *name)
{
      /*This checks if the typed name is valid or not 
   the name should not contain numbers or special charaters*/
   int len;
   len = strlen(name);
   if(len == 0)
   {
      printf("No name entered\n");
      return 0;
   }
   if(isspace(name[0]))
   {
      printf("Dont enter space in first\n");
      return 0;
   }
   for (int i = 0; name[i] != '\0'; i++) 
   {
      if (!isalpha(name[i]) && !isspace(name[i])) 
      {
         printf("Invalid name\n");
         return 0;
      }
   }
   return 1;
}
int is_valid_num(char *num)
{
      /*This verifies if the number typed has exact 10 numbers and weather it is only number*/
   int length = strlen(num);
   if(length != 10) 
   {
      return 0;
   }
   for(int i = 0; num[i] != '\0'; i++) 
   {
      if (!isdigit(num[i])) 
      {
         return 0;
      }
   }
   return 1;
}
int valid_religion(char str[])
{
   /*the user cant enter any other religion expect this*/
   for(int i = 0; str[i]; i++) 
   {
      str[i] = tolower((unsigned char)str[i]);
   }
   if(strcmp(str, "hindu") == 0 || strcmp(str, "muslim") == 0 ||
      strcmp(str, "christian") == 0 || strcmp(str, "sikh") == 0 ||
      strcmp(str, "jain") == 0 || strcmp(str, "buddhist") == 0 ||
      strcmp(str, "others") == 0) 
   {
      return 1;
   }
   return 0;
}
int valid_birth(char birth[])
{
   
   /*This checks weather the user typed the birthday in right format which is DD/MM/YYYY
   and also it does some error handling like user can't enter 30day on feb month */
   time_t now;
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);

    int year = timeinfo->tm_year + 1900; 

   int len;
   len = strlen(birth);
   if(len != 10)
   {
      return 0;
   }
   if(birth[2] != '/' || birth[5] != '/')
   {
      return 0;
   }
   int dayi;
   int monthi;
   int yeari;
   char *endptr;
   char days[3];
   char months[3];
   char years[5];
    int i = 0;
   days[0] = birth[0];
   days[1] = birth[1];
   days[2] = '\0';

   dayi = strtol(days ,&endptr, 10 );
   if(dayi > 0 && dayi <= 31)
   {
      i++;
   }

   months[0] = birth[3];
   months[1] = birth[4];
   months[2] = '\0';

   monthi = strtol(months , &endptr , 10);
   if(monthi > 0 && monthi<= 12)
   {
      i++;
   }

   years[0] = birth[6];
   years[1] = birth[7];
   years[2] = birth[8];
   years[3] = birth[9];
   years[4] = '\0';

   yeari = strtol(years , &endptr , 10);
   if(yeari %4 == 0 && monthi == 2)
   {
      if( dayi > 0 && dayi <= 29)
      {
         return 1;
      }
      else
      {
         printf("Its leap year feb as 29 days\n");
         return 0;
      }
   }
   else if( monthi == 2)
   {
      if(dayi > 0 && dayi <= 28)
      {
         return 1;
      }
      else
      {
         printf("The feb month as only 28 days\n");
         return 0;
      }
   }
   if(yeari >= (year-60) && yeari <= year)
   {
      i++;
   } 
   if( i == 3)
   {
     return 1;
   }
   else
   {
     printf("Enter the date properly or ");
     return 0;
   }

}


int student_admission(PGconn *conn)
{
      /*This function displays the information of the school and takes some information from user like class and display fees as per as class*/

   char c, ch[4];
   char d;
   int class, fees = 20000;
   char term;
   int count=0;
   printf("\t\t\t\t\t\tWelcome to Bright will school\n");
   printf("\n-----------------------------------------------------------------------------------------------------------------\n");
 
   printf("\n\nWe provide students with rich facilities like:-\n\n");
   printf("Computer lab\tLaboratories\tLibrary\t\tSports facilities\nActivity Rooms\tArt Room\tCafeteria\tMedical room\nPlayground\n\n");
 
   printf("Achievements:-\n> We are glad to share that the school won the 'Best School Delegation Trophy' at the Salwan Model United Nations Conference (SAMUN)\n");
   printf("> Bright will School has been awarded for being one amongst the “Top Favourite 100 Schools of the Year 2019”\n\n");
 
   printf("To see the fee structure\n");
   do 
   {
     while (1) 
      {
         printf("Enter a class number: ");
         if (scanf("%d%c", &class, &term) == 2 && term == '\n') 
         {
            if (class >= 1 && class <=  12) 
            {
               break;
            } else 
            {
               printf("Number out of range. Enter a number between 1 and 12.\n");
            }
         } else 
         {
            term = 0;
            printf("Invalid input. Enter a valid number between 1 and 12.\n");
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
         }
      }
   } 
   while (!class >= 1 && class <=  12);
   printf("\n");
   read_student_data(conn);
   temp_student = head_student;
   while (temp_student != NULL)
   {
      if(temp_student->grade.class == class)
      {
         count++;
      }
      temp_student = temp_student->next;
   }
   struct student *temp;
   while (head_student != 0)
   {
      temp = head_student;
      head_student = head_student->next;
      free(temp);
   }


   if(count >= 10)
   {
      printf("The admission is full for %d class\n\n", class);
      return 1;
   }


   fees = fees + (1000 * class);
   printf("Do you want bus facilities(Yes or No)\nPer year 5000rs\n");
   scanf("%s", ch);
   while (1) 
   {
      for (int i = 0; ch[i] != '\0'; i++) 
      {
          ch[i] = tolower((unsigned char)ch[i]);
      }
      if (strcmp(ch, "yes") == 0 || strcmp(ch, "no") == 0) 
      {
          break;  
      } else 
      {
          printf("Invalid input. Please enter 'yes' or 'no': ");
          scanf("%s", ch);
      }
   }
   if(strcmp(ch , "yes")== 0)
   {
      printf("\nSchool fees: %d\nBus facilities: 5000\n", fees);
      fees = fees+ 5000;
      printf("---------------------------------\n");
      printf("Total:  %d", fees);
   }  
   else
   {
      printf("\nSchool fees: %d\n", fees);
   }
   printf("\n\nAre you interested(Yes or No)\n");
   scanf("%s", ch);
   while (1) 
   {
      for (int i = 0; ch[i] != '\0'; i++) 
      {
          ch[i] = tolower((unsigned char)ch[i]);
      }
      if (strcmp(ch, "yes") == 0 || strcmp(ch, "no") == 0) 
      {
          break;  
      } else {
          printf("Invalid input. Please enter 'yes' or 'no': ");
          scanf("%s", ch);
      }
   }
   if(strcmp(ch , "no")== 0)
   {
      printf("Thank you for visiting\n");
      return 0;
   }
   exam_student(class, fees,conn);

} 
int exam_student(int class, int fees,PGconn *conn)
{
      /*This functions reads questions from file and ask the user to type the answer (NOTE: If the student is below class 5Th he/she don't have to attent exam they can directly enter the data)
   the student in 5TH or above 5th have to attent exam if they get below 70% they cant take admission*/
   char ch;
   int num;
   char ans;
   int marks=0;
   int paid_fees;
   char term;
   int per=0;
   if(class < 5)
   {
      
      printf("\nOut of %drs how much would you like to pay?\n",fees );
      while (1) 
      {
         if (scanf("%d%c", &paid_fees, &term) == 2 && term == '\n') 
         {
            break;
         }
         else 
         {
            term = 0;
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
            printf("Invalid input. Please enter a valid integer.\n");
         }
      }
      while (paid_fees > fees || paid_fees < 20000)
      {
         printf("Invaild, Enter again(minimun amound is 20000rs)\n");
         while (1) 
         {
            if (scanf("%d%c", &paid_fees, &term) == 2 && term == '\n') 
            {
               break;
            }
            else 
            {
               term = 0;
               while (term != '\n' && term != EOF) 
               {
                  term = getchar();
               }
               printf("Invalid input. Please enter a valid integer.\n");
            }
         }
      }
      fees = fees - paid_fees;
      write_student_data(class, fees, conn);
      printf("\nWelcome to school\n");
      return 0;
   }
   printf("Exams in\n");
   for(int k= 5 ; k>=0 ;k--)
   {
      sleep(1);
      printf("%d\n",k);
   }
   printf("------------------------------------------------------------------------------------------------------------\n");

   FILE *fp = 0;
   fp = fopen("exam.txt", "r");
   if(fp == 0)
   {
      printf("The file did'nt open \n");
      return 0;
   }
      while(!feof(fp))
      {
         fscanf(fp , "%d", &num);
         ch = fgetc(fp);
         if(num == class)
         {
            start_time = time(NULL);
            while(!feof(fp))
            { 
               if(ch == '$')
               {
                  break;
               }
               ch = fgetc(fp);
               ans = 0;
               if(ch == '^')
               {
                  scanf("%c", &ans);
                  if(ans >= 65 && ans<=90)
                  {
                     ans = ans +32;
                  }
                  ch = fgetc(fp);
                  if(ans == ch)
                  { 
                     marks++;
                  }
                  
                  if(!time_check())
                  {
                     printf("Time's up\n");
                     goto up;
                  }
                  continue;
               }
               printf("%c", ch);
            }
         }
      }

   up:
   per = (marks * 100)/5;
   printf("\nYou percentage is %d\n", per);
   if(per >= 70)
   {
      printf("\nCongratulation you are seleted\n");
      printf("Out of %drs how much would you like to pay?\n",fees );
      while (1) 
      {
         if (scanf("%d%c", &paid_fees, &term) == 2 && term == '\n') 
         {
            break;
         }
         else 
         {
            term = 0;
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
            printf("Invalid input. Please enter a valid integer.\n");
         }
      }
      while (paid_fees > fees || paid_fees < 20000)
      {
         printf("Invaild, Enter again(minimun amound is 20000rs)\n");
         while (1) 
         {
            if (scanf("%d%c", &paid_fees, &term) == 2 && term == '\n') 
            {
               break;
            }
            else 
            {
               term = 0;
               while (term != '\n' && term != EOF) 
               {
                  term = getchar();
               }
               printf("Invalid input. Please enter a valid integer.\n");
            }
         }
      }
      fees = fees - paid_fees;
      /*if they get above 70% they can enter the persnol data*/
      write_student_data(class, fees,conn);
      
      fclose(fp);

   }
   else
   {
      printf("You can't take admission\n");
      fclose(fp);
      return 0;
   }

   printf("\n");
}
int write_student_data(int class, int fee,PGconn *conn)
{
   /*Takes input from user with error handling and store then in the database*/
   struct student *newnode;
   char years[5];
   int yeari;
   char *endptr;
   newnode = (struct student*)calloc(1, sizeof(struct student));
   if (newnode == NULL) 
   {
      perror("Failed to allocate memory");
      return 0;
   }
   printf("\n\nEnter your details\n");
   do 
   {
      printf("Name: ");
      fgets(newnode->student_name, sizeof(newnode->student_name), stdin);
      newnode->student_name[strcspn(newnode->student_name, "\n")] = '\0';
   } 
   while (!valid_name(newnode->student_name));
   if(newnode->student_name[0] >= 97 && newnode->student_name[0] <= 122)
   {
      newnode->student_name[0] = newnode->student_name[0] - 32;
   }


   printf("Blood group: ");
   scanf("%s", newnode->blood_group);
   for(int i = 0 ; newnode->blood_group[i] != '\0' ; i++)
   {
      if(newnode->blood_group[i] >= 97 && newnode->blood_group[i] <= 122)
      {
         newnode->blood_group[i] = newnode->blood_group[i] - 32;
      }
   }
   while(strcmp(newnode->blood_group, "O+") != 0 && 
      strcmp(newnode->blood_group, "O-") != 0 &&
      strcmp(newnode->blood_group, "A+") != 0 && 
      strcmp(newnode->blood_group, "A-") != 0 &&
      strcmp(newnode->blood_group, "B+") != 0 &&
      strcmp(newnode->blood_group, "B-") != 0 &&
      strcmp(newnode->blood_group, "AB+") != 0 &&
      strcmp(newnode->blood_group, "AB-") != 0) 
   {
      printf("Enter a valid blood group\n");
      scanf("%s", newnode->blood_group);
      for(int i = 0 ; newnode->blood_group[i] != '\0' ; i++)
      {
         if(newnode->blood_group[i] >= 97 && newnode->blood_group[i] <= 122)
         {
            newnode->blood_group[i] = newnode->blood_group[i] - 32;
         }
      }
   }

   time_t now;
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);

    int present_year = timeinfo->tm_year + 1900; 
   do
   {
      printf("Enter your Date of birth(DD/MM/YYYY): ");
      scanf("%s", newnode->birth);
      if(!valid_birth(newnode->birth))
      {
         printf("Enter in (DD/MM/YYYY) format \n");
         scanf("%s",newnode->birth);
      }
   }
   while(!valid_birth(newnode->birth));
   int age;
   int year;
   age = class + 6;
   year = present_year - age;
   years[0] = newnode->birth[6];
   years[1] = newnode->birth[7];
   years[2] = newnode->birth[8];
   years[3] = newnode->birth[9];
   years[4] = '\0';
   yeari = strtol(years , &endptr , 10);
   if(!(year -2 <= yeari && year+2  >= yeari))
   {
      if(yeari < year)
      {
         printf("You are too old to study in %d class", class);
      }
      if(yeari > year)
      {
         printf("You are too young to study in %d class", class);
      }
      free(newnode);
      return 0;
   }
   do    
   {
      printf("Religion: ");
      scanf("%s", newnode->religion);
      if (!valid_religion(newnode->religion))
      {
         printf("Invalid religion. Please enter a valid religion (Hindu, Muslim, Christian, Jain, Buddhist, Sikh or Others)).\n");
      }
   }
   while (!valid_religion(newnode->religion));
   getchar();
   do 
   {
      printf("Father Name: ");
      fgets(newnode->par.father_name , sizeof(newnode->par.father_name), stdin);
      newnode->par.father_name[strcspn(newnode->par.father_name, "\n")] = '\0';
   } 
   while(!valid_name(newnode->par.father_name));
   do 
   {
      printf("Father's phone number: ");
      scanf("%s", newnode->par.f_num);
      if (!is_valid_num(newnode->par.f_num)) 
      {
         printf("Invalid number. Please enter a valid 10-digit number.\n");
      }
   } 
   while (!is_valid_num(newnode->par.f_num));
   getchar();
   do 
   {
      printf("Mother name: ");
      fgets(newnode->par.mother_name, sizeof(newnode->par.mother_name), stdin);
      newnode->par.mother_name[strcspn(newnode->par.mother_name, "\n")] = '\0';
   } 
   while (!valid_name(newnode->par.mother_name));
   do 
   {
      printf("Mother's phone number: ");
      scanf("%s", newnode->par.m_num);
      if (!is_valid_num(newnode->par.m_num)) 
      {
         printf("Invalid number. Please enter a valid 10-digit number.\n");
      }
   } 
   while (!is_valid_num(newnode->par.m_num));
   getchar(); 

   char colony[20];
   char road_name[20];
   char city[15];
   char state[20];
   char pin_code[7];
   
   do 
   {
      printf("Enter the address\nRoad name: ");
      fgets(road_name, sizeof(road_name), stdin);
      road_name[strcspn(road_name, "\n")] = '\0';
   } while (!valid_address(road_name));

   do 
   {
      printf("Colony: ");
      fgets(colony, sizeof(colony), stdin);
      colony[strcspn(colony, "\n")] = '\0';
   } while (!valid_address(colony));

   do 
   {
      printf("City: ");
      fgets(city, sizeof(city), stdin);
      city[strcspn(city, "\n")] = '\0';
   } while (!valid_address(city));
   
   do 
   {
      printf("State: ");
      fgets(state, sizeof(state), stdin);
      state[strcspn(state, "\n")] = '\0';
   } while (!valid_address(state));

   do 
   {
      printf("Pin code: ");
      scanf("%s", pin_code);
      while (getchar() != '\n');
      if (!valid_pin_code(pin_code)) 
      {
         printf("Enter a valid pin code with 6 digits\n");
      }
   } while (!valid_pin_code(pin_code));

   snprintf(newnode->address, sizeof(newnode->address),"%s %s %s %s %s", road_name, colony, city, state, pin_code);

   newnode->grade.class = class;
   newnode->fees = fee;
   change_student = 1;
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   strftime(newnode->joining_date_s, sizeof(newnode->joining_date_s), "%d/%m/%Y", tm);



   if (PQstatus(conn) != CONNECTION_OK) 
    {
        printf("Connection to database failed: %s\n", PQerrorMessage(conn));
     
        return 1;
    }
   char query[300];

   // snprintf(query, sizeof(query) , "insert into student values('%s' , '%s', '%s' ,'%s' ,'%s' ,'%s' ,'%s' ,'%s', %d , %d , '%s', '%s')", newnode->student_name , newnode->blood_group, newnode->birth , newnode->par.father_name, newnode->par.f_num , newnode->par.mother_name , newnode->par.m_num , newnode->address , newnode->grade.class ,newnode->fees , newnode->joining_date_s, newnode->religion);
   // PGresult *res = PQexec(conn, query);
   // ExecStatusType resStatus = PQresultStatus(res);
   

   // if (PQresultStatus(res) != PGRES_COMMAND_OK) 
   // {
   //    printf("Error while executing the query: %s\n", PQerrorMessage(conn));
   //    PQclear(res);
   // 
   //    return 1;
   // }
   // PQclear(res);
   // 
   // free(newnode);
   // return 0;
   char data[400];
   snprintf(query, sizeof(query), "SELECT class FROM student WHERE class = %d", class);
   PGresult *res = PQexec(conn, query);

   if (PQresultStatus(res) != PGRES_TUPLES_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res);
   
      return 1;
   }
   int rows = PQntuples(res);
   PQclear(res);
   
   snprintf(data, sizeof(data) , "[{\"name\":\"%s\", \"blood_group\":\"%s\", \"birth_of_date\":\"%s\", \"father_name\":\"%s\", \"father_num\":\"%s\", \"mother_name\":\"%s\", \"mother_num\":\"%s\", \"address\":\"%s\" , \"unpaid_fees\":%d, \"joining_data\":\"%s\", \"religion\":\"%s\"}]", newnode->student_name , newnode->blood_group, newnode->birth , newnode->par.father_name, newnode->par.f_num , newnode->par.mother_name , newnode->par.m_num , newnode->address ,newnode->fees , newnode->joining_date_s, newnode->religion);

   char query_db[400];
   if (rows > 0) 
   {
      snprintf(query_db, sizeof(query_db), "UPDATE student SET data = COALESCE(data::jsonb, '[]'::jsonb) || '%s'::jsonb WHERE class = %d", data, class);

   } 
   else 
   {
       snprintf(query_db, sizeof(query_db), "INSERT INTO student (class, data) VALUES (%d, '%s')", class, data);
   }
   PGresult *res_db = PQexec(conn, query_db);
   if (PQresultStatus(res_db) != PGRES_COMMAND_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res_db);
   
      return 1;
   }
   printf("Data processed successfully.\n");
   PQclear(res_db);

   free(newnode);
   return 0;

}

int teacher_hiring(PGconn *conn)
{

   /*This functions gives exams for one who came for teacher job*/
   char choice[10];
   strcpy(choice , choose_subject(conn));
   if(strcmp(choice , "no")==0)
   {
      printf("Thank you\n");
      return 0;
   }
   char ch , ans;
   int marks = 0;
   int num;
   int age, exp;
   char term;
   printf("Exams in\n");
   for(int k= 5 ; k>=0 ;k--)
   {
      sleep(1);
      printf("%d\n",k);
   }
   printf("press Enter\n");
   printf("------------------------------------------------------------------------------------------------------------\n");
   FILE *fp = 0;
   fp = fopen("exam.txt", "r");
   if(fp == 0)
   {
      printf("The file did'nt open \n");
      return 0;
   }

      while(!feof(fp))
      {  
         fscanf(fp , "%d", &num);
         ch = fgetc(fp);
         if(num == 13)
         {

            while(!feof(fp))
            { 
               if(ch == '$')
               {
                  break;
               }
               ch = fgetc(fp);
               ans = 0;
               if(ch == '^')
               {
                  scanf("%c", &ans);
                  if(ans >= 65 && ans<=90)
                  {
                     ans = ans +32;
                  }
                  ch = fgetc(fp);
                  if(ans == ch)
                  { 
                     marks++;
                  }
                  continue;
               }
               printf("%c", ch);
            }
         }
      }
   int per=0;
   per = (marks * 100)/5;
   printf("\nYou percentage is %d\n", per); 
   if(per < 70)
   {
      printf("Sorry can't work here\n");
      fclose(fp);
      return 0;
   }
   printf("Enter your work experience (in years): ");
   while (1) 
   {
      if (scanf("%d%c", &exp, &term) == 2 && term == '\n') 
      {
         if (exp > 4 && exp < 31) 
         {
            break;
         } 
         else 
         {
            if(exp > 5)
            {
               printf("The experience should be more than 5 years\n");
            }
            if(exp > 30)
            {
               printf("We need a young teacher\n");
            }
            fclose(fp);
            return 0;
         }
      } 
      else 
      {
         term = 0;
         printf("Enter numbers only\n");
         while ((term = getchar()) != '\n' && term != EOF); 
      }
   }
   printf("Enter your age: ");
   while (1) 
   {
      if (scanf("%d%c", &age, &term) == 2 && term == '\n') 
      {
         if (age >= (exp+23)) 
         {
            break;
         } 
         else 
         {
            printf("The age dosen't seems right enter again.\n");
            fclose(fp);
            return 0;
         }
      } 
      else 
      {
         term = 0;
         printf("Enter numbers only\n");
         while ((term = getchar()) != '\n' && term != EOF); 
      }
   }
   printf("Valid input. Proceeding...\n");

   printf("Congratulation you are seleted\n");
   fclose(fp);
   /*If the teacher secures 70%+ they can teacher in the school and they should enter their persnol data*/
   write_teacher_data(age, choice,conn);
}  
int write_teacher_data(int age, char subject[],PGconn *conn)
{
   /*This takes input from the teacher (with error handling) and stores in the database */
   int a= 1;
   int real_year;
   char *endptr;
   int yeari;
   char years[5];
   struct teacher *newnode = (struct teacher*)calloc(1, sizeof(struct teacher));
   if(!newnode) 
   {
      printf("Memory allocation failed\n");
      return 0;
   }

   printf("Enter your details\n");
   do
   {
      printf("Name: ");
      fgets(newnode->teacher_name , sizeof(newnode->teacher_name) , stdin);
      newnode->teacher_name[strcspn(newnode->teacher_name,"\n")] = '\0';
   }       
   while (!valid_name(newnode->teacher_name));
   if(newnode->teacher_name[0] >= 97 && newnode->teacher_name[0] <= 122)
   {
      newnode->teacher_name[0] = newnode->teacher_name[0] - 32;
   }
   do 
   {
      printf("Phone number: ");
      scanf("%s", newnode->teacher_num);
      if (!is_valid_num(newnode->teacher_num)) 
      {
         printf("Invalid number. Please enter a valid 10-digit number.\n");
      }
   } 
   while(!is_valid_num(newnode->teacher_num));
   do
   {
      
      printf("Enter your Date of birth(DD/MM/YYYY): ");
      back:
      scanf("%s", newnode->teacher_birthdate);
      years[0] = newnode->teacher_birthdate[6];
      years[1] = newnode->teacher_birthdate[7];
      years[2] = newnode->teacher_birthdate[8];
      years[3] = newnode->teacher_birthdate[9];
      years[4] = '\0';

      yeari = strtol(years , &endptr , 10);
      real_year = 2024 - age;
      if(yeari != real_year)
      {
         a = 0;
      }
      if(!valid_birth(newnode->teacher_birthdate) )
      {
        printf("Enter in (DD/MM/YYYY) format\n");
        scanf("%s",newnode->teacher_birthdate);
      }
   }
   while(!valid_birth(newnode->teacher_birthdate));

      years[0] = newnode->teacher_birthdate[6];
      years[1] = newnode->teacher_birthdate[7];
      years[2] = newnode->teacher_birthdate[8];
      years[3] = newnode->teacher_birthdate[9];
      years[4] = '\0';

      yeari = strtol(years , &endptr , 10);
      real_year = 2024 - age;
      if(yeari != real_year)
      {
         printf("Enter a valid year(the year should be %d)\n", real_year);
         goto back;
      }
   getchar(); 
   char colony[20];
   char road_name[20];
   char city[15];
   char state[20];
   char pin_code[7];
   
   do 
   {
      printf("Enter the address\nRoad name: ");
      fgets(road_name, sizeof(road_name), stdin);
      road_name[strcspn(road_name, "\n")] = '\0';
   } while (!valid_address(road_name));

   do 
   {
      printf("Colony: ");
      fgets(colony, sizeof(colony), stdin);
      colony[strcspn(colony, "\n")] = '\0';
   } while (!valid_address(colony));

   do 
   {
      printf("City: ");
      fgets(city, sizeof(city), stdin);
      city[strcspn(city, "\n")] = '\0';
   } while (!valid_address(city));
   
   do 
   {
      printf("State: ");
      fgets(state, sizeof(state), stdin);
      state[strcspn(state, "\n")] = '\0';
   } while (!valid_address(state));

   do 
   {
      printf("Pin code: ");
      scanf("%s", pin_code);
      while (getchar() != '\n');
      if (!valid_pin_code(pin_code)) 
      {
         printf("Enter a valid pin code with 6 digits\n");
      }
   } while (!valid_pin_code(pin_code));

   snprintf(newnode->teacher_address, sizeof(newnode->teacher_address),"%s %s %s %s %s", road_name, colony, city, state, pin_code);

   strcpy(newnode->subject_teacher , subject);
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   strftime(newnode->joining_date_t, sizeof(newnode->joining_date_t), "%d/%m/%Y", tm);
   newnode->teacher_age = age;
   change_teacher = 1;



   char query[300];

   snprintf(query, sizeof(query) , "insert into teacher values('%s','%s','%s','%s',%d,'%s','%s','%d')", newnode->teacher_name , newnode->teacher_num , newnode->teacher_birthdate , newnode->teacher_address , newnode->teacher_age , newnode->subject_teacher , newnode->joining_date_t , newnode->teacher_std);
   PGresult *res = PQexec(conn, query);
   ExecStatusType resStatus = PQresultStatus(res);
   

   if (PQresultStatus(res) != PGRES_COMMAND_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res);
   
      return 1;
   }

   free(newnode);
   PQclear(res);
   printf("Data processed successfully.\n");
   return 0;

}

int password(PGconn *conn)
{
    char term;
   char name[20];
   char pass[13];
   char pass2[13];
   int attempts;
   if (PQstatus(conn) != CONNECTION_OK) 
   {
      fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
      return 0;
   }
   const char *query = "SELECT CASE WHEN EXISTS (SELECT * FROM password LIMIT 1) THEN 1 ELSE 0 END;";
   PGresult *res_d = PQexec(conn, query);
   if (PQresultStatus(res_d) != PGRES_TUPLES_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));


      return 0;
   }
   int exists = atoi(PQgetvalue(res_d, 0, 0));
   PQclear(res_d);
   if (exists == 0) 
   {
      printf("There is no password existing\nCreate new one\n");
      while (1) 
      {

         do 
         {
            printf("Enter the name of the user: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
         } 
         while (!valid_name(name));
         
         
         while (1) 
         {
            printf("Enter a new password (8-12 characters): ");
            scanf("%s", pass);
            int len = strlen(pass);
            if (len >= 8 && len <= 12) 
            {
               break;
            } 
            else 
            {
               printf("The password should be between 8 and 12 characters\n");
            }
         }
         int len = strlen(pass);
         if (len >= 8 && len <= 12) 
         {
            char insert_query[100];
            snprintf(insert_query, sizeof(insert_query), "INSERT INTO password (name, pass) VALUES('%s', '%s')", name, pass);
            PGresult *insert_res = PQexec(conn, insert_query);
            if (PQresultStatus(insert_res) != PGRES_COMMAND_OK) 
            {
               printf("Error while executing the insert query: %s\n", PQerrorMessage(conn));
            } 
            else 
            {
               printf("Password added successfully.\n");
            }
            PQclear(insert_res);
            return 0;
         } 
         else 
         {
            printf("The password should be between 8 and 12 characters.\n");
         }
      }
   }
    else 
    {
        const char *query = "SELECT * FROM password";
        PGresult *res = PQexec(conn, query);
        ExecStatusType resStatus = PQresultStatus(res);

        if (resStatus != PGRES_TUPLES_OK) 
        {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res);
             
            return 1;
        }

        char pass2[13];
        int attempts;
        while (1) 
        {
            for (attempts = 0; attempts < 3; attempts++) 
            {
                printf("Enter the password: ");
                scanf("%12s", pass2);
                int rows = PQntuples(res);
                int found = 0;
                for (int i = 0; i < rows; i++) 
                {
                    char password[13];
                    strncpy(password, PQgetvalue(res, i, 0), sizeof(password) - 1);
                    password[sizeof(password) - 1] = '\0';
                    if (strcmp(password, pass2) == 0) 
                    {
                        found = 1;
                        break;
                    }
                }
                if (found) 
                {
                    printf("Access granted.\n");
                    PQclear(res);
                    
                    return 2;
                } 
                else 
                {
                    printf("Incorrect password. Try again.\n");
                }
            }
            printf("Too many incorrect attempts.\n");
            printf("Enter 1 to re-try and 0 to exit\n");
            int choice;
            while (1) 
            {
                printf("Enter: ");
                if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
                {
                    break;
                } 
                else 
                {
                    printf("Error, Enter a valid input\n");
                    term = 0;
                    while (term != '\n' && term != EOF) 
                    {
                        term = getchar();
                    }
                }
            }
            
            if (choice == 0) 
            {
                PQclear(res);
                return 0;
            }
        }
    }

    PQclear(res_d);

    return 0;
}

int change_pass(PGconn *conn)
{


    char pass[13], pass2[13];
    while (1) 
    {
        printf("Enter the password you want to change: ");
        back:
        scanf("%12s", pass);

        const char *query = "SELECT * FROM password";
        PGresult *res = PQexec(conn, query);
        ExecStatusType resStatus = PQresultStatus(res);
        if (resStatus != PGRES_TUPLES_OK) 
        {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res);
 
            return 1;
        }

        int rows = PQntuples(res);
        int exists = 0;
        for (int i = 0; i < rows; i++) 
        {
            char stored_pass[13];
            strncpy(stored_pass, PQgetvalue(res, i, 0), sizeof(stored_pass) - 1);
            stored_pass[sizeof(stored_pass) - 1] = '\0';
            if (strcmp(stored_pass, pass) == 0) 
            {
                exists = 1;
                break;
            }
        }
        PQclear(res);

        if (!exists) 
        {
            printf("This password does not exist.\nEnter again: ");
            goto back;
        }
         printf("Enter a new password (8-12 characters): ");
         while (1) 
         {
            
            scanf("%s", pass2);
            int len = strlen(pass2);
            if (len >= 8 && len <= 12) 
            {
               break;
            } 
            else 
            {
               printf("The password should be between 8 and 12 characters\n");
            }
         }
        char update_query[150];
        snprintf(update_query, sizeof(update_query), "UPDATE password SET pass='%s' WHERE pass='%s'", pass2, pass);
        PGresult *res_update = PQexec(conn, update_query);
        ExecStatusType resStatus_update = PQresultStatus(res_update);   
        if (resStatus_update != PGRES_COMMAND_OK) 
        {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res_update);

            return 1;
        }

        printf("\nPassword changed successfully.\n");
        PQclear(res_update);
        return 0;
    }
}

int pass_add(PGconn *conn)
{
   char name[20];
   /*To add new passwords and the passwords are stores in database and only 5 passwords can be added*/

   if (PQstatus(conn) != CONNECTION_OK) 
   {
      printf("Connection to database failed: %s", PQerrorMessage(conn));
   
      return 1;
   }
   const char *query = "SELECT * FROM password";
   PGresult *res = PQexec(conn, query);
   ExecStatusType resStatus = PQresultStatus(res);

   if (resStatus != PGRES_TUPLES_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res);
    
      return 1;
   }
   int rows = PQntuples(res);
   PQclear(res);
   if (rows >= 5) 
   {
      printf("Cannot add more than 5 passwords.\n");
   
      return 1;
   }
   char pass[13];
   while (1) 
   {
               
      do 
      {
         printf("Enter the name of the user:- ");
         fgets(name, sizeof(name), stdin);
         name[strcspn(name, "\n")] = '\0';
      } 
      while (!valid_name(name));

      printf("Enter a new password (8-12 characters): ");
      scanf("%12s", pass);
      int len = strlen(pass);
      if (len >= 8 && len <= 12) 
      {
         char insert_query[100];
         snprintf(insert_query, sizeof(insert_query), "INSERT INTO password VALUES('%s', '%s')", pass, name);
         PGresult *res = PQexec(conn, insert_query);
         ExecStatusType resStatus = PQresultStatus(res);   
         if (resStatus != PGRES_COMMAND_OK) 
         {
            printf("The password has been used enter different\n");
            PQclear(res);
         
            return 1;
         }
         printf("Password added successfully.\n");
         PQclear(res);
      
         return 0;
      } 
      else 
      {
         printf("The password should be between 8 and 12 characters.\n");
      }
   }
}
int delete_pass(PGconn *conn)
{
   /*deleting the particular password which is refelected in database too*/

   if (PQstatus(conn) != CONNECTION_OK) 
   {
      fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
   
      return 1;
   }
   char pass[13];
   printf("Enter the password which you want to delete: ");
   scanf("%12s", pass);
   const char *query = "SELECT * FROM password";
   PGresult *res = PQexec(conn, query);
   ExecStatusType resStatus = PQresultStatus(res);
   if (resStatus != PGRES_TUPLES_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res);
    
      return 1;
   }
   int rows = PQntuples(res);
   int exists = 0;
   for (int i = 0; i < rows; i++) 
   {
      char stored_pass[13];
      strncpy(stored_pass, PQgetvalue(res, i, 0), sizeof(stored_pass) - 1);
      stored_pass[sizeof(stored_pass) - 1] = '\0';
      if (strcmp(stored_pass, pass) == 0) 
      {
         exists = 1;
         break;
      }
   }
   PQclear(res);
   if (!exists) 
   {
      printf("This password does not exist.\n");
   
      return 1;
   }
   char delete_query[50];
   snprintf(delete_query, sizeof(delete_query), "DELETE FROM password WHERE pass = '%s'", pass);
   PGresult *res_delete = PQexec(conn, delete_query);
   if (PQresultStatus(res_delete) != PGRES_COMMAND_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res_delete);
   
      return 1;  
   }
   printf("Password deleted successfully.\n");
   PQclear(res_delete);

   return 0;
}
int read_student_data(PGconn *conn)
{
   /*This reads all the student data from the database*/

   if (PQstatus(conn) != CONNECTION_OK) 
   {
      fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
   
      return 1;
   }
   const char *query = "SELECT * FROM student ORDER BY class";
   PGresult *res = PQexec(conn, query);

   ExecStatusType resStatus = PQresultStatus(res);

   if (resStatus != PGRES_TUPLES_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res);
    
      return 1;
   }

   int rows = PQntuples(res);
   for (int i = 0, j; i < rows; i++, j++) 
   {
      j =0;
      const char *json_data = PQgetvalue(res, i, 1); 

      cJSON *json_array = cJSON_Parse(json_data);
      if (json_array == NULL)
      {
         PQclear(res);
         return 1;
      }
         const char *class_s = PQgetvalue(res, i, j);
         int class = atoi(class_s);
      cJSON *item;
      cJSON_ArrayForEach(item, json_array)
      {
         struct student *newnode = (struct student *)calloc(1, sizeof(struct student));
         if (newnode == NULL)
         {
            printf("Memory allocation failed in student file.\n");
            cJSON_Delete(json_array);
            return 0;
         }  
         cJSON *json_field;
         json_field = cJSON_GetObjectItemCaseSensitive(item, "name");
         strcpy(newnode->student_name, json_field->valuestring);
         
         json_field = cJSON_GetObjectItemCaseSensitive(item, "blood_group");
         strcpy(newnode->blood_group, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "birth_of_date");
         strcpy(newnode->birth, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "religion");
         strcpy(newnode->religion, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "father_name");
         strcpy(newnode->par.father_name, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "father_num");
         strcpy(newnode->par.f_num, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "mother_name");
         strcpy(newnode->par.mother_name, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "mother_num");
         strcpy(newnode->par.m_num, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "address");
         strcpy(newnode->address, json_field->valuestring);

         json_field = cJSON_GetObjectItemCaseSensitive(item, "unpaid_fees");
         newnode->fees = json_field->valueint;

         newnode->grade.class = class;

         json_field = cJSON_GetObjectItemCaseSensitive(item , "joining_data");
         strcpy(newnode->joining_date_s , json_field->valuestring);

         if (head_student == NULL) 
         {
            head_student = temp_student = newnode;
         } 
         else 
         {
               temp_student = head_student;
               while (temp_student->next != NULL)
               {
                  temp_student = temp_student->next;

               }
               temp_student->next = newnode;
         }
      }

      cJSON_Delete(json_array);
      assign_rollno(1);

   }
   
   
   PQclear(res);

   return 0;

}

int read_teacher_data(PGconn *conn)
{
      /*This reads all the teacher data from the database*/


   if (PQstatus(conn) != CONNECTION_OK) 
   {
      fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
   
      return 1;
   }
   const char *query = "SELECT * FROM teacher";
   PGresult *res = PQexec(conn, query);
   if (PQresultStatus(res) != PGRES_TUPLES_OK) 
   {
      fprintf(stderr, "Error while executing the query: %s", PQerrorMessage(conn));
      PQclear(res);
   
      return 1;
   }

   int rows = PQntuples(res);

   for (int i = 0; i < rows; i++) 
   {
      struct teacher *newnode = (struct teacher *)calloc(1, sizeof(struct teacher));
      if (newnode == NULL)
      {
         printf("Memory allocation failed.\n");
         PQclear(res);
      
         return 1;
      }
      
      int j = 0;
      strcpy(newnode->teacher_name, PQgetvalue(res, i, j++));
      strcpy(newnode->teacher_num, PQgetvalue(res, i, j++));
      strcpy(newnode->teacher_birthdate, PQgetvalue(res, i, j++));
      strcpy(newnode->teacher_address, PQgetvalue(res, i, j++));
      const char *age_str = PQgetvalue(res, i, j++);
      strcpy(newnode->subject_teacher, PQgetvalue(res, i, j++));
      strcpy(newnode->joining_date_t, PQgetvalue(res, i, j++));
      const char *class_str = PQgetvalue(res, i, j++);
      newnode->teacher_age= atoi(age_str);
      newnode->teacher_std = atoi(class_str);
      if (head_teacher == NULL) 
      {
          head_teacher = newnode;
      } 
      else 
      {
         temp_teacher = head_teacher;
         while (temp_teacher->next != NULL)
         {
             temp_teacher = temp_teacher->next;
         }
         temp_teacher->next = newnode;
      }
   }

   PQclear(res);

   assign_rollno(0);
   return 1; 
}


int assign_rollno(int choice)
{
   /*This assigns rollno to the students by alphatic order and by class*/
   int roll = 0;
   int clas = 1;

   if (choice) 
   {
      if (head_student == NULL) 
      {
         printf("No data found\n");
         return 0;
      }

      struct student *i, *j ,*k;
      struct student temp;

      for (i = head_student; i != NULL; i = i->next) 
      {
         for (j = i->next; j != NULL; j = j->next) 
         {
            if (i->grade.class > j->grade.class) 
            {
               temp = *i;
               *i = *j;
               *j = temp;
               struct student *tempNext = i->next;
               i->next = j->next;
               j->next = tempNext;
            }
         }
      }
         int current_class =  1;
         for (i = head_student; i != NULL; i = i->next) 
         {
            if (i->grade.class != current_class) 
            {
               current_class = i->grade.class;
            }
            for (j = i->next; j != NULL && j->grade.class == current_class; j = j->next) 
            {
               if (strcmp(i->student_name, j->student_name) > 0) 
               {

                  struct student tempStudent = *i;
                  *i = *j;
                  *j = tempStudent;
                  struct student *tempNext = i->next;
                  i->next = j->next;
                  j->next = tempNext;
               }
            }
            current_class++;
         }

      temp_student = head_student;
      while (temp_student != NULL) 
      {
         roll = 0;
         while (temp_student != NULL && temp_student->grade.class == clas) 
         {
            roll++;
            temp_student->grade.roll.roll_no = roll;
            temp_student = temp_student->next;
         }
         clas++;
      }
   }
   else
   {
      /*This assigns teacher code to teacher*/

      int roll_no = 0;
      temp_teacher = head_teacher;
      while (temp_teacher != 0)
      {
         roll++;
         temp_teacher ->s_no = roll;
         temp_teacher = temp_teacher ->next;
   
      }
   }
}
int search_student_data()
{
   int choice = 1;
   int roll;
   int class_no;
   char alpha;
   int fees;
   char term;
   
/*To search the student data by rollno , class , first alphabet and pending fees*/
   while (choice)
   {
      printf("Press\n1. To search by roll number\n2. To search by grade\n3. To search by alphabet\n4. To search by pending fees\n0. To exit\n");
      while (1) 
      {
         if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
         {
            break;
         } 
         else 
         {
            printf("Error, Enter a valid input\n");
            term = 0;
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
         }
      }
      

      switch (choice)
      {
         case 1:
            temp_student = head_student;
            printf("Enter class: ");
            while (1) 
               {

                  if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                  {

                     break;
                  } else 
                  {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                  }
               }
               while (class_no < 1 || class_no > 12) 
               {
                  printf("Invalid data. Enter between 1st to 12th: ");
                  while (1) 
                  {

                     if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                     {
                     break;
                     } else 
                     {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                     }
                  }
               }
            printf("Enter roll number: ");
            while (1) 
            {
               if (scanf("%d%c", &roll, &term) == 2 && term == '\n') 
               {
                  break;
               } 
               else 
               {
                  printf("Error, Enter a valid input\n");
                  term = 0;
                  while (term != '\n' && term != EOF) 
                  {
                     term = getchar();
                  }
               }
            }
            int found = 0;
            while (temp_student != NULL)
            {
               if (temp_student->grade.class == class_no && temp_student->grade.roll.roll_no == roll)
               {
                  present_teacher(temp_student->teacher_for_grade);
                  printf("Name: %s\n", temp_student->student_name);
                  printf("Blood group: %s\n", temp_student->blood_group);
                  printf("Birth of date: %.10s\n", temp_student->birth);
                  printf("Religion: %s\n", temp_student->religion);
                  printf("Father name: %s\n", temp_student->par.father_name);
                  printf("Father number: %s\n", temp_student->par.f_num);
                  printf("Mother name: %s\n", temp_student->par.mother_name);
                  printf("Mother number: %s\n", temp_student->par.m_num);
                  printf("Address: %s\n", temp_student->address);
                  printf("Grade: %d\n", temp_student->grade.class);
                  printf("Pending fees: %d\n", temp_student->fees);
                  printf("Joining date: %s\n", temp_student->joining_date_s);
                  int i;
                  i = strlen(temp_student->teacher_for_grade);
                  if(i == 0)
                  {
                     printf("teacher: Not assigned yet\n\n");
                  }
                  else
                  {
                     printf("teacher: %s\n\n", temp_student->teacher_for_grade);
                  }
                  found = 1;
               }
               temp_student = temp_student->next;
            }
            if (!found)
            {
               printf("No student found with roll number %d in class %d.\n", roll, class_no);
            }
            break;

         case 2:
            temp_student = head_student;
            printf("Enter class: ");
            while (1) 
               {

                  if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                  {

                     break;
                  } else 
                  {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                  }
               }
               while (class_no < 1 || class_no > 12) 
               {
                  printf("Invalid data. Enter between 1st to 12th: ");
                  while (1) 
                  {

                     if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                     {
                     break;
                     } else 
                     {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                     }
                  }
               }
            found = 0;
            while (temp_student != NULL)
            {
               if (temp_student->grade.class == class_no)
               {
                  present_teacher(temp_student->teacher_for_grade);
                  printf("Name: %s\n", temp_student->student_name);
                  printf("Blood group: %s\n", temp_student->blood_group);
                  printf("Birth of date: %.10s\n", temp_student->birth);
                  printf("Religion: %s\n", temp_student->religion);
                  printf("Father name: %s\n", temp_student->par.father_name);
                  printf("Father number: %s\n", temp_student->par.f_num);
                  printf("Mother name: %s\n", temp_student->par.mother_name);
                  printf("Mother number: %s\n", temp_student->par.m_num);
                  printf("Address: %s\n", temp_student->address);
                  printf("Grade: %d\n", temp_student->grade.class);
                  printf("Roll number: %d\n", temp_student->grade.roll.roll_no);
                  printf("Pending fees: %d\n", temp_student->fees);
                  printf("Joining date: %s\n", temp_student->joining_date_s);
                  int i;
                  i = strlen(temp_student->teacher_for_grade);
                  if(i == 0)
                  {
                     printf("teacher: Not assigned yet\n\n");
                  }
                  else
                  {
                     printf("teacher: %s\n\n", temp_student->teacher_for_grade);
                  }
                  found = 1;
               }
               temp_student = temp_student->next;
            }
            if (!found)
            {
               printf("No students found in class %d.\n", class_no);
            }
            break;

         case 3:
            temp_student = head_student;
            printf("Enter class: ");
            while (1) 
               {

                  if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                  {

                     break;
                  } else 
                  {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                  }
               }
               while (class_no < 1 || class_no > 12) 
               {
                  printf("Invalid data. Enter between 1st to 12th: ");
                  while (1) 
                  {

                     if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                     {
                     break;
                     } else 
                     {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                     }
                  }
               }
            printf("Enter the first alphabet of the name: ");
            while (1)
            {
               scanf("%c", &alpha);
               if(!isalpha(alpha))
               {
                  printf("Enter first charater of the name: ");
               }
               else
               {
                  break;
               }
            }
            if(alpha >= 97 && alpha <= 122)
            {
               alpha = alpha - 32;
            }
            found = 0;
            while (temp_student != NULL)
            {
               if (temp_student->grade.class == class_no && temp_student->student_name[0] == alpha)
               {
                  present_teacher(temp_student->teacher_for_grade);
                  printf("Name: %s\n", temp_student->student_name);
                  printf("Blood group: %s\n", temp_student->blood_group);
                  printf("Birth of date: %.10s\n", temp_student->birth);
                  printf("Religion: %s\n", temp_student->religion);
                  printf("Father name: %s\n", temp_student->par.father_name);
                  printf("Father number: %s\n", temp_student->par.f_num);
                  printf("Mother name: %s\n", temp_student->par.mother_name);
                  printf("Mother number: %s\n", temp_student->par.m_num);
                  printf("Address: %s\n", temp_student->address);
                  printf("Grade: %d\n", temp_student->grade.class);
                  printf("Roll number: %d\n", temp_student->grade.roll.roll_no);
                  printf("Pending fees: %d\n", temp_student->fees);
                  printf("Joining date: %s\n", temp_student->joining_date_s);
                  int i;
                  i = strlen(temp_student->teacher_for_grade);
                  if(i == 0)
                  {
                     printf("teacher: Not assigned yet\n\n");
                  }
                  else
                  {
                     printf("teacher: %s\n\n", temp_student->teacher_for_grade);
                  }
                  found = 1;
               }
               temp_student = temp_student->next;
            }
            if (!found)
            {
               printf("No students found with name starting with '%c' in class %d.\n", alpha, class_no);
            }
            break;

         case 4:
            temp_student = head_student;
            printf("Enter class: ");
            while (1) 
               {

                  if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                  {

                     break;
                  } else 
                  {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                  }
               }
               while (class_no < 1 || class_no > 12) 
               {
                  printf("Invalid data. Enter between 1st to 12th: ");
                  while (1) 
                  {

                     if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
                     {
                     break;
                     } else 
                     {
                        printf("Enter 1 to 12 numbers only\n");
                        term = 0;
                        while (term != '\n' && term != EOF) 
                        {
                           term = getchar();
                        }
                     }
                  }
               }
            found = 0;
            while (temp_student != NULL)
            {
               if (temp_student->grade.class == class_no && temp_student->fees > 0)
               {
                  present_teacher(temp_student->teacher_for_grade);
                  printf("Name: %s\n", temp_student->student_name);
                  printf("Blood group: %s\n", temp_student->blood_group);
                  printf("Birth of date: %.10s\n", temp_student->birth);
                  printf("Religion: %s\n", temp_student->religion);
                  printf("Father name: %s\n", temp_student->par.father_name);
                  printf("Father number: %s\n", temp_student->par.f_num);
                  printf("Mother name: %s\n", temp_student->par.mother_name);
                  printf("Mother number: %s\n", temp_student->par.m_num);
                  printf("Address: %s\n", temp_student->address);
                  printf("Grade: %d\n", temp_student->grade.class);
                  printf("Roll number: %d\n", temp_student->grade.roll.roll_no);
                  printf("Pending fees: %d\n", temp_student->fees);
                  printf("Joining date: %s\n", temp_student->joining_date_s);
                  int i;
                  i = strlen(temp_student->teacher_for_grade);
                  if(i == 0)
                  {
                     printf("teacher: Not assigned yet\n\n");
                  }
                  else
                  {
                     printf("teacher: %s\n\n", temp_student->teacher_for_grade);
                  }
                  
                  found = 1;
               }
               temp_student = temp_student->next;
            }
            if (!found)
            {
               printf("No students with pending fees found in class %d.\n", class_no);
            }
            break;

         case 0:
            choice = 0;
            
            break;

         default:
            printf("Error, Enter a valid input\n");
      }
      printf("----------------------------------------------------------------------------------------------------\n");
   }
}

int edit_student_data(PGconn *conn)
{
   /*To change the data of particlar student which reflects in database*/
   int class_no;
   int roll;
   int found = 0;
   char term;
   char name[20];
   char years[5];
   int yeari;
   char *endptr;
   printf("Enter class: ");
   while (1) 
      {
         if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
         {
            break;
         } else 
         {
               printf("Enter 1 to 12 numbers only\n");
               term = 0;
               while (term != '\n' && term != EOF) 
               {
                  term = getchar();
               }
         }
      }
      while (class_no < 1 || class_no > 12) 
      {
         printf("Invalid data. Enter between 1st to 12th: ");
         while (1) 
         {
            if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
            {
            break;
            } else 
            {
               printf("Enter 1 to 12 numbers only\n");
               term = 0;
               while (term != '\n' && term != EOF) 
               {
                  term = getchar();
               }
            }
         }
      }
   printf("Enter roll number: ");
   while (1) 
   {
      if (scanf("%d%c", &roll, &term) == 2 && term == '\n') 
      {
         break;
      } 
      else 
      {
         term = 0;
         while (term != '\n' && term != EOF) 
         {
            term = getchar();
         }
      }
   }
   temp_student = head_student;
   
   while (temp_student != NULL)
   {
      if (temp_student->grade.class == class_no && temp_student->grade.roll.roll_no == roll)
      {
         strcpy(name , temp_student->student_name);
         printf("\n\nEnter the new details\n");
          do 
          {
             printf("Name: ");
             fgets(temp_student->student_name, sizeof(temp_student->student_name), stdin);
             temp_student->student_name[strcspn(temp_student->student_name, "\n")] = '\0';
          } 
          while (!valid_name(temp_student->student_name));
          if(temp_student->student_name[0] >= 97 && temp_student->student_name[0] <= 122)
          {
             temp_student->student_name[0] = temp_student->student_name[0] - 32;
          }

            printf("Blood group: ");
            scanf("%s", temp_student->blood_group);
            for(int i = 0 ; temp_student->blood_group[i] != '\0' ; i++)
            {
               if(temp_student->blood_group[i] >= 97 && temp_student->blood_group[i] <= 122)
               {
                  temp_student->blood_group[i] = temp_student->blood_group[i] - 32;
               }
            }
            while(strcmp(temp_student->blood_group, "O+") != 0 && 
               strcmp(temp_student->blood_group, "O-") != 0 &&
               strcmp(temp_student->blood_group, "A+") != 0 && 
               strcmp(temp_student->blood_group, "A-") != 0 &&
               strcmp(temp_student->blood_group, "B+") != 0 &&
               strcmp(temp_student->blood_group, "B-") != 0 &&
               strcmp(temp_student->blood_group, "AB+") != 0 &&
               strcmp(temp_student->blood_group, "AB-") != 0) 
            {
               printf("Enter a valid blood group\n");
               scanf("%s", temp_student->blood_group);
               for(int i = 0 ; temp_student->blood_group[i] != '\0' ; i++)
               {
                  if(temp_student->blood_group[i] >= 97 && temp_student->blood_group[i] <= 122)
                  {
                     temp_student->blood_group[i] = temp_student->blood_group[i] - 32;
                  }
               }
            }
   time_t now;
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);

    int present_year = timeinfo->tm_year + 1900; 
   do
   {
      printf("Enter your Date of birth(DD/MM/YYYY): ");
      scanf("%s", temp_student->birth);
      if(!valid_birth(temp_student->birth))
      {
         printf("Enter in (DD/MM/YYYY) format \n");
         scanf("%s",temp_student->birth);
      }
   }
   while(!valid_birth(temp_student->birth));
   int age;
   int year;
   age = temp_student->grade.class + 6;
   year = present_year - age;
   years[0] = temp_student->birth[6];
   years[1] = temp_student->birth[7];
   years[2] = temp_student->birth[8];
   years[3] = temp_student->birth[9];
   years[4] = '\0';
   yeari = strtol(years , &endptr , 10);
   if(!(year -2 <= yeari && year+2  >= yeari))
   {
      if(yeari < year)
      {
         printf("You are too old to study in %d class", temp_student->grade.class);
      }
      if(yeari > year)
      {
         printf("You are too young to study in %d class", temp_student->grade.class);
      }
      free(temp_student);
      return 0;
   }
         do
         {
             printf("Religion: ");
             scanf(" %s", temp_student->religion);
             if (!valid_religion(temp_student->religion)) 
             {
                printf("Invalid religion. Please enter a valid religion (Hindu, Muslim, Christian, Jain, Buddhist, Sikh or Others)).\n");
             }
         } while (!valid_religion(temp_student->religion));
         getchar();
         do 
         {
            printf("Father Name: ");
            fgets(temp_student->par.father_name , sizeof(temp_student->par.father_name), stdin);
            temp_student->par.father_name[strcspn(temp_student->par.father_name, "\n")] = '\0';
         }
         while(!valid_name(temp_student->par.father_name));
         do 
         {
             printf("Father's number: ");
             scanf("%s", temp_student->par.f_num);
             if (!is_valid_num(temp_student->par.f_num)) 
             {
                 printf("Invalid number. Please enter a valid 10-digit number.\n");
             }
         } 
         while (!is_valid_num(temp_student->par.f_num));
         getchar();
         do 
         {
            printf("Mother name: ");
            fgets(temp_student->par.mother_name, sizeof(temp_student->par.mother_name), stdin);
            temp_student->par.mother_name[strcspn(temp_student->par.mother_name, "\n")] = '\0';
         } 
         while (!valid_name(temp_student->par.mother_name));
         do 
         {
             printf("Mother's number: ");
             scanf("%s", temp_student->par.m_num);
             if (!is_valid_num(temp_student->par.m_num)) 
             {
                 printf("Invalid number. Please enter a valid 10-digit number.\n");
             }
         } 
         while (!is_valid_num(temp_student->par.m_num));
         getchar(); 
         char colony[20];
         char road_name[20];
         char city[15];
         char state[20];
         char pin_code[7];
         
         do 
         {
            printf("Enter the address\nRoad name: ");
            fgets(road_name, sizeof(road_name), stdin);
            road_name[strcspn(road_name, "\n")] = '\0';
         } while (!valid_address(road_name));

         do 
         {
            printf("Colony: ");
            fgets(colony, sizeof(colony), stdin);
            colony[strcspn(colony, "\n")] = '\0';
         } while (!valid_address(colony));

         do 
         {
            printf("City: ");
            fgets(city, sizeof(city), stdin);
            city[strcspn(city, "\n")] = '\0';
         } while (!valid_address(city));
         
         do 
         {
            printf("State: ");
            fgets(state, sizeof(state), stdin);
            state[strcspn(state, "\n")] = '\0';
         } while (!valid_address(state));

         do 
         {
            printf("Pin code: ");
            scanf("%s", pin_code);
            while (getchar() != '\n');
            if (!valid_pin_code(pin_code)) 
            {
               printf("Enter a valid pin code with 6 digits\n");
            }
         } while (!valid_pin_code(pin_code));

         snprintf(temp_student->address, sizeof(temp_student->address),"%s %s %s %s %s", road_name, colony, city, state, pin_code);
         found = 1;

         int i, xy=0;
         char query_D[256], name_check[20];
         snprintf(query_D, sizeof(query_D), "SELECT * FROM student where class=%d", class_no);  
         PGresult *res_d = PQexec(conn, query_D);   
            if (PQresultStatus(res_d) != PGRES_TUPLES_OK) 
            {
               printf("Error while executing the query: %s\n", PQerrorMessage(conn));
               PQclear(res_d);
               return 1;
            }
            int rows = PQntuples(res_d);  
            for(i=0 ; i<=rows; i++)
            {
               const char *json_data = PQgetvalue(res_d, 0, i);  
               cJSON *json_array = cJSON_Parse(json_data);
               if (json_array == NULL)
               {
                   printf("Error parsing JSON data.\n");
                   PQclear(res_d);
                   cJSON_Delete(json_array);
                   return 1;
               }
               cJSON *json_field;
               cJSON *item;
               cJSON_ArrayForEach(item, json_array)
               {
                  json_field= cJSON_GetObjectItemCaseSensitive(item, "name");
                  strcpy(name_check , json_field->valuestring);
                  printf("%s\n", name_check);
                  xy++;
                  if(strcmp(name , name_check)==0)
                  {
                     cJSON_Delete(json_array);
                     goto front;
                  }
               }
               cJSON_Delete(json_array); 
            } 
            char query[550];

              front:
                 xy = xy-1;

         snprintf(query, sizeof(query), " UPDATE student SET data = jsonb_set(data::jsonb, '{%d}', '{\"name\": \"%s\", \"blood_group\": \"%s\", \"birth_of_date\": \"%s\", \"father_name\": \"%s\", \"father_num\": \"%s\", \"mother_name\": \"%s\", \"mother_num\": \"%s\", \"address\": \"%s\", \"unpaid_fees\": %d, \"joining_data\": \"%s\", \"religion\": \"%s\"}'::jsonb) WHERE data::jsonb @> '[{\"name\": \"%s\"}]'",
         xy , temp_student->student_name , temp_student->blood_group, temp_student->birth, temp_student->par.father_name, temp_student->par.f_num, temp_student->par.mother_name, temp_student->par.m_num, temp_student->address, temp_student->fees,temp_student->joining_date_s, temp_student->religion, name); 


         PGresult *res = PQexec(conn, query);
         if (PQresultStatus(res) != PGRES_COMMAND_OK) 
         {
            fprintf(stderr, "Update failed: %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQclear(res_d);
            return 0;
         } 
         else 
         {
             printf("Update successful.\n");
         }

         PQclear(res);
         PQclear(res_d);
         return 0;
      }
      temp_student = temp_student->next;
   }
   if (!found)
   {
       printf("No data found for roll number %d in class %d.\n", roll, class_no);
   }
   return 0;
}
int search_teacher_data()
{
   /*to search teacher data by teacher code and to display all*/
   int roll;
   
   int choice =1;
   char term;
   if(head_teacher == 0)
   {
      printf("NO data found\n");
      return 0;
   }
   while(choice)
   {

   printf("Press\n1. To search by teacher code\n2. To display all\n0. To exit\n");
   while (1) 
   {
      printf("Enter: ");
      if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
      {
         break;
      } 
      else 
      {
         printf("Error, Enter a valid input\n");
         term = 0;
         while (term != '\n' && term != EOF) 
         {
             term = getchar();
         }
      }
   }
      switch(choice)
      {
         case 1:
            temp_teacher = head_teacher;
            printf("Enter the teacher code: ");
            while (1) 
            {
               if (scanf("%d%c", &roll, &term) == 2 && term == '\n') 
               {
                  break;
               } 
               else 
               {
                  printf("Error, Enter a valid input\n");
                  term = 0;
                  while (term != '\n' && term != EOF) 
                  {
                     term = getchar();
                  }
               }
            }
            while (temp_teacher != 0)
            {
               if(temp_teacher->s_no == roll)
               {
                  printf("\n\nName: %s\n", temp_teacher->teacher_name);
                  printf("Teacher number: %s\n", temp_teacher->teacher_num);
                  printf("Teacher date of birth: %s\n", temp_teacher->teacher_birthdate);
                  printf("Teacher address: %s\n", temp_teacher->teacher_address);
                  temp_teacher->subject_teacher[0] -= 32;
                  printf("%s subject teacher\n", temp_teacher->subject_teacher);
                  printf("Teacher age: %d\n", temp_teacher->teacher_age);
                  printf("Teacher code: %d\n", temp_teacher->s_no);
                  if(temp_teacher->teacher_std == 0)
                  {
                     printf("%s is not assigned yet\n", temp_teacher->teacher_name);
                  }
                  else
                  {
                     printf("Teaching to %d class\n", temp_teacher->teacher_std);
                  }
                  printf("Teacher joining date: %s\n", temp_teacher->joining_date_t);
               }
               temp_teacher = temp_teacher->next;
            }
            break;
         case 2:
            temp_teacher = head_teacher;
            while (temp_teacher != 0)
            {
               printf("\n\nName: %s\n", temp_teacher->teacher_name);
               printf("Teacher number: %s\n", temp_teacher->teacher_num);
               printf("Teacher date of birth: %s\n", temp_teacher->teacher_birthdate);
               printf("Teacher address: %s\n", temp_teacher->teacher_address);
               temp_teacher->subject_teacher[0] -= 32;
               printf("%s subject teacher\n", temp_teacher->subject_teacher);
               printf("Teacher age: %d\n", temp_teacher->teacher_age);
               printf("Teacher code: %d\n", temp_teacher->s_no);
               if(temp_teacher->teacher_std == 0)
               {
                  printf("%s is not assigned yet\n", temp_teacher->teacher_name);
               }
               else
               {
                  printf("Teaching to %d class\n", temp_teacher->teacher_std);
               }
               printf("Teacher joining date: %s\n", temp_teacher->joining_date_t);
               temp_teacher = temp_teacher->next;
               printf("\n\n");
            }
            break;
         
         case 0:
            choice =0;
            break;
         
         default:
         printf("Error, Enter a valid input\n");
      }
   }

}
int edit_teacher_data(PGconn *conn)
{
   /*To change the data of particlar teacher which reflects in database*/
   int a= 1;
   int real_year;
   char *endptr;
   int yeari;
   char years[5];
   int no;
   int found = 0;
   char term, name[20];
   printf("Enter the teacher code\n");
   while (1) 
   {
      if (scanf("%d%c", &no, &term) == 2 && term == '\n') 
      {
         break;
      } 
      else 
      {
         term = 0;
         while (term != '\n' && term != EOF) 
         {
            term = getchar();
         }
      }
   }

   temp_teacher = head_teacher;
   while (temp_teacher != NULL) 
   {
      if(temp_teacher->s_no == no)
      {
         strcpy(name , temp_teacher->teacher_name);
         printf("\n\nEnter the new details\n");
         do
         {
            printf("Name: ");
            fgets(temp_teacher->teacher_name, sizeof(temp_teacher->teacher_name), stdin);
            temp_teacher->teacher_name[strcspn(temp_teacher->teacher_name, "\n")] = '\0';
         } 
         while (!valid_name(temp_teacher->teacher_name));

         do 
         {
            printf("Phone number: ");
            scanf("%s", temp_teacher->teacher_num);
            if (!is_valid_num(temp_teacher->teacher_num)) 
            {
               printf("Invalid number. Please enter a valid 10-digit number.\n");
            }
         } 
         while(!is_valid_num(temp_teacher->teacher_num));

   do
   {
      
      printf("Enter your Date of birth(DD/MM/YYYY): ");
      back:
      scanf("%s", temp_teacher->teacher_birthdate);
      years[0] = temp_teacher->teacher_birthdate[6];
      years[1] = temp_teacher->teacher_birthdate[7];
      years[2] = temp_teacher->teacher_birthdate[8];
      years[3] = temp_teacher->teacher_birthdate[9];
      years[4] = '\0';

      yeari = strtol(years , &endptr , 10);
      real_year = 2024 - temp_teacher->teacher_age;
      if(yeari != real_year)
      {
         a = 0;
      }
      if(!valid_birth(temp_teacher->teacher_birthdate) )
      {
        printf("Enter in (DD/MM/YYYY) format\n");
        scanf("%s",temp_teacher->teacher_birthdate);
      }
   }
   while(!valid_birth(temp_teacher->teacher_birthdate));

      years[0] = temp_teacher->teacher_birthdate[6];
      years[1] = temp_teacher->teacher_birthdate[7];
      years[2] = temp_teacher->teacher_birthdate[8];
      years[3] = temp_teacher->teacher_birthdate[9];
      years[4] = '\0';

      yeari = strtol(years , &endptr , 10);
      real_year = 2024 - temp_teacher->teacher_age;
      if(yeari != real_year)
      {
         printf("Enter a valid year(the year should be %d)\n", real_year);
         goto back;
      }
   getchar(); 

         

         char colony[20];
         char road_name[20];
         char city[15];
         char state[20];
         char pin_code[7];
         
         do 
         {
            printf("Enter the address\nRoad name: ");
            fgets(road_name, sizeof(road_name), stdin);
            road_name[strcspn(road_name, "\n")] = '\0';
         } while (!valid_address(road_name));

         do 
         {
            printf("Colony: ");
            fgets(colony, sizeof(colony), stdin);
            colony[strcspn(colony, "\n")] = '\0';
         } while (!valid_address(colony));

         do 
         {
            printf("City: ");
            fgets(city, sizeof(city), stdin);
            city[strcspn(city, "\n")] = '\0';
         } while (!valid_address(city));
         
         do 
         {
            printf("State: ");
            fgets(state, sizeof(state), stdin);
            state[strcspn(state, "\n")] = '\0';
         } while (!valid_address(state));

         do 
         {
            printf("Pin code: ");
            scanf("%s", pin_code);
            while (getchar() != '\n');
            if (!valid_pin_code(pin_code)) 
            {
               printf("Enter a valid pin code with 6 digits\n");
            }
         } while (!valid_pin_code(pin_code));

         snprintf(temp_teacher->teacher_address, sizeof(temp_teacher->teacher_address),
                  "%s %s %s %s %s", road_name, colony, city, state, pin_code);
         found = 1;
         break;  
      }
      temp_teacher = temp_teacher->next;
   }
   if(found == 0)
   {
      printf("No data found with %d teacher code\n", no);
      return 1;
   }
   
   printf("1\n");


   if (PQstatus(conn) != CONNECTION_OK) 
   {
      fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
   
      return 1;
   }

   char query[1024]; 
   snprintf(query, sizeof(query),
       "UPDATE teacher SET teacher_name='%s', teacher_number='%s', teacher_birthdate='%s', teacher_address='%s', teacher_age=%d, subject='%s', joining_date='%s', teacher_class=%d WHERE teacher_name='%s'",
       temp_teacher->teacher_name ,
       temp_teacher->teacher_num ,
       temp_teacher->teacher_birthdate ,
       temp_teacher->teacher_address ,
       temp_teacher->teacher_age,
       temp_teacher->subject_teacher ,
       temp_teacher->joining_date_t ,
       temp_teacher->teacher_std,
       name);

   PGresult *res = PQexec(conn, query);
   ExecStatusType resStatus = PQresultStatus(res);

   

   if (resStatus != PGRES_COMMAND_OK) 
   {
      printf("Error while executing the query: %s\n", PQerrorMessage(conn));
      PQclear(res);
   
      return 1;
   }
   printf("The data is updated\n");


   PQclear(res);

   return 0;
}


int remove_student_data(PGconn *conn)
{
   /*To remove a particlar student and also remove from the database*/
   int class_no;
   int roll;
   char term, Name[20];
   printf("Enter the class: ");
   while (1) 
   {
      if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
      {
         break;
      } else 
      {
            printf("Enter 1 to 12 numbers only\n");
            term = 0;
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
      }
   }
   while (class_no < 1 || class_no > 12) 
   {
      printf("Invalid data. Enter between 1st to 12th: ");
      while (1) 
      {
         if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
         {
         break;
         } else 
         {
            printf("Enter 1 to 12 numbers only\n");
            term = 0;
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
         }
      }
   }
   printf("Enter the roll no: ");
   while (1) 
   {
      if (scanf("%d%c", &roll, &term) == 2 && term == '\n') 
      {
         break;
      } 
      else 
      {
         term = 0;
         while (term != '\n' && term != EOF) 
         {
            term = getchar();
         }
      }
   }
   if (head_student == NULL) 
   {
       printf("No students available.\n");
       return 0;
   }
   struct student *temp = NULL, *temp_student = head_student;
   
   char ent;
   while (temp_student != NULL) 
   {
      if (temp_student->grade.class == class_no && temp_student->grade.roll.roll_no == roll) 
      {
         strcpy(Name, temp_student->student_name);
         if (temp_student->fees > 0)
         {
            printf("%s from %d std has pending fees of %d, please clear before exiting\n", temp_student->student_name, temp_student->grade.class, temp_student->fees);
            sleep(1);
            pay_fees_leaving(class_no, roll,conn);
         }

         if (temp_student == head_student)
         {
            head_student = temp_student->next;
         }
         else
         {
            temp->next = temp_student->next;
         }

//       UPDATE school 
// SET data = (
//     SELECT jsonb_agg(elem) 
//     FROM jsonb_array_elements(data::jsonb) AS elem
//     WHERE elem->>'Name' <> 'Rakshit'
// ) 
// WHERE data::jsonb @> '[{"Name": "Rakshit"}]';
         char query_D[256], name_check[20];
         snprintf(query_D, sizeof(query_D), "UPDATE student SET data = (SELECT jsonb_agg(elem) FROM jsonb_array_elements(data::jsonb) AS elem WHERE elem->>'name' <> '%s') WHERE data::jsonb @> '[{\"name\": \"%s\"}]'", Name, Name);  
         PGresult *res_d = PQexec(conn, query_D);   


         free(temp_student);
         printf("Student with roll number %d and class number %d removed successfully.\n", roll, class_no);
         struct student *temp_s;
         while (head_student != 0)
         {
            temp_s = head_student;
            head_student = head_student->next;
            free(temp_s);
         }
         head_student = NULL;
         read_student_data(conn);
         PQclear(res_d);
         return 0;
      }
      temp = temp_student;
      temp_student = temp_student->next;
   }
   printf("Student with roll number %d and class number %d not found.\n", roll, class_no);
   
   return 0;
}
int remove_teacher_data(PGconn *conn)
{
      /*To remove a particlar teacher and also remove from the database*/
    int roll;
    char term;
    char  name[20];
    printf("Enter the teacher code: ");
    while (1) 
    {
        if (scanf("%d%c", &roll, &term) == 2 && term == '\n') 
        {
            break;
        } 
        else 
        {
            term = 0;
            while (term != '\n' && term != EOF) 
            {
                term = getchar();
            }
        }
    }

    if (roll < 1) 
    {
        printf("Invalid roll number.\n");
        return 0;
    }

    if (head_teacher == NULL) 
    {
        printf("No teachers available.\n");
        return 0;
    }
    struct teacher *temp = head_teacher;
    struct teacher *prev = NULL;

    while (temp != NULL) 
    {
        if (temp->s_no == roll) 
        {
            if (prev == NULL) 
            {
               head_teacher = temp->next;
            } 
            else 
            {
               prev->next = temp->next;
            }
         
            if (PQstatus(conn) != CONNECTION_OK) 
            {
               fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
            
               return 1;
            }

            char query[1024];
           snprintf(query, sizeof(query), "DELETE FROM teacher WHERE teacher_name='%s' AND teacher_address='%s'", temp->teacher_name, temp->teacher_address);
 
            PGresult *res = PQexec(conn, query);
            ExecStatusType resStatus = PQresultStatus(res);

            

            if (resStatus != PGRES_COMMAND_OK) 
            {
               printf("Error while executing the query: %s\n", PQerrorMessage(conn));
               PQclear(res);
            
               return 1;
            }

         

            PQclear(res);
         

            free(temp);


            printf("Teacher with teacher code %d removed successfully.\n", roll);
            struct teacher *temp_t;
            while (head_teacher != 0)
            {
               temp_t = head_teacher;
               head_teacher = head_teacher->next;
               free(temp_t);
            }
            head_teacher = NULL;
            read_teacher_data(conn);
            return 0;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Teacher with code %d not found.\n", roll);
    return 0;
}


int allocating_teacher(PGconn *conn)
{
    char term, name[20];
    int class_no = 0, roll;
    int teacher_found = 0;

    // Input teacher code
    printf("Enter teacher code: ");
    while (1) {
        if (scanf("%d%c", &roll, &term) == 2 && term == '\n') {
            break;
        } else {
            printf("Invalid input. Please enter a valid teacher code.\n");
            while ((term = getchar()) != '\n' && term != EOF);
        }
    }

    // Check if the teacher exists
    temp_teacher = head_teacher;
    while (temp_teacher != NULL) {
        if (temp_teacher->s_no == roll) {
            teacher_found = 1;
            break;
        }
        temp_teacher = temp_teacher->next;
    }

    if (!teacher_found) {
        printf("The teacher is not found. Please enter a different code.\n");
        return 0; // Exit without allocation
    }

    // Input class number
    while (1) {
        printf("Enter class (1-12): ");
        if (scanf("%d%c", &class_no, &term) == 2 && term == '\n' && class_no >= 1 && class_no <= 12) {
            break;
        } else {
            printf("Invalid data. Please enter a number between 1 and 12.\n");
            while ((term = getchar()) != '\n' && term != EOF);
        }
    }

    // Check if the class has students
    temp_student = head_student;
    int class_has_students = 0;
    while (temp_student != NULL) {
        if (temp_student->grade.class == class_no) {
            class_has_students = 1;
            break;
        }
        temp_student = temp_student->next;
    }
    if (!class_has_students) 
    {
        printf("No data found in class %d. Please enter a different class.\n", class_no);
        return 0;
    }

   temp_teacher = head_teacher;
   while (temp_teacher != NULL)
   {
      if(temp_teacher->teacher_std == class_no)
      {
         temp_teacher->teacher_std = 0;
      }
      temp_teacher = temp_teacher->next;
   }
   

   temp_teacher = head_teacher;
   while (temp_teacher!= NULL)
   {
      if(temp_teacher->s_no == roll)
      {
         break;
      }
      temp_teacher = temp_teacher->next;
   }
   
    temp_student = head_student;
    while (temp_student != NULL) 
    {
        if (temp_student->grade.class == class_no)
        {
            strcpy(name, temp_teacher->teacher_name);
            strcpy(temp_student->teacher_for_grade, temp_teacher->teacher_name);
            temp_teacher->teacher_std =class_no;
        }
        temp_student = temp_student->next;
    }


   temp_teacher = head_teacher;
   char query[256];
   while(temp_teacher != NULL)
   {

      
       snprintf(query, sizeof(query), "UPDATE teacher SET teacher_class=%d WHERE teacher_name='%s'", temp_teacher->teacher_std , temp_teacher->teacher_name);
      PGresult *res = PQexec(conn, query);
      if (PQresultStatus(res) != PGRES_COMMAND_OK) 
      {
          printf("Error while executing the query: %s\n", PQerrorMessage(conn));
          PQclear(res);
          return 1;
      }
      PQclear(res);
      temp_teacher = temp_teacher->next;
   }
   printf("Teacher %s has been allocated to class %d.\n", name, class_no);

   struct student *temp_s;
   while (head_student != 0)
   {
      temp_s = head_student;
      head_student = head_student->next;
      free(temp_s);
   }
   head_student = NULL;
   read_student_data(conn);
   allocting();
   return 0;
}


int free_data()
{
   /*To free the dynimic memeory to prevent memeory leak*/
   struct student *temp_s;
   while (head_student != 0)
   {
      temp_s = head_student;
      head_student = head_student->next;
      free(temp_s);
   }
   struct teacher *temp_t;
   while (head_teacher != 0)
   {
      temp_t = head_teacher;

      head_teacher = head_teacher->next;
      free(temp_t);
   }
   head_teacher = NULL;
   head_student = NULL;
   return 0;
}

int pay_fees(PGconn *conn)
{
   int class_no, roll, fee, found = 0;
   char term, Name[20], name_check[20];
   

   printf("Enter the class no: ");
   while (1) 
   {
      if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
      {
         break;
      } else 
      {
         printf("Enter 1 to 12 numbers only\n");
         while ((term = getchar()) != '\n' && term != EOF);
      }
   }


   while (class_no < 1 || class_no > 12) 
   {
      printf("Invalid data. Enter between 1st to 12th: ");
      while (1) 
      {
         if (scanf("%d%c", &class_no, &term) == 2 && term == '\n') 
         {
            break;
         } else 
         {
            printf("Enter 1 to 12 numbers only\n");
            while ((term = getchar()) != '\n' && term != EOF);
         }
      }
   }


   printf("Enter the roll number: ");
   while (1) 
   {
      if (scanf("%d%c", &roll, &term) == 2 && term == '\n') 
      {
         break;
      } 
      else 
      {
         while ((term = getchar()) != '\n' && term != EOF);
      }
   }


   temp_student = head_student;
   while (temp_student != NULL)
   {
      if (temp_student->grade.class == class_no && temp_student->grade.roll.roll_no == roll)
      {
         strcpy(Name, temp_student->student_name);
         printf("Name: %s\n", Name);
         printf("Father name: %s\n", temp_student->par.father_name);
         printf("Father number: %s\n", temp_student->par.f_num);
         printf("Address: %s\n", temp_student->address);
         printf("Grade: %d\n", temp_student->grade.class);
         printf("Pending fees: %d\n", temp_student->fees);
         printf("Joining date: %s\n", temp_student->joining_date_s);
         printf("Teacher: %s\n", strlen(temp_student->teacher_for_grade) == 0 ? "Not assigned yet" : temp_student->teacher_for_grade);
         printf("\n");

         if (temp_student->fees == 0)
         {
            printf("No pending fees\n");
            return 0;
         }

         if (temp_student->fees <= 6000)
         {
            printf("You have to pay the whole amount: %d\n", temp_student->fees);
            do
            {
               printf("Enter the amount: ");
               while (1) 
               {
                  if (scanf("%d%c", &fee, &term) == 2 && term == '\n') 
                  {
                     break;
                  } 
                  else 
                  {
                     while ((term = getchar()) != '\n' && term != EOF);
                  }
               }
               if (fee != temp_student->fees)
               {
                   printf("Invalid amount. You must pay the entire amount: %d.\n", temp_student->fees);
               }
               else
               {
                   break;
               }
            } while (1);
         }
         else
         {
            do
            {
               printf("Out of %d, how much are you paying?\n", temp_student->fees);
               while (1) 
               {
                  if (scanf("%d%c", &fee, &term) == 2 && term == '\n') 
                  {
                     break;
                  } 
                  else 
                  {
                     while ((term = getchar()) != '\n' && term != EOF);
                  }
               }
               if (fee <= 0 || fee > temp_student->fees || fee < (temp_student->fees / 2))
               {
                   printf("Invalid amount. Please enter a value between %d and %d.\n", temp_student->fees/2, temp_student->fees);
               }
               else
               {
                   break;
               }
            } while (1);
         }


         temp_student->fees -= fee;
         if (temp_student->fees < 0) temp_student->fees = 0;
         printf(temp_student->fees == 0 ? "Fees are fully paid.\n" : "%d rs are remaining.\n", temp_student->fees);


         char query[256];
         snprintf(query, sizeof(query), "SELECT data FROM student WHERE class=%d AND data::jsonb @> '[{\"name\": \"%s\"}]'", class_no, Name);
         PGresult *res_d = PQexec(conn, query);
         if (PQresultStatus(res_d) != PGRES_TUPLES_OK) 
         {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res_d);
            return 1;
         }
         int xy=0,i;
        int rows = PQntuples(res_d);  
         for(i=0 ; i<=rows; i++)
         {
            const char *json_data = PQgetvalue(res_d, 0, i);  
            cJSON *json_array = cJSON_Parse(json_data);
            if (json_array == NULL)
            {
                printf("Error parsing JSON data.\n");
                PQclear(res_d);
                return 1;
            }
            cJSON *json_field;
            cJSON *item;
            cJSON_ArrayForEach(item, json_array)
            {
               json_field= cJSON_GetObjectItemCaseSensitive(item, "name");
               strcpy(name_check , json_field->valuestring);
               xy++;
               if(strcmp(Name , name_check)==0)
               {
                  PQclear(res_d);
                  cJSON_Delete(json_array);
                  goto front;
               }
            }
         } 
         
         front:

         xy = xy-1;
         snprintf(query, sizeof(query), 
                  "UPDATE student SET data = jsonb_set(data::jsonb, '{%d, unpaid_fees}', '%d'::jsonb) "
                  "WHERE data::jsonb @> '[{\"name\": \"%s\"}]' ", xy, temp_student->fees, Name);

         PGresult *res = PQexec(conn, query);
         if (PQresultStatus(res) != PGRES_COMMAND_OK) 
         {
             printf("Error while executing the update query: %s\n", PQerrorMessage(conn));
             PQclear(res);
             PQclear(res_d);
             return 1;
         } 

          PQclear(res);
         found = 1;
         
         return 0;
      }
      temp_student = temp_student->next;
   }  

   if (!found)
   {
      printf("No student found with class number %d and roll number %d.\n", class_no, roll);
   }

   return 0;
}
int pay_fees_leaving(int class_no, int roll,PGconn *conn)
{
   /*If the student if leaving the school he/she should pay the whole fees before leaving*/
   temp_student = head_student; 
   int fee;
   int found = 0;
   char term;
   while (temp_student != NULL)
      {
        if (temp_student->grade.class == class_no && temp_student->grade.roll.roll_no == roll)
         {
            printf("Name: %s\n", temp_student->student_name);
            printf("Father name: %s\n", temp_student->par.father_name);
            printf("Father number: %s\n", temp_student->par.f_num);
            printf("Address: %s\n", temp_student->address);
            printf("Grade: %d\n", temp_student->grade.class);
            printf("Pending fees: %d\n", temp_student->fees);
            printf("Joining date: %s\n", temp_student->joining_date_s);

            int len = strlen(temp_student->teacher_for_grade);
            if (len == 0)
            {
               printf("Teacher: Not assigned yet\n");
            }
            else
            {
               printf("Teacher: %s\n", temp_student->teacher_for_grade);
            }
            printf("\n");

            while (1)
            {
               printf("You have to pay the full amount: %d\n", temp_student->fees);
               printf("Enter the amount: ");
               while (1) 
               {
                  if (scanf("%d%c", &fee, &term) == 2 && term == '\n') 
                  {
                     break;
                  } 
                  else 
                  {
                     term = 0;
                     while (term != '\n' && term != EOF) 
                     {
                        term = getchar();
                     }
                  }
               }
               if (fee != temp_student->fees)
               {
                   printf("Invalid amount. You must pay the entire amount: %d.\n", temp_student->fees);
               }
               else
               {
                  temp_student->fees -= fee;
                  if (temp_student->fees <= 0)
                  {
                     temp_student->fees = 0;
                     printf("Fees are fully paid.\n");
                  }
                  else
                  {
                     printf("%d rs are remaining.\n", temp_student->fees);
                  }
                  found = 1;
                  break; 
               }
            }
            break;
         }
         temp_student = temp_student->next;
      }

    if (!found)
    {
        printf("No student found with class number %d and roll number %d.\n", class_no, roll);
    }
    return 0;
}

int manage_student_data(PGconn *conn)
{
   /*To accsess the search ,edit , remove , pay fees features*/
   char term;
   int choice;
   back:
   printf("Press\n1. To search data\n2. To edit data\n3. To remove\n4. Pay the fees\n0. To exit\n");
    while (1) 
    {
      printf("Enter: ");
        if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
        {
            break;
        } else 
        {
            printf("Error, Enter a valid input\n");
            term = 0;
            while (term != '\n' && term != EOF) 
            {
                term = getchar();
            }
        }
    }
   switch (choice) 
   {
      case 1:
         search_student_data();
         break;
      case 2:
         edit_student_data(conn);
         break;
      case 3:
         remove_student_data(conn);
         break;
      case 4:
         pay_fees(conn);
         break;
      case 0:
         printf("Thank you\n");

         break;
      default:
         printf("Error, Enter a valid input\n\n");
         goto back;
   }
}
int manage_teacher_data(PGconn *conn)
{
   /*To access search , edit , remove , allocte teacher to class feature*/
   char term;
   int choice;
   back:
   printf("Press\n1. To search data\n2. To edit data\n3. To remove\n4. To allocate teacher to class\n0. To exit\n");
    while (1) 
    {
      printf("Enter: ");
        if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
        {
            break;
        } else 
        {
            printf("Error, Enter a valid input\n");
            term = 0;
            while (term != '\n' && term != EOF) 
            {
                term = getchar();
            }
        }
    }
   switch (choice) 
   {
      case 1:
         search_teacher_data();
         break;
      case 2:
         edit_teacher_data(conn);
         break;
      case 3:
         remove_teacher_data(conn);
         break;
      case 4:
         allocating_teacher(conn);
         break;
      case 0:
         printf("Thank you\n");

         break;
      default:
         printf("Error, Enter a valid input\n\n");
         goto back;
   }
   printf("-------------------------------------------------\n");
}
int manage_password(PGconn *conn)
{
   /*To change , add new password and to delete pass */
   int choice;
   char term;
   back:
   while (1) 
   {
      printf("1) To change pass\n2) To Add pass\n3) To delete pass\n0) Exit\n");
      while (1) 
      {
         printf("Enter: ");
         if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
         {
            break;
         } 
         else 
         {
            printf("Error, Enter a valid input\n");
            term = 0;
            while (term != '\n' && term != EOF) 
            {
               term = getchar();
            }
         }
      }
      switch (choice) 
      {

         case 1:
             change_pass(conn);
             break;
         case 2:
             pass_add(conn);
             break;
         case 3:
             delete_pass(conn);
             break;
         case 0:
             return 0;
         default:
             printf("Invalid choice.\n\n");
             goto back;
      }
   }
}


int present_teacher(char *name)
{
   /*If the teacher is already removed it deallocated from the particular class*/
   int found = 0;
   temp_teacher = head_teacher;
   while ( temp_teacher !=0)
   {
      if(strcmp(name , temp_teacher->teacher_name) == 0)
      {
         found = 1;
      }
      temp_teacher = temp_teacher ->next;
   }
   if(!found)
   {
      strcpy(name , "\0");
   }
}

int allocting()
{
/*If the teacher is allocated they remains allcated*/
   int class_no;
   temp_teacher = head_teacher;
   while (temp_teacher != 0)
   {

      if(temp_teacher->teacher_std != 0)
      {
         class_no = temp_teacher->teacher_std;

         temp_student = head_student;
         while (temp_student != NULL)
         {
            if(temp_student->grade.class == class_no)
            {
              strcpy(temp_student->teacher_for_grade, temp_teacher->teacher_name);


            }
            temp_student = temp_student->next;
         }
      }
      temp_teacher = temp_teacher->next;
   }
}

char* choose_subject(PGconn *conn)
{
   /*While hiring the teacher they can choose the subject to teacher and there is limit for all subjects*/
   int choice;
   char term;
   static char ch[4];
   static char subject[10];
   int num;
   printf("\t\t\t\t\tRequirements\n");
   printf("Press\n");
   printf("1) English \n2) Hindi \n3) Kannada \n4) Physics\n5) Maths\n6) Chemistry\n7) Biology\n8) Computer science\n9) Physical education\n10) Art\n");

   back:
   while (1) 
   {

     printf("Enter: ");
      if (scanf("%d%c", &choice, &term) == 2 && term == '\n') 
      {

         break;

      } else 
      {
         printf("Error, Enter a valid input\n");
         term = 0;
         while (term != '\n' && term != EOF) 
         {
            term = getchar();
         }
      }
   } 

   switch (choice)
   {
   case 1:
      strcpy(subject , "english");
      num = vacancy(subject,conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 2:
      strcpy(subject , "hindi");
      num = vacancy(subject,conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;
   
   case 3:
      strcpy(subject , "kannada");
      num = vacancy(subject, conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 4:
      strcpy(subject , "physics");
      num = vacancy(subject, conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 5:
      strcpy(subject , "maths");
      num = vacancy(subject, conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 6:
      strcpy(subject , "chemistry");
      num = vacancy(subject, conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;
   
   case 7:
      strcpy(subject , "biology");
      num = vacancy(subject, conn);
      if(num == 5)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 5)
      {
         num = 5 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 8:
      strcpy(subject , "cs");
      num = vacancy(subject, conn);
      if(num == 3)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 3)
      {
         num = 3 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 9:
      strcpy(subject , "pe");
      num = vacancy(subject, conn);
      if(num == 2)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 2)
      {
         num = 2 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   case 10:
      strcpy(subject , "art");
      num = vacancy(subject, conn);
      if(num == 2)
      {
         printf("No vacancy\n");
         return "no";
      }
      if(num < 2)
      {
         num = 2 - num;
         printf("Vacancy of %d\n", num);
      }
      break;

   default:
      printf("Error, enter a valid number\n");
      goto back;
   }
   printf("Are you interested?(Yes/No)\n");
   while (1) 
   {
      scanf("%s", ch);
      if (strcasecmp(ch, "yes") == 0 || strcasecmp(ch, "no") == 0) 
      {
          break;  
      } else 
      {
         printf("Invalid input. Please enter 'yes' or 'no': ");
      }
   }
   if(strcasecmp(ch , "no") == 0)
   {
      return ch;
   }
   return subject;
}

int vacancy(char sub[],PGconn *conn)
{
   /*It checks the vacancy of the teacher */
   int i = 0;
   if(head_teacher == NULL)
   {
      read_teacher_data(conn);
      change_teacher = 1; 
   }
   temp_teacher = head_teacher;
     while (temp_teacher != 0)
   {
      temp_teacher = temp_teacher ->next;
   }
      temp_teacher = head_teacher;
   while (temp_teacher != 0)
   {
      if(strcmp(temp_teacher->subject_teacher , sub) == 0)
      {
         i++;
      }
      temp_teacher = temp_teacher ->next;
   }

   return i;
   
}

int log_file(char error[])
{
   FILE *fp = NULL;
   char date[15];
   fp = fopen("logfile.txt", "r");
   if(fp == NULL)
   {
      fp = fopen("logfile.txt", "w");
   }

   fp = fopen("logfile.txt", "a");
      time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   strftime(date, sizeof(date), "%d/%m/%Y", tm);
   
   fprintf(fp, "%s\n%s",date, error);                                                                                                

   fclose(fp);
}
#include<stdio.h>
#include<stdlib.h>
#include "library.h"
#include<libpq-fe.h>
struct student *head_student  , *temp_student;
struct teacher *head_teacher , *temp_teacher;
const char *conninfo = "dbname=school_data user=postgres password=docketrun host=localhost port=5432";
int change_student  , change_teacher;
int main() 
{
   PGconn *conn = PQconnectdb(conninfo);
   if (PQstatus(conn) != CONNECTION_OK) 
   {
      printf("Connection to database failed: %s\n", PQerrorMessage(conn));
      return 1;
   }
   new_table_db(conn);
   int a;
   int choice = 1;
   char term;
   next:
   while (1) 
   {
      
      back:
      printf("\n-------------------------------------------------\n");
      printf("Press\n1. Admission and hiring\n2. Data management\n0. Exit\n");
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
            printf("\n-------------------------------------------------\n");
            
            printf("Press\n1. To student admission\n2. To teacher hiring\n0. To exit\n");
            int sub_choice1;
            while (1) 
            {
               printf("Enter: ");
               if (scanf("%d%c", &sub_choice1, &term) == 2 && term == '\n') 
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
            switch (sub_choice1) 
            {
               case 1:
                  student_admission(conn);
                  break;
               case 2:
                  teacher_hiring(conn);
                  break;
               case 0:
                  printf("Thank you\n");
                  break;
               default:
                  printf("Error, Enter a valid input\n");
                  break;
            }
            break;
         case 2:
            
            printf("\n-------------------------------------------------\n");
            
            a = password(conn);

            if(a == 0)
            {
               goto back;
            }

            printf("Opening the data management\n");
            if (head_student == 0) 
            {  
               int ab;
               ab = read_student_data(conn);
               if(ab == 2)
               {
                  goto next;
               }
            }
            if(head_student != 0 && change_student == 1)
            { 
               struct student *temp;
               while (head_student != 0)
               {
                  temp = head_student;
                  head_student = head_student->next;
                  free(temp);
               }
               read_student_data(conn);
            }
            if (head_teacher == 0) 
            {
               int ab;
               ab = read_teacher_data(conn);
               if(ab == 2)
               {
                  goto next;
               }
            }
            if(head_teacher != 0 && change_teacher == 1)
            {
               struct teacher *temp;
               while (head_teacher != 0)
               {
                  temp = head_teacher;
                  head_teacher = head_teacher->next;
                  free(temp);
               }
               read_teacher_data(conn);
            }
            allocting();
            int choice2 = 1;
            while (choice2) 
            {
               printf("\n-------------------------------------------------\n");
               printf("Press\n1. To manage student data\n2. To manage teacher data\n3. To manage password\n0. To exit\n");

               int sub_choice2;
            while (1) 
            {            
               printf("Enter: ");      
               if (scanf("%d%c", &sub_choice2, &term) == 2 && term == '\n') 
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
               printf("\n-------------------------------------------------\n");
               switch (sub_choice2) 
               {
                  case 1:
                     manage_student_data(conn);
                     break;
                        
                  case 2:
                     manage_teacher_data(conn);
                     break;
                  case 3:
                     manage_password(conn);
                     break;
                  case 0:
                     printf("Thank you\n");
                     free_data();
                     choice2 = 0;
                     break;
                  default:
                     printf("Error, Enter a valid input\n");
                     break;
               }
            }
            break;
         case 0:
            printf("Thank you\n");
            PQfinish(conn);
            free_data();
            exit(0);
         default:
            printf("Error, Enter a valid input\n");
            break;
      }
   }
}


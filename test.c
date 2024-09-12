#include<stdio.h>
#include<stdlib.h>
#include "library.h"
struct student *head_student  , *temp_student;
struct teacher *head_teacher , *temp_teacher;

int change_student  , change_teacher;
int main() 
{
   
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
                  student_admission();
                  break;
               case 2:
                  teacher_hiring();
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
            printf("To enter data management, enter the password\n");
            printf("Enter: ");
            a = password();
            printf("%d\n",a);
            if(a == 0)
            {
               goto back;
            }

            printf("Opening the data management\n");
            if (head_student == 0) 
            {  
               int ab;
               ab = read_student_data();
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
               read_student_data();
            }
            if (head_teacher == 0) 
            {
               int ab;
               ab = read_teacher_data();
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
               read_teacher_data();
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
                     manage_student_data();
                     break;
                        
                  case 2:
                     manage_teacher_data();
                     break;
                  case 3:
                     manage_password();
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
            free_data();
            exit(0);
         default:
            printf("Error, Enter a valid input\n");
            break;
      }
   }
}
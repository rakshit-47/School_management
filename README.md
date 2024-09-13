### School management program

int time_check()
/*It returns boolean number 
   Used as timer in exam it will close the exam after certein time
   if time's up it returns 0 and ends the exam
   and if not it returns 1 it continues the exam till time's up */


int valid_address(const char *address)
/*It returns boolean number
    It checks if the typed address by user is valid address or not
    if input is special charater or number it returns 0 
    and if everything is alright it returns 1 */

int valid_pin_code(const char *pin)
/*It returns boolean number
    It checks if the typed pin code is valid or not 
    the pincode should be exact 6 numbers and only numbers*/

int valid_name(const char *name)
/*It returns boolean number
    This checks if the typed name is valid or not 
    the name should not contain numbers or special charaters*/

int is_valid_num(char *num)
/*It returns boolean number
    This verifies if the number typed has exact 10 numbers and weather it is only number*/

int valid_religion(char str[])
/*It returns boolean number
    There are some predefined valid reigions in the program if the user dont enter any of them it will considered as error 
    both small and capital case is valid*/

int valid_birth(char birth[])
/*It returns boolean number
    This checks weather the user typed the birthday in right format   which is DD/MM/YYYY and also it does some error handling like user can't enter 30day on feb month */

int student_admission()
/*It returns boolean number
    This function displays the information of the school and takes some information from user like class and display fees as per as class*/

int exam_student(int class, int fees)
/*It returns boolean number
    This functions reads questions from file and ask the user to type the answer (NOTE: If the student is below class 5Th he/she don't have to attent exam they can directly enter the data)
    the student in 5TH or above 5th have to attent exam if they get below 70% they cant take admission*/
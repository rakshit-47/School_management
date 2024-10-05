#include<libpq-fe.h>
struct parents
{
   char father_name[20];
   char mother_name[20];
   char f_num[11];
   char m_num[11];
};



struct teacher
{
   int teacher_age;
   char teacher_name[20];
   char teacher_num[11];
   char joining_date_t[20];
   char teacher_address[60];
   char teacher_birthdate[10];
   int teacher_std;
   int s_no;
   char subject_teacher[10];
   struct teacher *next;
};
struct Roll
{
   int roll_no;
};
struct std
{
   int class;
   struct Roll roll;
};

struct student
{
   char blood_group[4];
   char religion[11];
   char student_name[25];
   char address[60];
   int fees;
   char joining_date_s[20];
   char birth[10];
   char teacher_for_grade[30];
   struct student *next;
   struct std grade;
   struct parents par;
};

int new_table_db(PGconn *conn);

int time_check();

int valid_name(const char *name);

int is_valid_num(char *num);

int valid_address(const char *address);

int valid_pin_code(const char *pin); 

int student_admission(PGconn *conn);

int exam_student(int class, int fees,PGconn *conn);

int write_student_data(int class, int fee,PGconn *conn);

int teacher_hiring(PGconn *conn);

int write_teacher_data(int age, char subject[],PGconn *conn);

int password(PGconn *conn);

int change_pass(PGconn *conn);

int read_student_data(PGconn *conn);

int read_teacher_data(PGconn *conn);

int assign_rollno(int choice);

int search_student_data();

int edit_student_data(PGconn *conn);

int search_teacher_data();

int edit_teacher_data(PGconn *conn);

int remove_student_data(PGconn *conn);

int remove_teacher_data(PGconn *conn);

int allocating_teacher(PGconn *conn);

int free_data();

int manage_student_data(PGconn *conn);

int manage_teacher_data(PGconn *conn);

int pay_fees(PGconn *conn);

int pay_fees_leaving(int class_no , int roll,PGconn *conn);

int valid_birth(char birth[]);

int valid_religion(char str[]);

int present_teacher(char *name);

int allocting();

char* choose_subject(PGconn *conn);

int vacancy(char sub[],PGconn *conn);

int pass_add(PGconn *conn);

int delete_pass(PGconn *conn);

int manage_password(PGconn *conn);

int log_file(char error[]);


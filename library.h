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

int new_table_db();

int time_check();

int valid_name(const char *name);

int is_valid_num(char *num);

int valid_address(const char *address);

int valid_pin_code(const char *pin); 

int student_admission();

int exam_student(int class, int fees);

int write_student_data(int class, int fee);

int teacher_hiring();

int write_teacher_data(int age, char subject[]);

int password();

int change_pass();

int read_student_data();

int read_teacher_data();

int assign_rollno(int choice);

int search_student_data();

int edit_student_data();

int search_teacher_data();

int edit_teacher_data();

int remove_student_data();

int remove_teacher_data();

int allocating_teacher();

int free_data();

int manage_student_data();

int manage_teacher_data();

int pay_fees();

int pay_fees_leaving(int class_no , int roll);

int valid_birth(char birth[]);

int valid_religion(char str[]);

int present_teacher(char *name);

int allocting();

char* choose_subject();

int vacancy(char sub[]);

int pass_add();

int delete_pass();

int manage_password();
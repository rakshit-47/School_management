## This School Management System

### Database Information
```
Database Name: school_data
Host: localhost
Port: 5432
```

### Function Descriptions


### Database function

**int new_table_db(PGconn \*conn)**  
Returns int: It creates new tables in the database if the tables are not present.

### Time Management

**int time_check()**  
Returns int: Manages exam timing. Returns 0 to end the exam when time's up, and 1 to continue if time is still remaining.

### Validation Functions

**int valid_address(const char \*address)**  
Returns int: Validates a user-provided address. Returns 0 for invalid input (special characters or numbers) and 1 for valid addresses.

**int valid_pin_code(const char \*pin)**  
Returns int: Validates a PIN code. Should be exactly 6 digits.

**int valid_name(const char \*name)**  
Returns int: Checks if a name is valid. The name should not contain numbers or special characters.

**int is_valid_num(char \*num)**  
Returns int: Verifies if the number is exactly 10 digits long and contains only digits.

**int valid_religion(char str[])**  
Returns int: Checks if the religion entered is among predefined valid options, case insensitive.

**int valid_birth(char birth[])**  
Returns int: Validates the date of birth in DD/MM/YYYY format, including error handling for invalid dates like February 30th.


### Student Management

**int student_admission()**  
Returns int: Displays school information, gathers student details, and calculates fees based on the class.

**int exam_student(int class, int fees)**  
Returns int: Administers exams for students in class 5 and above. Students scoring below 70% cannot take admission.

**int write_student_data(int class, int fee)**  
Returns int: Collects and stores student data with error handling.

**int read_student_data()**  
Returns int: Reads all student data from the database into dynamic memory.

**int assign_rollno(int choice)**  
Returns int: Assigns roll numbers to students or teacher codes based on the choice parameter.

**int search_student_data()**  
Returns int: Searches for student data by roll number, class, first letter of name, or pending fees.

**int edit_student_data()**  
Returns int: Edits student data and updates the database.

**int remove_student_data()**  
Returns int: Removes a student from the database.

**int pay_fees()**  
Returns int: Handles fee payments. Students pay full if fees are 6000 or below; otherwise, they pay half.

**int pay_fees_leaving(int class_no, int roll)**  
Returns int: Manages fee payment for students leaving the school.

**int manage_student_data()**  
Returns int: Provides access to search, edit, remove, and fee payment functionalities.


### Teacher Management

**int teacher_hiring()**  
Returns int: Administers exams for prospective teachers and calculates their marks.

**int write_teacher_data()**  
Returns int: Collects and stores teacher data with error handling.

**int read_teacher_data()**  
Returns int: Reads all teacher data from the database into dynamic memory.

**int search_teacher_data()**  
Returns int: Searches for teacher data by teacher code.

**int edit_teacher_data()**  
Returns int: Edits teacher data and updates the database.

**int remove_teacher_data()**  
Returns int: Removes a teacher from the database.

**int allocating_teacher()**  
Returns int: Allocates teachers to classes and updates the database.

**char\* choose_subject()**  
Returns char*: Allows teachers to select subjects they wish to teach.

**int vacancy(char sub[])**  
Returns int: Checks the availability of teachers for a given subject.

### Password Management

**int password()**  
Returns int: Allows users to enter or create a new password if none exists.

**int change_pass()**  
Returns int: Changes an existing password and updates it in the database.

**int pass_add()**  
Returns int: Adds a new password to the system with a maximum of 5 passwords.

**int delete_pass()**  
Returns int: Deletes a specific password and updates the database.

**int manage_password()**  
Returns int: Provides functionalities to change, add, or delete passwords.

### Memory Management

**int free_data()**  
Returns int: Frees dynamically allocated memory to prevent memory leaks.

### Teacher Presence

**int present_teacher(char \*name)**  
Returns int: Manages the presence of teachers, including deallocating removed teachers from their classes.

**int allocting()**  
Returns int: Manages the allocation status of teachers.

**int log_file(char error[])**
returns int: logs all the error in the file (Note if the log don't exist it creates and once it creates it appentes )
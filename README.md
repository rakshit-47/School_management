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
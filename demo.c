#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>

int main() 
{
    char pass[12];
    const char *conninfo = "dbname=school_data user=postgres password=docketrun host=localhost port=5432";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) 
    {
        printf("Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    } 


    PGresult *res = PQexec(conn, "SELECT * FROM password");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) 
    {
        printf("SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    printf("\n------------------------------------------------------------------------------------------------------------\n");
    char *value;

   value = PQgetvalue(res, 0, 0);
   if(value == NULL)
   {
      printf("4");
      while (1) 
      {
         printf("Enter a new password: ");
         scanf("%s", pass);
         int len = strlen(pass);
         if (len >= 8 && len <= 12) 
         {
            printf("www");
            char query[50];
            snprintf(query, sizeof(query),"insert into password values('rakshitt')");
            if (PQresultStatus(res) != PGRES_TUPLES_OK) 
            {
               printf("The Password is already exist enter different\n");
               PQclear(res);
               PQfinish(conn);
               return 1;
            }
            printf("eee");
            return 1;
         } 
         else 
         {
            printf("The password should be between 8 and 12 characters\n");
         }
      }
   } 
   
    PQclear(res);
    PQfinish(conn);
    //PQfreemem(res);
    //PQfreemem(conn);
    
    return 1;
}

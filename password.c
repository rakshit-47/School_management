#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include<ctype.h>
#include<stdlib.h>
const char *conninfo = "dbname=school_data user=postgres password=docketrun host=localhost port=5432";
int valid_name(const char *name)
{
      /*This checks if the typed name is valid or not 
   the name should not contain numbers or special charaters*/
   int len;
   len = strlen(name);
   printf("%d\n", len);
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
int password() 
{
    char term;
   char name[20];
   char pass[13];
   char pass2[13];
   int attempts;
   PGconn *conn = PQconnectdb(conninfo);
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
            break;
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
        printf("Query Status: %s\n", PQresStatus(resStatus));
        if (resStatus != PGRES_TUPLES_OK) 
        {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn); 
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
                    PQfinish(conn);
                    return 0;
                } 
                else 
                {
                    printf("Incorrect password. Try again.\n");
                }
            }
            printf("Too many incorrect attempts.\n");
            printf("1 or 0\n");
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
                PQfinish(conn);
                return 0;
            }
        }
    }

    PQclear(res_d);
    PQfinish(conn);
    return 0;
}

int change_pass() 
{
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) 
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    char pass[13], pass2[13];
    while (1) 
    {
        printf("Enter the password you want to change: ");
        scanf("%12s", pass);

        const char *query = "SELECT * FROM password";
        PGresult *res = PQexec(conn, query);
        ExecStatusType resStatus = PQresultStatus(res);
        printf("Query Status: %s\n", PQresStatus(resStatus));
        if (resStatus != PGRES_TUPLES_OK) 
        {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn); 
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
            printf("This password does not exist. Enter again: ");
            continue;
        }

        printf("Enter the new password: ");
        scanf("%12s", pass2);

        char update_query[150];
        snprintf(update_query, sizeof(update_query), "UPDATE password SET pass='%s' WHERE pass='%s'", pass2, pass);
        PGresult *res_update = PQexec(conn, update_query);
        ExecStatusType resStatus_update = PQresultStatus(res_update);
        printf("Query Status: %s\n", PQresStatus(resStatus_update));    
        if (resStatus_update != PGRES_COMMAND_OK) 
        {
            printf("Error while executing the query: %s\n", PQerrorMessage(conn));
            PQclear(res_update);
            PQfinish(conn);
            return 1;
        }

        printf("Password changed successfully.\n");
        PQclear(res_update);
        PQfinish(conn);
        return 0;
    }
}

int pass_add() 
{
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) 
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    const char *query = "SELECT * FROM password";
    PGresult *res = PQexec(conn, query);
    ExecStatusType resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));
    if (resStatus != PGRES_TUPLES_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn); 
        return 1;
    }

    int rows = PQntuples(res);
    PQclear(res);

    if (rows >= 5) 
    {
        printf("Cannot add more than 5 passwords.\n");
        PQfinish(conn);
        return 1;
    }

    char pass[13];
    while (1) 
    {
        printf("Enter a new password (8-12 characters): ");
        scanf("%12s", pass);
        int len = strlen(pass);
        if (len >= 8 && len <= 12) 
        {
            char insert_query[100];
            snprintf(insert_query, sizeof(insert_query), "INSERT INTO password VALUES('%s')", pass);
            PGresult *res = PQexec(conn, insert_query);
            ExecStatusType resStatus = PQresultStatus(res);
            printf("Query Status: %s\n", PQresStatus(resStatus));       
            if (resStatus != PGRES_COMMAND_OK) 
            {
                printf("Error while executing the query: %s\n", PQerrorMessage(conn));
                PQclear(res);
                PQfinish(conn);
                return 1;
            }
            printf("Password added successfully.\n");
            PQclear(res);
            PQfinish(conn);
            return 0;
        } 
        else 
        {
            printf("The password should be between 8 and 12 characters.\n");
        }
    }
}

int delete_pass()
{
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) 
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    char pass[13];
    printf("Enter the password which you want to delete: ");
    scanf("%12s", pass);

    const char *query = "SELECT * FROM password";
    PGresult *res = PQexec(conn, query);
    ExecStatusType resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));
    if (resStatus != PGRES_TUPLES_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn); 
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
        PQfinish(conn);
        return 1;
    }
    char delete_query[50];
    snprintf(delete_query, sizeof(delete_query), "DELETE FROM password WHERE pass = '%s'", pass);
    PGresult *res_delete = PQexec(conn, delete_query);
    if (PQresultStatus(res_delete) != PGRES_COMMAND_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res_delete);
        PQfinish(conn);
        return 1;  
    }
    printf("Password deleted successfully.\n");
    PQclear(res_delete);
    PQfinish(conn);
    return 0;
}

int main() 
{
    int choice;
    char term;
    while (1) 
    {
        printf("1) To open\n2) To change pass\n3) To Add pass\n4) To delete pass\n5) Exit\n");
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
                password();
                break;
            case 2:
                change_pass();
                break;
            case 3:
                pass_add();
                break;
            case 4:
                delete_pass();
                break;
            case 5:
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}

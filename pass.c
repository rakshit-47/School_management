#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>

const char *conninfo = "dbname=school_data user=postgres password=docketrun host=localhost port=5432";
int main()
{
    char pass[20];
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) 
    {
        printf("Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    } 

    printf("Enter a new password: ");
    scanf("%s", pass);
    char query[100];
    printf("The pass: %s\n", pass);
    snprintf(query, sizeof(query),"INSERT INTO password (pass) VALUES ('%s')", pass);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 0;
    }

    printf("Query Executed Successfully\n");
    
}











#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>

const char *conninfo = "dbname=school_data user=postgres password=docketrun host=localhost port=5432";

int execute_query(PGconn *conn, const char *query, PGresult **res) {
    *res = PQexec(conn, query);
    if (PQresultStatus(*res) != PGRES_COMMAND_OK && PQresultStatus(*res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s", PQerrorMessage(conn));
        PQclear(*res);
        return 1;
    }
    return 0;
}

int password() 
{
   char term;
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    PGresult *res;
    const char *query = "SELECT * FROM password";
    if (execute_query(conn, query, &res)) {
        PQfinish(conn);
        return 1;
    }

    char *value = PQgetvalue(res, 0, 0);
    if (value == NULL) {
        char pass[13];
        while (1) {
            printf("Enter a new password (8-12 characters): ");
            scanf("%12s", pass);
            int len = strlen(pass);
            if (len >= 8 && len <= 12) {
                char query[100];
                snprintf(query, sizeof(query), "INSERT INTO password VALUES('%s')", pass);
                if (execute_query(conn, query, &res)) {
                    fprintf(stderr, "Error while executing the query: %s\n", PQerrorMessage(conn));
                    PQfinish(conn);
                    return 1;
                }
                printf("Password added successfully.\n");
                PQclear(res);
                PQfinish(conn);
                return 0;
            } else {
                printf("The password should be between 8 and 12 characters.\n");
            }
        }
    } else {
        char pass2[13];
        int attempts;
        while (1) {
            for (attempts = 0; attempts < 3; attempts++) {
                printf("Enter the password: ");
                scanf("%12s", pass2);
                int rows = PQntuples(res);
                int found = 0;
                for (int i = 0; i < rows; i++) {
                    char password[13];
                    strncpy(password, PQgetvalue(res, i, 0), sizeof(password) - 1);
                    password[sizeof(password) - 1] = '\0';
                    if (strcmp(password, pass2) == 0) {
                        found = 1;
                        break;
                    }
                }
                if (found) {
                    printf("Access granted.\n");
                    PQclear(res);
                    PQfinish(conn);
                    return 0;
                } else {
                    printf("Incorrect password. Try again.\n");
                }
            }
            printf("Too many incorrect attempts.\n");
            int choice;
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
            if (choice == 0) {
                PQclear(res);
                PQfinish(conn);
                return 0;
            }
        }
    }

    PQclear(res);
    PQfinish(conn);
    return 0;
}

int change_pass() {
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    char pass[13], pass2[13];
    while (1) {
        printf("Enter the password you want to change: ");
        scanf("%12s", pass);

        PGresult *res;
        const char *query = "SELECT * FROM password";
        if (execute_query(conn, query, &res)) {
            PQfinish(conn);
            return 1;
        }

        int rows = PQntuples(res);
        int exists = 0;
        for (int i = 0; i < rows; i++) {
            char stored_pass[13];
            strncpy(stored_pass, PQgetvalue(res, i, 0), sizeof(stored_pass) - 1);
            stored_pass[sizeof(stored_pass) - 1] = '\0';
            if (strcmp(stored_pass, pass) == 0) {
                exists = 1;
                break;
            }
        }
        PQclear(res);

        if (!exists) {
            printf("This password does not exist. Enter again: ");
            continue;
        }

        printf("Enter the new password: ");
        scanf("%12s", pass2);

        char update_query[150];
        snprintf(update_query, sizeof(update_query), "UPDATE password SET pass='%s' WHERE pass='%s'", pass2, pass);
        if (execute_query(conn, update_query, &res)) {
            PQfinish(conn);
            return 1;
        }

        printf("Password changed successfully.\n");
        PQfinish(conn);
        return 0;
    }
}

int pass_add() {
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    PGresult *res;
    const char *query = "SELECT * FROM password";
    if (execute_query(conn, query, &res)) {
        PQfinish(conn);
        return 1;
    }

    int rows = PQntuples(res);
    PQclear(res);

    if (rows >= 5) {
        printf("Cannot add more than 5 passwords.\n");
        PQfinish(conn);
        return 1;
    }

    char pass[13];
    while (1) {
        printf("Enter a new password (8-12 characters): ");
        scanf("%12s", pass);
        int len = strlen(pass);
        if (len >= 8 && len <= 12) {
            char insert_query[100];
            snprintf(insert_query, sizeof(insert_query), "INSERT INTO password VALUES('%s')", pass);
            if (execute_query(conn, insert_query, &res)) {
                fprintf(stderr, "Error while executing the query: %s\n", PQerrorMessage(conn));
                PQfinish(conn);
                return 1;
            }
            printf("Password added successfully.\n");
            PQfinish(conn);
            return 0;
        } else {
            printf("The password should be between 8 and 12 characters.\n");
        }
    }
}

int main() {
    int choice;
    char term;
    while (1) {
        printf("1) To open\n2) To change pass\n3) To Add pass\n4) To delete pass\n");
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

        switch (choice) {
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
                printf("Functionality to delete passwords is not yet implemented.\n");
                break;
            default:
                printf("Invalid choice.\n");

        }
    }
    return 0;
}

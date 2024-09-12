#include<stdio.h>
#include<stdlib.h>
#include<libpq-fe.h>
#include<string.h>

char *conninfo = "dbname=test user=postgres password=docketrun host=localhost port=5432";

int displaydb()
{
    PGconn *conn = PQconnectdb(conninfo);
    char *query = "select * from students order by name";
    PGresult *res = PQexec(conn , query);
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
    int col = PQnfields(res);

    printf("\n------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < col; j++) 
        {
            char name[10];
            strcpy(name, PQgetvalue(res, i, j));
            printf("%s\t", name);

        }
        printf("\n");

    }

    PQclear(res);
    PQfinish(conn);
    return 0;
}

int updatedb()
{
    int rollno;
    char name[10];
    char blood[4];
    PGconn *conn = PQconnectdb(conninfo);
    printf("Enter the roll no of the student\n");
    scanf("%d", &rollno);
    printf("Enter the new name\n");
    scanf("%s", name);
    printf("Enter the blood group\n");  
    scanf("%s", blood);
    char query[100];
    snprintf(query, sizeof(query) , "UPDATE students SET name='%s' , blood='%s' where rollno=%d", name ,blood, rollno);

    PGresult *res = PQexec(conn, query);
    ExecStatusType resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));

    if (PQresultStatus(res) != PGRES_COMMAND_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    printf("Query Executed Successfully\n");

    PQclear(res);
    PQfinish(conn);

    return 0;
}

int writedb()
{
    int roll;
    char name[100];
    char blood[10];
    PGconn *conn = PQconnectdb(conninfo);
    printf("Enter the rollno: ");
    scanf("%d", &roll);

    printf("Enter name: ");
    scanf("%s", name);
    printf("Blood group: ");
    scanf("%s", blood);


    char query[100];
    snprintf(query, sizeof(query),"INSERT INTO students (name, rollno, blood) VALUES ('%s', %d, '%s')",name, roll, blood);
    

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 0;
    }

    printf("Query Executed Successfully\n");

    PQclear(res);
    PQfinish(conn);

    return 0;
}


int delete_datadb()
{
    int roll;
    char query[256];  


    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) 
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;  
    }

    printf("Enter the rollno: ");
    scanf("%d", &roll);


    snprintf(query, sizeof(query), "DELETE FROM students WHERE rollno = %d", roll);

  
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) 
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 1;  
    }

    printf("Query Executed Successfully\n");

    PQclear(res);
    PQfinish(conn);

    return 0; 
}
int main()
{
    char term;
    int choice;
    while (1) 
    {
        printf("Press\n1.) To display \n2.) Update data \n3.) To enter data\n4.) To delete\n0.) To exit\n");
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
                displaydb();
                break;

            case 2:
                updatedb();
                break;

            case 3:
                writedb();
                break;

            case 4:
                delete_datadb();
                break;

            case 0:
                exit(0);
            
            default:
                printf("Enter a valid number\n");
        }
        printf("\n------------------------------------------------------------------------------------------------------------\n");
    }
}
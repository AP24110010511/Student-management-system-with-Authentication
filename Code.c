#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE      "student.txt"
#define CREDENTIALS_FILE  "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

int  loginSystem(void);
void mainMenu(void);
void adminMenu(void);
void staffMenu(void);
void guestMenu(void);

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main(void) {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}

int loginSystem(void) {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[10];

    printf("========= LOGIN SCREEN =========\n");
    printf("Username: ");
    scanf("%19s", username);
    printf("Password: ");
    scanf("%19s", password);

    FILE *fp = fopen(CREDENTIALS_FILE, "r");
    if (!fp) {
        printf("Error: %s not found!\n", CREDENTIALS_FILE);
        return 0;
    }

    while (fscanf(fp, "%19s %19s %9s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {

            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            return 1;      
        }
    }

    fclose(fp);
    return 0;              
}
void mainMenu(void) {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

void adminMenu(void) {
    int choice;
    do {
        printf("\n====== ADMIN MENU ======\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayStudents(); break;
            case 3: searchStudent();   break;
            case 4: updateStudent();   break;
            case 5: deleteStudent();   break;
            case 6: printf("Logging out...\n"); break;
            default: printf("Invalid Choice!\n");
        }
    } while (choice != 6);
}

void staffMenu(void) {
    printf("\n[STAFF MENU] (not implemented yet)\n");
}
void guestMenu(void) {
    printf("\n[GUEST MENU] (not implemented yet)\n");
}

void addStudent(void) {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll: ");
    scanf("%d", &st.roll);
    printf("Enter Name: ");
    scanf("%49s", st.name);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);
    printf("Student Added Successfully!\n");
}

void displayStudents(void) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("-----------------------------\n");

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}

void searchStudent(void) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;
    int roll, found = 0;

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll to search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("\nRecord Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n",
                   st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record not found.\n");

    fclose(fp);
}

void updateStudent(void) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file!\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter Roll to update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Enter new Name: ");
            scanf("%49s", st.name);
            printf("Enter new Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record updated.\n");
    else
        printf("Record not found.\n");
}

void deleteStudent(void) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file!\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter Roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue;   
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record deleted.\n");
    else
        printf("Record not found.\n");
}

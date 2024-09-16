#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50
#define PAGE_SIZE 5  // Define how many students to show per page

// Structure to hold student information
typedef struct {
    int roll_number;
    char name[NAME_LENGTH];
    int age;
    float marks;
} Student;

// Function prototypes
void menu();
Student* add_student(Student *students, int *count, int *capacity);
void display_students(Student *students, int count);
void paginate_students(Student *students, int count, int page_size);
void search_student(Student *students, int count, int search_by_roll);
void update_student(Student *students, int count);
void delete_student(Student *students, int *count);
void sort_students(Student *students, int count);
int binary_search(Student *students, int low, int high, int roll_number);
void save_to_file(Student *students, int count);
Student* load_from_file(Student *students, int *count, int *capacity);
void free_memory(Student *students);

// Display menu options
void menu() {
    printf("\n-------- Student Database Management System --------\n");
    printf("1. Add a new student\n");
    printf("2. Display all students (with pagination)\n");
    printf("3. Search for a student by roll number\n");
    printf("4. Search for a student by name\n");
    printf("5. Update a student\n");
    printf("6. Delete a student\n");
    printf("7. Sort students (by roll number, name, or marks)\n");
    printf("8. Save to file\n");
    printf("9. Load from file\n");
    printf("10. Exit\n");
    printf("Enter your choice: ");
}

// Input validation function
int validate_input(const char *prompt, int min, int max) {
    int value;
    do {
        printf("%s", prompt);
        scanf("%d", &value);
        if (value < min || value > max) {
            printf("Invalid input! Please enter a value between %d and %d.\n", min, max);
        }
    } while (value < min || value > max);
    return value;
}

// Add a new student with dynamic memory management
Student* add_student(Student *students, int *count, int *capacity) {
    if (*count == *capacity) {
        *capacity *= 2;
        students = (Student*) realloc(students, (*capacity) * sizeof(Student));
    }

    Student new_student;
    new_student.roll_number = validate_input("Enter a positive roll number: ", 1, 9999);
    
    printf("Enter name: ");
    getchar();
    fgets(new_student.name, NAME_LENGTH, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = '\0';

    new_student.age = validate_input("Enter age (5-30): ", 5, 30);
    new_student.marks = validate_input("Enter marks (0-100): ", 0, 100);

    students[(*count)++] = new_student;
    printf("Student added successfully!\n");
    
    return students;
}

// Display all students with pagination
void paginate_students(Student *students, int count, int page_size) {
    int total_pages = (count + page_size - 1) / page_size;
    int page = 0;

    while (page < total_pages) {
        int start = page * page_size;
        int end = start + page_size > count ? count : start + page_size;

        printf("\nDisplaying page %d/%d:\n", page + 1, total_pages);
        printf("Roll Number\tName\t\tAge\tMarks\n");
        printf("--------------------------------------------------\n");

        for (int i = start; i < end; i++) {
            printf("%d\t\t%s\t\t%d\t%.2f\n", students[i].roll_number, students[i].name, students[i].age, students[i].marks);
        }

        page++;
        if (page < total_pages) {
            printf("Press Enter to view the next page...\n");
            getchar();
            getchar();
        }
    }
}

// Search student by roll number or name
void search_student(Student *students, int count, int search_by_roll) {
    if (search_by_roll) {
        int roll_number = validate_input("Enter roll number to search: ", 1, 9999);
        int index = binary_search(students, 0, count - 1, roll_number);
        
        if (index != -1) {
            printf("Student found: %s, Age: %d, Marks: %.2f\n", students[index].name, students[index].age, students[index].marks);
        } else {
            printf("Student with roll number %d not found!\n", roll_number);
        }
    } else {
        char name[NAME_LENGTH];
        printf("Enter the name to search: ");
        getchar();
        fgets(name, NAME_LENGTH, stdin);
        name[strcspn(name, "\n")] = '\0';

        for (int i = 0; i < count; i++) {
            if (strcmp(students[i].name, name) == 0) {
                printf("Student found: Roll Number: %d, Age: %d, Marks: %.2f\n", 
                        students[i].roll_number, students[i].age, students[i].marks);
                return;
            }
        }
        printf("Student with name '%s' not found!\n", name);
    }
}

// Binary search for optimized search by roll number
int binary_search(Student *students, int low, int high, int roll_number) {
    while (low <= high) {
        int mid = (low + high) / 2;
        if (students[mid].roll_number == roll_number)
            return mid;
        if (students[mid].roll_number < roll_number)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Update student information
void update_student(Student *students, int count) {
    int roll_number = validate_input("Enter roll number to update: ", 1, 9999);

    int index = binary_search(students, 0, count - 1, roll_number);
    if (index != -1) {
        printf("Updating student: %s\n", students[index].name);
        printf("Enter new name: ");
        getchar();
        fgets(students[index].name, NAME_LENGTH, stdin);
        students[index].name[strcspn(students[index].name, "\n")] = '\0';
        students[index].age = validate_input("Enter new age: ", 5, 30);
        students[index].marks = validate_input("Enter new marks: ", 0, 100);
        printf("Student updated successfully!\n");
    } else {
        printf("Student with roll number %d not found!\n", roll_number);
    }
}

// Delete a student
void delete_student(Student *students, int *count) {
    int roll_number = validate_input("Enter roll number to delete: ", 1, 9999);
    int index = binary_search(students, 0, *count - 1, roll_number);

    if (index != -1) {
        for (int i = index; i < *count - 1; i++) {
            students[i] = students[i + 1];
        }
        (*count)--;
        printf("Student deleted successfully!\n");
    } else {
        printf("Student with roll number %d not found!\n", roll_number);
    }
}

// Sorting students by roll number, name, or marks
void sort_students(Student *students, int count) {
    int choice = validate_input("Sort by: 1. Roll Number, 2. Name, 3. Marks: ", 1, 3);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int swap = 0;
            if (choice == 1 && students[i].roll_number > students[j].roll_number) swap = 1;
            if (choice == 2 && strcmp(students[i].name, students[j].name) > 0) swap = 1;
            if (choice == 3 && students[i].marks > students[j].marks) swap = 1;

            if (swap) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    printf("Students sorted successfully!\n");
}

// Save data to file
void save_to_file(Student *students, int count) {
    FILE *file = fopen("students.dat", "wb");
    if (!file) {
        printf("Error opening file for saving!\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), count, file);
    fclose(file);
    printf("Data saved successfully!\n");
}
// Load data from file
Student* load_from_file(Student *students, int *count, int *capacity) {
    FILE *file = fopen("students.dat", "rb");
    if (!file) {
        printf("Error opening file for loading!\n");
        return students;
    }

    fread(count, sizeof(int), 1, file);

    if (*count > *capacity) {
        *capacity = *count;
        students = (Student*) realloc(students, (*capacity) * sizeof(Student));
    }

    fread(students, sizeof(Student), *count, file);
    fclose(file);
    printf("Data loaded successfully!\n");

    return students;
}

// Free dynamically allocated memory
void free_memory(Student *students) {
    free(students);
}

// Main function with menu
int main() {
    int capacity = 10;  // Initial capacity for student array
    int count = 0;      // Number of students
    Student *students = (Student *)malloc(capacity * sizeof(Student));
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                students = add_student(students, &count, &capacity);
                break;
            case 2:
                paginate_students(students, count, PAGE_SIZE);
                break;
            case 3:
                search_student(students, count, 1);  // Search by roll number
                break;
            case 4:
                search_student(students, count, 0);  // Search by name
                break;
            case 5:
                update_student(students, count);
                break;
            case 6:
                delete_student(students, &count);
                break;
            case 7:
                sort_students(students, count);
                break;
            case 8:
                save_to_file(students, count);
                break;
            case 9:
                students = load_from_file(students, &count, &capacity);
                break;
            case 10:
                printf("Saving data and exiting...\n");
                save_to_file(students, count);
                free_memory(students);
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

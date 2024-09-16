# Student Database Management System

A simple Student Database Management System written in C. This project allows users to add, update, delete, search, and display student records. It also supports features like saving and loading data from files, sorting students by roll number, pagination for displaying students, and dynamic memory management.

## Features
- Add new students with validation for roll number, age, and marks.
- Display student records with pagination (viewing a few records per page).
- Search for students by roll number or name.
- Update existing student records.
- Delete student records.
- Save and load student data to/from a file.
- Sort students by roll number.
- Efficient memory management with dynamic allocation.

## How to Run

### Prerequisites
- C Compiler (e.g., GCC)
- `make` (optional, if you use a Makefile for compilation)
- Git (if you want to clone this repository)

### Compilation

1. **Using GCC**:

   If you're using GCC, you can compile the program using the following command:

   ```bash
   gcc student_dbms.c -o student_dbms


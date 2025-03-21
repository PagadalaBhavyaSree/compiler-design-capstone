#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_CODE_SIZE 1024
#define TIMEOUT 2  // Execution timeout in seconds
// List of unsafe functions for static analysis
const char *unsafe_functions[] = {"gets", "strcpy", "sprintf", "system", NULL};
// Function to perform lexical analysis (tokenization)
void lexical_analysis(const char *code) {
    printf("Performing Lexical Analysis...\n");
    const char *keywords[] = {"int", "return", "if", "else", "for", "while", "void", NULL};
    char buffer[100];
    int i = 0, j = 0;  
    while (code[i] != '\0') {
        if (isalpha(code[i])) {  // Identify keywords and identifiers
            buffer[j++] = code[i];
        } else {
            buffer[j] = '\0';
            j = 0;
            for (int k = 0; keywords[k] != NULL; k++) {
                if (strcmp(buffer, keywords[k]) == 0) {
                    printf("Keyword detected: %s\n", buffer);
                }
            }
        }
        i++;
    }
}
// Function to perform static analysis (detect unsafe functions)
void static_analysis(const char *code) {
    printf("Performing Static Analysis...\n");
    for (int i = 0; unsafe_functions[i] != NULL; i++) {
        if (strstr(code, unsafe_functions[i]) != NULL) {
            printf("Warning: Unsafe function detected: %s\n", unsafe_functions[i]);
        }
    }
}
// Function to perform taint analysis (tracking input variables)
void taint_analysis(const char *code) {
    printf("Performing Taint Analysis...\n");
    if (strstr(code, "scanf") != NULL || strstr(code, "gets") != NULL) {
        printf("Warning: Untrusted input detected. Validate user input properly.\n");
    }
}
// Function to execute the student's program securely
void execute_code(const char *filename) {
    printf("Executing compiled code with security checks...\n");
    char command[256];
    snprintf(command, sizeof(command), "timeout %d ./%s", TIMEOUT, filename);
    system(command);
}
// Function to perform auto-grading by comparing outputs
void auto_grading(const char *student_output, const char *expected_output) {
    FILE *stu_file = fopen(student_output, "r");
    FILE *exp_file = fopen(expected_output, "r");
    if (!stu_file || !exp_file) {
        printf("Error: Could not open output files for grading.\n");
        return;
    }
    char stu_line[100], exp_line[100];
    int correct = 1;
    while (fgets(stu_line, sizeof(stu_line), stu_file) && fgets(exp_line, sizeof(exp_line), exp_file)) {
        if (strcmp(stu_line, exp_line) != 0) {
            correct = 0;
            break;
        }
    }
    fclose(stu_file);
    fclose(exp_file);
    if (correct) {
        printf("Auto-Grading Result: PASS\n");
    } else {
        printf("Auto-Grading Result: FAIL\n");
    }
}
// Main function to compile and evaluate student code
int main() {
    char student_code[MAX_CODE_SIZE];
    FILE *code_file = fopen("student_code.c", "r");
    if (!code_file) {
        printf("Error: Unable to read student_code.c\n");
        return 1;
    }
    fread(student_code, sizeof(char), MAX_CODE_SIZE, code_file);
    fclose(code_file);
    // Perform security and correctness checks
    lexical_analysis(student_code);
    static_analysis(student_code);
    taint_analysis(student_code);
    // Compile the student code
    printf("Compiling student code...\n");
    int compile_status = system("gcc student_code.c -o student_program 2> compile_errors.txt");
    if (compile_status != 0) {
        printf("Compilation failed. Check compile_errors.txt for details.\n");
        return 1;
    }
    // Execute compiled code securely
    execute_code("student_program");
    // Perform auto-grading
    auto_grading("student_output.txt", "expected_output.txt");
    return 0;
}

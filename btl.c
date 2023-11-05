
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_LENGTH_COMMAND 300
#define MAX_NO_TASKS 100
#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33
#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20
enum Status
{
    IN_PROGRESS,
    DONE,
    ARCHIVED
};
char *status_name[] = {"In Progress", "Done", "Archived"};
enum CommandType
{
    ADD,
    EDIT,
    SHOW,
    DELETE,
    QUIT,
    INVALID
};
char *command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};
struct Task
{
    int num;
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];
    enum Status status;
};
void printTask(struct Task *task)
{
    printf("--------------------------------------------\n");
    printf("Num: #%d. Title: %s\n", task->num, task->title);
    printf("Description: %s\n", task->description);
    printf("Status: %s\n", status_name[task->status]);
    printf("--------------------------------------------\n");
}
void printUnsupportedTime(struct Task *task)
{
    printf("----- Show week view -----\n");
    printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
    printf("In Task:\n");
    printTask(task);
}
// ------ Begin: Student Answer ------
enum CommandType getCommandType(char *command)
{
    char commandCopy[200];
    strncpy(commandCopy, command, sizeof(commandCopy));
    commandCopy[sizeof(commandCopy) - 1] = '\0';

    char *token = strtok(commandCopy, " ");
    if (token == NULL)
    {
        return INVALID;
    } // Nếu không có lệnh thì trả về INVALID

    struct
    {
        const char *name;
        enum CommandType type;
    } commandTypes[] = {
        {"Add", ADD},
        {"Edit", EDIT},
        {"Show", SHOW},
        {"Delete", DELETE},
        {"Quit", QUIT},
    }; // Khởi tạo mảng struct

    for (size_t i = 0; i < sizeof(commandTypes) / sizeof(commandTypes[0]); ++i)
    {
        if (strcasecmp(token, commandTypes[i].name) == 0) // So sánh 2 chuỗi không phân biệt chữ hoa chữ thường
        {
            return commandTypes[i].type; // Trả về kiểu lệnh nếu tìm thấy
        }
    }
    return INVALID; // Trả về INVALID nếu không tìm thấy
}
// Other functions

// Thành phần Add
int checkTitle(char *raw_title)
{
    int length = strlen(raw_title);
    if (length > 100)
    {
        return length;
    }
    if (isspace(raw_title[0]) || isspace(raw_title[length - 1]))
    {
        return 0;
    }
    for (int i = 0; i < length; ++i)
    {
        if (!(isalnum(raw_title[i]) || strchr(" ,.-:|/", raw_title[i])))
        {
            return i;
        }
    }
    return -1;
}

void getTitleFromAdd(char *command, char *out_title)
{
    char *start = strchr(command, '[') + 1; // Tìm thấy ký tự '[' đầu tiên
    char *end = strchr(start, ']');
    strncpy(out_title, start, end - start);
    out_title[end - start] = '\0';
}

void getDescriptionFromAdd(char *command, char *out_description)
{
    char *start = strchr(strchr(command, '[') + 1, '[') + 1; // Tìm thấy ký tự '[' thứ 2
    char *end = strchr(start, ']');
    strncpy(out_description, start, end - start);
    out_description[end - start] = '\0';
}

void getTimeFromAdd(char *command, char *out_time)
{
    char *start = strchr(strchr(strchr(command, '[') + 1, '[') + 1, '[') + 1; // Tìm thấy ký tự '[' thứ 3
    char *end = strchr(start, ']');
    strncpy(out_time, start, end - start);
    out_time[end - start] = '\0';
}

int checkDescription(char *raw_description)
{
    int length = strlen(raw_description);
    if (length > 200)
    {
        return length;
    }
    if (isspace(raw_description[0]) || isspace(raw_description[length - 1]))
    {
        return 0;
    }
    for (int i = 0; i < length; ++i)
    {
        if (!(isalnum(raw_description[i]) || strchr(" ,.-:|/", raw_description[i])))
        {
            return i;
        }
    }
    return -1;
}

int isLeapYear(int year)
{
    return year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);
}

int checkTime(char *raw_time)
{
    int day1, month1, year1, hour1, minute1;
    int day2, month2, year2, hour2, minute2;
    if (sscanf(raw_time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d", &hour1, &minute1, &day1, &month1, &year1, &hour2, &minute2, &day2, &month2, &year2) != 10)
    {
        return 0;
    }
    if (hour1 < 0 || hour1 > 23)
    {
        return 11 + hour1;
    }
    if (minute1 < 0 || minute1 > 59)
    {
        return 21 + minute1;
    }
    if (day1 < 1 || day1 > 31)
    {
        return 31 + day1;
    }
    if (month1 < 1 || month1 > 12)
    {
        return 41 + month1;
    }
    if (year1 < 1)
    {
        return 51 + year1;
    }
    if (hour2 < 0 || hour2 > 23)
    {
        return 12 + hour2;
    }
    if (minute2 < 0 || minute2 > 59)
    {
        return 22 + minute2;
    }
    if (day2 < 1 || day2 > 31)
    {
        return 32 + day2;
    }
    if (month2 < 1 || month2 > 12)
    {
        return 42 + month2;
    }
    if (year2 < 1)
    {
        return 52 + month2;
    }
    if (year1 < year2 || (year1 == year2 && (month1 < month2 || (month1 == month2 && (day1 < day2 || (day1 == day2 && (hour1 < hour2 || (hour1 == hour2 && minute1 < minute2))))))))
    {
        return -1;
    }
    return 0;
}
// Thành phần Edit
void getTitleFromEdit(char *command, char *out_title)
{
    char *start = strchr(command, '[') + 1; // Find the first '['
    char *end = strchr(start, ']');         // Find the first ']' after the first '['
    strncpy(out_title, start, end - start);
    out_title[end - start] = '\0';
}

void getDescriptionFromEdit(char *command, char *out_description)
{
    char *start = strchr(strchr(command, '[') + 1, '[') + 1; // Find the second '['
    char *end = strchr(start, ']');                          // Find the first ']' after the second '['
    strncpy(out_description, start, end - start);
    out_description[end - start] = '\0';
}

void getTimeFromEdit(char *command, char *out_time)
{
    char *start = strchr(strchr(strchr(command, '[') + 1, '[') + 1, '[') + 1; // Find the third '['
    char *end = strchr(start, ']');                                           // Find the first ']' after the third '['
    strncpy(out_time, start, end - start);
    out_time[end - start] = '\0';
}
int getNumFromCommand(char *command)
{
    char *start = strchr(command, '#');
    if (start == NULL)
    {
        return -1;
    }
    start += 1;
    char *end;
    long num = strtol(start, &end, 10);
    if (end == start)
    {
        return 0;
    }
    return (int)num;
}
int getFieldFromEdit(char *edit_cmd)
{
    char *start = strchr(strchr(edit_cmd, ' ') + 1, ' ') + 1; // Find the second ' '
    char *end = strchr(start, ':');                           // Find the first ':' after the second ' '
    char field[end - start + 1];
    strncpy(field, start, end - start);
    field[end - start] = '\0';
    if (strcmp(field, "title") == 0)
    {
        return 1;
    }
    else if (strcmp(field, "description") == 0)
    {
        return 2;
    }
    else if (strcmp(field, "time") == 0)
    {
        return 3;
    }
    else if (strcmp(field, "status") == 0)
    {
        return 4;
    }
    else
    {
        return 0;
    }
}
enum Status getStatusFromEdit(char *edit_cmd)
{
    char *start = strchr(edit_cmd, ':') + 1; // Find the first ':'
    char status = *start;
    switch (status)
    {
    case 'I':
    case 'i':
        return IN_PROGRESS;
    case 'D':
    case 'd':
        return DONE;
    case 'A':
    case 'a':
        return ARCHIVED;
    default:
        return INVALID;
    }
}
// Thành phần Show
void printAllTasks(struct Task *array_tasks, int no_tasks)
{
    for (int i = 0; i < no_tasks; i++)
    {
        printTask(&array_tasks[i]);
    }
}
void printTaskByNum(struct Task *array_tasks, int no_tasks, int num)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (array_tasks[i].num == num)
        {
            printTask(&array_tasks[i]);
            break;
        }
    }
}
void printHeadTasks(struct Task *array_tasks, int no_tasks, int quan)
{
    int tasks_to_print = quan < no_tasks ? quan : no_tasks;
    for (int i = 0; i < tasks_to_print; i++)
    {
        printTask(&array_tasks[i]);
    }
}
void printTailTasks(struct Task *array_tasks, int no_tasks, int quan)
{
    int start = no_tasks - quan > 0 ? no_tasks - quan : 0;
    for (int i = start; i < no_tasks; i++)
    {
        printTask(&array_tasks[i]);
    }
}

void printFilteredTasksByTitle(struct Task *array_tasks, int no_tasks, char *filter_title)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (strstr(array_tasks[i].title, filter_title) != NULL)
        {
            printTask(&array_tasks[i]);
        }
    }
}

void printFilteredTasksByDescription(struct Task *array_tasks, int no_tasks, char *filter_description)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (strstr(array_tasks[i].description, filter_description) != NULL)
        {
            printTask(&array_tasks[i]);
        }
    }
}

void printFilteredTasksByStatus(struct Task *array_tasks, int no_tasks, enum Status filter_status)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (array_tasks[i].status == filter_status)
        {
            printTask(&array_tasks[i]);
        }
    }
}
// Thành phần Add
bool addTask(struct Task *array_tasks, int *no_tasks, char *new_title, char *new_description, char *new_time)
{
    if (*no_tasks == MAX_NO_TASKS)
    {
        return false;
    }

    strcpy(array_tasks[*no_tasks].title, new_title);
    strcpy(array_tasks[*no_tasks].description, new_description);
    strcpy(array_tasks[*no_tasks].time, new_time);
    array_tasks[*no_tasks].num = *no_tasks + 1;
    array_tasks[*no_tasks].status = IN_PROGRESS;

    (*no_tasks)++;
    return true;
}
// Thành phần delete
bool deleteTask(struct Task *array_tasks, int *no_tasks, int num)
{
    if (num < 1 || num > *no_tasks)
    {
        return false;
    }

    for (int i = num - 1; i < *no_tasks - 1; i++)
    {
        array_tasks[i] = array_tasks[i + 1];
    }

    (*no_tasks)--;

    for (int i = num - 1; i < *no_tasks; i++)
    {
        array_tasks[i].num = i + 1;
    }

    return true;
}

// ------ End: Student Answer ------
void runTodoApp()
{
    // Example of command Add
    char command[MAX_LENGTH_COMMAND + 1];

    while (true)
    {
        // Sample input:
        // Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-
        // 12:00|01/10/2023]
        fgets(command, MAX_LENGTH_COMMAND + 1, stdin);
        command[strlen(command) - 1] = '\0';

        enum CommandType commandType = getCommandType(command);
        printf("Command : %s\n", command);
        printf("Command type: %s\n", command_name[commandType]);
        break; // only one loop for simple test
               // actual app will break when encounter QUIT command
    }
}
int main()
{
    runTodoApp();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
bool is_space(char c)
{
    return c == ' ';
}

bool is_alphanumeric(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
int checkTitle(char *raw_title)
{
    int length = strlen(raw_title);
    if (length > 100)
    {
        return length;
    }
    if (is_space(raw_title[0]) || is_space(raw_title[length - 1]))
    {
        return 0;
    }
    for (int i = 0; i < length; ++i)
    {
        if (!(is_alphanumeric(raw_title[i]) || strchr(" ,.-:|/", raw_title[i])))
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
    if (is_space(raw_description[0]) || is_space(raw_description[length - 1])) // Nếu ký tự đầu hoặc cuối là khoảng trắng thì trả về vị trí lỗi
    {
        return 0;
    }
    for (int i = 0; i < length; ++i)
    {
        if (!(is_alphanumeric(raw_description[i]) || strchr(" ,.-:|/", raw_description[i]))) // Nếu không phải là ký tự đặc biệt thì trả về vị trí lỗi
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
    sscanf(raw_time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d", &hour1, &minute1, &day1, &month1, &year1, &hour2, &minute2, &day2, &month2, &year2);

    if (isLeapYear(year1))
    {
        if (month1 == 2 && (day1 > 29 || day1 < 1))
        {
            return 3100 + day1;
        }
    }
    else
    {
        if (month1 == 2 && (day1 > 28 || day1 < 1))
        {
            return 3100 + day1;
        }
    }
    if (hour1 < 0 || hour1 > 23)
    {
        return 1100 + hour1;
    }
    if (minute1 < 0 || minute1 > 59)
    {
        return 2100 + minute1;
    }
    if (day1 < 1 || day1 > 31)
    {
        return 3100 + day1;
    }
    if (month1 < 1 || month1 > 12)
    {
        return 4100 + month1;
    }
    if (year1 < 1)
    {
        return 510000 + year1;
    }
    if (isLeapYear(year2))
    {
        if (month2 == 2 && (day2 > 29 || day2 < 1))
        {
            return 3200 + day2;
        }
    }
    else
    {
        if (month2 == 2 && (day2 > 28 || day2 < 1))
        {
            return 3200 + day2;
        }
    }
    if (hour2 < 0 || hour2 > 23)
    {
        return 1200 + hour2;
    }
    if (minute2 < 0 || minute2 > 59)
    {
        return 2200 + minute2;
    }
    if (day2 < 1 || day2 > 31)
    {
        return 3200 + day2;
    }
    if (month2 < 1 || month2 > 12)
    {
        return 4200 + month2;
    }
    if (year2 < 1)
    {
        return 520000 + year2;
    }
    if (minute1 + hour1 * 60 + day1 * 1440 + month1 * 44640 + year1 * 535680 > minute2 + hour2 * 60 + day2 * 1440 + month2 * 44640 + year2 * 535680)
    {
        return 0;
    }
    return -1;
}

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
// Thành phần Edit
void getTitleFromEdit(char *command, char *out_title)
{
    char *start = strchr(command, '[') + 1;
    char *end = strchr(start, ']');
    strncpy(out_title, start, end - start);
    out_title[end - start] = '\0';
}

void getDescriptionFromEdit(char *command, char *out_description)
{
    char *start = strchr(command, '[') + 1;
    char *end = strchr(start, ']');
    strncpy(out_description, start, end - start);
    out_description[end - start] = '\0';
}

void getTimeFromEdit(char *command, char *out_time)
{
    char *start = strchr(command, '[') + 1;
    char *end = strchr(start, ']');
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
    char *start = strchr(strchr(edit_cmd, ' ') + 1, ' ') + 1;
    char *end = strchr(start, ':');
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
    char *start = strchr(edit_cmd, '[') + 1;
    char *end = strchr(edit_cmd, ']');
    char status[end - start + 1];
    strncpy(status, start, end - start);
    status[end - start] = '\0';

    if (strcasecmp(status, "I") == 0)
        return IN_PROGRESS;
    else if (strcasecmp(status, "D") == 0)
        return DONE;
    else if (strcasecmp(status, "A") == 0)
        return ARCHIVED;
    else
        return INVALID;
}
// Thành phần Show
int isTimeEven(char *time)
{
    char *start_time = time;
    while ((start_time = strchr(start_time, ':')) != NULL) // Tìm thấy ký tự ':' đầu tiên
    {
        start_time++; // Bỏ qua ký tự ':'
        char *end_time = strchr(start_time, '|');
        if (end_time == NULL)
        {
            end_time = strchr(start_time, '-');
            if (end_time == NULL)
            {
                end_time = strchr(start_time, '\0');
            }
        }
        char minute[3];
        strncpy(minute, start_time, end_time - start_time); // Lấy phút
        minute[end_time - start_time] = '\0';               // Lấy phút
        if (strcmp(minute, "00") != 0)
        {
            return 0; // Nếu phút khác 00 thì trả về 0
        }
        start_time = end_time; // Tiếp tục tìm ký tự ':' tiếp theo
    }
    return 1;
}
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
int printWeekTime(struct Task *array_tasks, int no_tasks, char *date)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (!isTimeEven(array_tasks[i].time))
        {
            printUnsupportedTime(&array_tasks[i]);
            return i;
        }
    }
    printf("Các công việc trong tuần bắt đầu từ ngày %s:\n", date);
    for (int i = 0; i < no_tasks; i++)
    {
        char *time = array_tasks[i].time;
        char *date_part = strtok(time, "|");
        date_part = strtok(NULL, "|");

        if (strcmp(date_part, date + 4) == 0)
        {
            printf("Tiêu đề: %s\n", array_tasks[i].title);
            printf("Mô tả: %s\n", array_tasks[i].description);
            printf("Thời gian: %s\n", array_tasks[i].time);
        }
    }
    return -1;
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
void TestFunction() // Yêu cầu 20

{
    struct Task tasks[MAX_NO_TASKS];
    int no_tasks = 0;

    char command[MAX_LENGTH_COMMAND + 1];
    while (true)
    {
        // Sample input:
        // Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]

        printf("Nhap lenh:\n");
        fgets(command, MAX_LENGTH_COMMAND + 1, stdin);
        command[strlen(command) - 1] = '\0';
        enum CommandType commandType = getCommandType(command);
        printf("Command : %s\n", command);
        printf("Command type: %s\n", command_name[commandType]);
        if (command_name[commandType] == "QUIT")
        {
            break;
        }
        if (command_name[commandType] == "ADD")
        {
            char title[MAX_LENGTH_TITLE + 1];
            char description[MAX_LENGTH_DESCRIPTION + 1];
            char time[MAX_LENGTH_TIME + 1];
            getTitleFromAdd(command, title);
            getDescriptionFromAdd(command, description);
            getTimeFromAdd(command, time);
            int checkTitleResult = checkTitle(title);
            int checkDescriptionResult = checkDescription(description);
            int checkTimeResult = checkTime(time);
            if (checkTitleResult == -1 && checkDescriptionResult == -1 && checkTimeResult == -1)
            {
                if (addTask(tasks, &no_tasks, title, description, time))
                {
                    printf("Add task successfully\n");
                }
                else
                {
                    printf("Cannot add more task\n");
                }
            }
            else
            {
                if (checkTitleResult != -1)
                {
                    printf("Title is invalid at position %d\n", checkTitleResult);
                }
                if (checkDescriptionResult != -1)
                {
                    printf("Description is invalid at position %d\n", checkDescriptionResult);
                }
                if (checkTimeResult != -1)
                {
                    printf("Time is invalid at position %d\n", checkTimeResult);
                }
            }
        }
        // EDIT #1 title: [Learn Python Programming]
        // EDIT #1 description: [Home]
        // EDIT #1 time: [09:00|01/12/2022-11:00|01/12/2022]
        // EDIT #1 status: [Done]
        if (command_name[commandType] == "EDIT")
        {
            char title[MAX_LENGTH_TITLE + 1];
            char description[MAX_LENGTH_DESCRIPTION + 1];
            char time[MAX_LENGTH_TIME + 1];
            int num = getNumFromCommand(command);
            int field = getFieldFromEdit(command);
            enum Status status = getStatusFromEdit(command);
            if (num == -1 || num > no_tasks || num < 1)
            {
                printf("Cannot find task with num %d\n", num);
            }
            else if (num == 0)
            {
                printf("Invalid num\n");
            }
            else if (field == 0 || field > 4)
            {
                printf("Invalid field\n");
            }
            else if (field == 1)
            {
                getTitleFromEdit(command, title);
                int checkTitleResult = checkTitle(title);
                if (checkTitleResult == -1)
                {
                    strcpy(tasks[num - 1].title, title);
                    printf("Edit task successfully\n");
                }
                else
                {
                    printf("Title is invalid at position %d\n", checkTitleResult);
                }
            }
            else if (field == 2)
            {
                getDescriptionFromEdit(command, description);
                int checkDescriptionResult = checkDescription(description);
                if (checkDescriptionResult == -1)
                {
                    strcpy(tasks[num - 1].description, description);
                    printf("Edit task successfully\n");
                }
                else
                {
                    printf("Description is invalid at position %d\n", checkDescriptionResult);
                }
            }
            else if (field == 3)
            {
                getTimeFromEdit(command, time);
                int checkTimeResult = checkTime(time);
                if (checkTimeResult == -1)
                {
                    strcpy(tasks[num - 1].time, time);
                    printf("Edit task successfully\n");
                }
                else
                {
                    printf("Time is invalid at position %d\n", checkTimeResult);
                }
            }
            else if (field == 4)
            {
                if (status == INVALID)
                {
                    printf("Invalid status\n");
                }
                else
                {
                    tasks[num - 1].status = status;
                    printf("Edit task successfully\n");
                }
            }
        }
        // Show all
        // Show #1...

        if (command_name[commandType] == "SHOW")
        {
            char filter_title[MAX_LENGTH_TITLE + 1];
            char filter_description[MAX_LENGTH_DESCRIPTION + 1];
            enum Status filter_status;
            char date[MAX_LENGTH_TIME + 1];
            int num = getNumFromCommand(command);
            int quan;
            if (num == -1)
            {
                if (strstr(command, "head") != NULL)
                {
                    sscanf(command, "%*s %*s %d", &quan);
                    printHeadTasks(tasks, no_tasks, quan);
                }
                if (strstr(command, "tail") != NULL)
                {
                    sscanf(command, "%*s %*s %d", &quan);
                    printTailTasks(tasks, no_tasks, quan);
                }
                if (strstr(command, "title") != NULL)
                {
                    getTitleFromEdit(command, filter_title);
                    printFilteredTasksByTitle(tasks, no_tasks, filter_title);
                }
                if (strstr(command, "description") != NULL)
                {
                    getDescriptionFromEdit(command, filter_description);
                    printFilteredTasksByDescription(tasks, no_tasks, filter_description);
                }
                if (strstr(command, "status") != NULL)
                {
                    filter_status = getStatusFromEdit(command);
                    if (filter_status == INVALID)
                    {
                        printf("Invalid status\n");
                    }
                    else
                    {
                        printFilteredTasksByStatus(tasks, no_tasks, filter_status);
                    }
                }
                if (strstr(command, "week") != NULL)
                {
                    getTimeFromEdit(command, date);
                    if (printWeekTime(tasks, no_tasks, date) != -1)
                    {
                        break;
                    }
                }
                if (strstr(command, "all") != NULL)
                {
                    printAllTasks(tasks, no_tasks);
                }
            }
            else
            {
                if (num == 0)
                {
                    printf("Invalid num\n");
                }
                else
                {
                    printTaskByNum(tasks, no_tasks, num);
                }
            }
        }
        // Delete #1
        if (command_name[commandType] == "DELETE")
        {
            int num = getNumFromCommand(command);
            if (num == -1)
            {
                printf("wrong format %d\n", num);
            }
            else if (num == 0)
            {
                printf("Invalid num\n");
            }
            else
            {
                if (deleteTask(tasks, &no_tasks, num))
                {
                    printf("Delete task successfully\n");
                }
                else
                {
                    printf("Cannot delete task\n");
                }
            }
        }
    }
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
    runTodoApp(); // Nếu thực hiện yêu cầu 20 thì chạy hàm TestFunction() thay cho hàm runTodoApp()
    return 0;
}
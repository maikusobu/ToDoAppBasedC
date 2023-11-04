#include <stdio.h>
#include <string.h>
#include <ctype.h>
typedef enum
{
    ADD,
    EDIT,
    SHOW,
    DELETE,
    QUIT,
    INVALID
} CommandType;

CommandType getCommandType(const char *command)
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
        CommandType type;
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
    char *start = strchr(command, '[') + 1;
    char *end = strchr(start, ']');
    strncpy(out_title, start, end - start);
    out_title[end - start] = '\0';
}

void getDescriptionFromAdd(char *command, char *out_description)
{
    char *start = strchr(strchr(command, '[') + 1, '[') + 1;
    char *end = strchr(start, ']');
    strncpy(out_description, start, end - start);
    out_description[end - start] = '\0';
}

void getTimeFromAdd(char *command, char *out_time)
{
    char *start = strchr(strchr(strchr(command, '[') + 1, '[') + 1, '[') + 1;
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
// Thành phàn Add

int main()
{
    char command[100];
    printf("Nhập lệnh: ");
    fgets(command, sizeof(command), stdin);
    strtok(command, "\n"); // Loại bỏ ký tự newline khi đọc chuỗi từ bàn phím
    CommandType type = getCommandType(command);
    printf("Kiểu lệnh: %d\n", type);
    char command[] = "Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]";
    char raw_title[200];
    char raw_description[200];
    char raw_time[200];
    getTitleFromAdd(command, raw_title);
    getDescriptionFromAdd(command, raw_description);
    getTimeFromAdd(command, raw_time);
    printf("raw_title: %s\n", raw_title);
    printf("raw_description: %s\n", raw_description);
    printf("raw_time: %s\n", raw_time);

    return 0;
}
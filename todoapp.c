#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#define MAX_LENGTH_COMMAND 300
#define MAX_NO_TASKS 100
#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33
#define WEEK_CELL_FIRST_COL_WIDTH 12
#define WEEK_CELL_OTHER_COL_WIDTH 19
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
// Yêu cầu 1
enum CommandType getCommandType(char *command)
{

  char commandCopy[200];
  strncpy(commandCopy, command, sizeof(commandCopy));
  commandCopy[sizeof(commandCopy) - 1] = '\0';

  char *token = strtok(commandCopy, " ");
  if (token == NULL)
  {
    return INVALID;
  }
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
  };

  for (size_t i = 0; i < sizeof(commandTypes) / sizeof(commandTypes[0]); ++i)
  {

    if (strcasecmp(token, commandTypes[i].name) == 0)
    {
      return commandTypes[i].type;
    }
  }
  return INVALID;
}
// helper function
bool is_space(char c)
{
  return c == ' ';
}

bool is_alphanumeric(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
// 2 hàm trên để kiểm tra xem có phải là ký tự khoảng trắng và ký tự chữ và số hay không
// Yêu cầu 2
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
// Yêu cầu 3
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
// Yêu câu 4
int checkDescription(char *raw_description)
{

  int length = strlen(raw_description);

  if (length > 200)
  {
    return length;
  }

  if (is_space(raw_description[0]) || is_space(raw_description[length - 1]))
  {
    return 0;
  }

  for (int i = 0; i < length; ++i)
  {

    if (!(is_alphanumeric(raw_description[i]) || strchr(" ,.-:|/", raw_description[i])))
    {
      return i;
    }
  }

  return -1;
}

// Yêu cầu 5
int isLeapYear(int year)
{

  return year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);
}

int checkTime(char *raw_time)
{

  int day1, month1, year1, hour1, minute1;
  int day2, month2, year2, hour2, minute2;

  sscanf(raw_time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d", &hour1, &minute1, &day1, &month1, &year1, &hour2, &minute2, &day2, &month2, &year2);
  if (hour1 < 0 || hour1 > 23)
  {
    return 1100 + hour1;
  }
  if (minute1 < 0 || minute1 > 59)
  {
    return 2100 + minute1;
  }
  if (month1 == 1 || month1 == 3 || month1 == 5 || month1 == 7 || month1 == 8 || month1 == 10 || month1 == 12)
  {
    if (day1 < 1 || day1 > 31)
    {
      return 3100 + day1;
    }
  }
  else if (month1 == 4 || month1 == 6 || month1 == 9 || month1 == 11)
  {
    if (day1 < 1 || day1 > 30)
    {
      return 3100 + day1;
    }
  }
  else if (month1 == 2)
  {
    if (isLeapYear(year1))
    {
      if (day1 < 1 || day1 > 29)
      {
        return 3100 + day1;
      }
    }
    else
    {
      if (day1 < 1 || day1 > 28)
      {
        return 3100 + day1;
      }
    }
  }
  else
  {
    return 4100 + month1;
  }
  if (year1 < 1)
  {
    return 510000 + year1;
  }
  if (hour2 < 0 || hour2 > 23)
  {
    return 1200 + hour2;
  }
  if (minute2 < 0 || minute2 > 59)
  {
    return 2200 + minute2;
  }
  if (month2 == 1 || month2 == 3 || month2 == 5 || month2 == 7 || month2 == 8 || month2 == 10 || month2 == 12)
  {
    if (day2 < 1 || day2 > 31)
    {
      return 3200 + day2;
    }
  }
  else if (month2 == 4 || month2 == 6 || month2 == 9 || month2 == 11)
  {
    if (day2 < 1 || day2 > 30)
    {
      return 3200 + day2;
    }
  }
  else if (month2 == 2)
  {
    if (isLeapYear(year2))
    {
      if (day2 < 1 || day2 > 29)
      {
        return 3200 + day2;
      }
    }
    else
    {
      if (day2 < 1 || day2 > 28)
      {
        return 3200 + day2;
      }
    }
  }
  else
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

// Yêu cầu 6
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
// Yêu cầu 7
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
// Yêu cầu 8
int getFieldFromEdit(char *edit_cmd)
{

  char *start = strchr(strchr(edit_cmd, ' ') + 1, ' ') + 1;

  char *end = strchr(start, ':');

  char *field = (char *)malloc(end - start + 1);
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

// Yêu cầu 9
enum Status getStatusFromEdit(char *edit_cmd)
{

  char *start = strchr(edit_cmd, '[') + 1;

  char *end = strchr(edit_cmd, ']');

  char *status = (char *)malloc(end - start + 1);
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
// Yêu cầu 10
void printAllTasks(struct Task *array_tasks, int no_tasks)
{

  for (int i = 0; i < no_tasks; i++)
  {

    printTask(&array_tasks[i]);
  }
}
// Yêu cầu 11

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
// Yêu cầu 12
void printHeadTasks(struct Task *array_tasks, int no_tasks, int quan)
{

  int tasks_to_print = quan < no_tasks ? quan : no_tasks;

  for (int i = 0; i < tasks_to_print; i++)
  {
    printTask(&array_tasks[i]);
  }
}
// Yêu cầu 13
void printTailTasks(struct Task *array_tasks, int no_tasks, int quan)
{

  int start = no_tasks - quan > 0 ? no_tasks - quan : 0;

  for (int i = start; i < no_tasks; i++)
  {
    printTask(&array_tasks[i]);
  }
}
// Yêu cầu 14
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
// Yêu cầu 15
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
// Yêu cầu 16
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
// Yêu cầu 17
bool addTask(struct Task *array_tasks, int no_tasks, char *new_title, char *new_description, char *new_time)
{

  if (no_tasks == MAX_NO_TASKS)
  {

    return false;
  }

  strcpy(array_tasks[no_tasks].title, new_title);

  strcpy(array_tasks[no_tasks].description, new_description);

  strcpy(array_tasks[no_tasks].time, new_time);

  array_tasks[no_tasks].num = no_tasks + 1;

  array_tasks[no_tasks].status = IN_PROGRESS;

  return true;
}
// Yêu cầu 18
bool deleteTask(struct Task *array_tasks, int no_tasks, int num)
{

  if (num < 1 || num > no_tasks)
  {

    return false;
  }

  for (int i = num - 1; i < no_tasks - 1; i++)
  {
    array_tasks[i] = array_tasks[i + 1];
  }

  for (int i = num - 1; i < no_tasks; i++)
  {
    array_tasks[i].num = i + 1;
  }

  return true;
}
// Yêu cầu 19
int isTimeEven(char *time)
{
  char *start_time = time;
  while ((start_time = strchr(start_time, ':')) != NULL)
  {
    start_time++;
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
    strncpy(minute, start_time, end_time - start_time);
    minute[end_time - start_time] = '\0';
    if (strcmp(minute, "00") != 0)
    {
      return 0;
    }
    start_time = end_time;
  }
  return 1;
}
int getDayNumber(char *day)
{
  if (strcmp(day, "mon") == 0)
    return 1;
  if (strcmp(day, "tue") == 0)
    return 2;
  if (strcmp(day, "wed") == 0)
    return 3;
  if (strcmp(day, "thu") == 0)
    return 4;
  if (strcmp(day, "fri") == 0)
    return 5;
  if (strcmp(day, "sat") == 0)
    return 6;
  if (strcmp(day, "sun") == 0)
    return 7;
  return -1;
}

int getDaysInMonth(int month, int year)
{
  if (month == 2)
  {
    if (isLeapYear(year))
    {
      return 29;
    }
    else
    {
      return 28;
    }
  }
  else if (month == 4 || month == 6 || month == 9 || month == 11)
  {
    return 30;
  }
  else
  {
    return 31;
  }
}

char **WeekArray(char *input)
{
  char day[4];
  int date, month, year;
  sscanf(input, "%[^/]/%d/%d/%d", day, &date, &month, &year);

  int dayNumber = getDayNumber(day);
  int start = date - dayNumber + 1;
  char **result = malloc(8 * sizeof(char *));
  char *str = malloc(4 * sizeof(char));

  sprintf(str, "%d", year);
  result[0] = str;
  for (int i = 1; i < 8; i++)
  {
    int currentDay = start + i - 1;
    int currentMonth = month;
    int currentYear = year;

    if (currentDay <= 0)
    {
      currentMonth--;
      if (currentMonth <= 0)
      {
        currentMonth = 12;
        currentYear--;
      }
      currentDay += getDaysInMonth(currentMonth, currentYear);
    }
    else if (currentDay > getDaysInMonth(currentMonth, currentYear))
    {
      currentDay -= getDaysInMonth(currentMonth, currentYear);
      currentMonth++;
      if (currentMonth > 12)
      {
        currentMonth = 1;
        currentYear++;
      }
    }

    result[i] = malloc(6 * sizeof(char));
    sprintf(result[i], "%02d/%02d", currentDay, currentMonth);
  }

  return result;
}
void printTaskByTime(char *task, int width)
{
  printf("|");
  int length = strlen(task);
  int size = (width - length - 2) / 2;
  for (int i = 0; i < size; i++)
  {
    printf(" ");
  }
  printf("%s", task);
  for (int i = 0; i < size; i++)
  {
    printf(" ");
  }
  if ((width - length) % 2 != 0)
    printf(" ");
  printf("|");
}
void printLine(int n, int arr[7])
{

  for (int i = 0; i < WEEK_CELL_FIRST_COL_WIDTH; i++)
  {
    if (3 <= i && i <= 7)
    {
      if (n < 0)
      {
        printf("-");
      }
      else
      {
        printf("%02d:%02d", n, 0);
        i += 4;
      }
    }
    else
      printf("-");
  }

  for (int i = 0; i < WEEK_CELL_OTHER_COL_WIDTH * 7; i++) //
  {
    if (i % WEEK_CELL_OTHER_COL_WIDTH == 0 || i % WEEK_CELL_OTHER_COL_WIDTH == WEEK_CELL_OTHER_COL_WIDTH - 1)
    {
      printf("-");
    }
    else if (arr[i / (WEEK_CELL_OTHER_COL_WIDTH)] == 1)
    {
      printf(" ");
    }
    else
    {
      printf("-");
    }
  }

  printf("\n");
}
void processTasksForTime(int i, int no_tasks, struct Task *array_tasks, char **week, int *arr, bool first_line)
{
  char time[6];
  sprintf(time, "%02d:00", i);

  for (int l = 0; l < 7; l++)
  {
    for (int t = 0; t < no_tasks; t++)
    {

      char startingTime[6];
      char endingTime[6];
      char startingDay[3];
      char startingMonth[3];
      char startingDate[6];

      sscanf(array_tasks[t].time, "%5[^|]|%2[^/]/%2[^/]/%*d-%5[^|]", startingTime, startingDay, startingMonth, endingTime);
      sprintf(startingDate, "%s/%s", startingDay, startingMonth);
      int hour1, hour2;
      sscanf(startingTime, "%d:", &hour1);
      sscanf(endingTime, "%d:", &hour2);
      if (first_line)
      {

        if (i != -1 && hour2 > i && hour1 < i && strstr(startingDate, week[l + 1]) != NULL)
        {
          arr[l] = 1;
          break;
        }
      }
      else
      {
        char time2[6];
        sprintf(time2, "%02d:00", i + 1);
        if (i != -1 && hour2 > i + 1 && strstr(startingDate, week[l + 1]) != NULL && (i == hour1 || i + 1 > hour1))
        {
          arr[l] = 1;
          break;
        }
      }
    }
  }
}
int printWeekTime(struct Task *array_tasks, int no_tasks, char *date)
{
  char **week = WeekArray(date);
  char *Date[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

  for (int t = 0; t < no_tasks; t++)
  {
    if (!isTimeEven(array_tasks[t].time))
    {
      printUnsupportedTime(&array_tasks[t]);
      return t;
    }
  }

  for (int i = -1; i < 24; i++)
  {
    char time[6];
    int arr[7] = {0};
    sprintf(time, "%02d:00", i);

    if (i == -1)
    {
      printLine(i, arr);
    }
    else
    {
      processTasksForTime(i, no_tasks, array_tasks, week, arr, true);
      printLine(i, arr);
    }
    for (int j = 0; j < 2; j++)

    {

      for (int k = 0; k < 8; k++)
      {
        if (i == -1 && j == 0)
        {
          if (k == 0)
          {
            printTaskByTime(week[0], WEEK_CELL_FIRST_COL_WIDTH);
          }
          else
          {
            printTaskByTime(Date[k - 1], WEEK_CELL_OTHER_COL_WIDTH);
          }
        }
        else if (i == -1 && j == 1)
        {
          if (k == 0)
          {
            printTaskByTime("", WEEK_CELL_FIRST_COL_WIDTH);
          }
          else
          {
            printTaskByTime(week[k], WEEK_CELL_OTHER_COL_WIDTH);
          }
        }
        else if (k == 0)
        {
          printTaskByTime("", WEEK_CELL_FIRST_COL_WIDTH);
        }
        else if (k != 0)
        {

          bool hasTasks = false;

          for (int t = 0; t < no_tasks; t++)
          {
            char startingTime[6];
            char endingTime[6];
            char startingDay[3];
            char startingMonth[3];
            char startingDate[6];
            sscanf(array_tasks[t].time, "%5[^|]|%2[^/]/%2[^/]/%*d-%5[^|]", startingTime, startingDay, startingMonth, endingTime);
            sprintf(startingDate, "%s/%s", startingDay, startingMonth);

            if (strstr(startingTime, time) != NULL && strstr(startingDate, week[k]) != NULL && j == 0)
            {
              char timeStamp[16];
              sprintf(timeStamp, "#%d|%s-%s", array_tasks[t].num, startingTime, endingTime);
              printTaskByTime(timeStamp, WEEK_CELL_OTHER_COL_WIDTH);
              hasTasks = true;
              break;
            }
            else if (strstr(startingTime, time) != NULL && strstr(startingDate, week[k]) != NULL && j == 1)
            {
              if (strlen(array_tasks[t].title) <= WEEK_CELL_OTHER_COL_WIDTH - 2)
              {
                printTaskByTime(array_tasks[t].title, WEEK_CELL_OTHER_COL_WIDTH);
              }
              else
              {
                char contractedTitle[WEEK_CELL_OTHER_COL_WIDTH];
                snprintf(contractedTitle, sizeof(contractedTitle), "%.14s...", array_tasks[t].title);
                printTaskByTime(contractedTitle, WEEK_CELL_OTHER_COL_WIDTH);
              }
              hasTasks = true;
              break;
            }
          }

          if (!hasTasks)
          {
            printTaskByTime("", WEEK_CELL_OTHER_COL_WIDTH);
          }
        }
        if (k == 7)
        {
          printf("\n");
        }
      }
    }

    int arr2[7] = {0};
    char time2[6];
    sprintf(time2, "%02d:00", i);
    processTasksForTime(i, no_tasks, array_tasks, week, arr2, false);
    printLine(-1, arr2);
  }
  return -1;
}

void Todoapp()

{
  struct Task tasks[MAX_NO_TASKS];
  int no_tasks = 0;

  char command[MAX_LENGTH_COMMAND + 1];
  while (true)
  {

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
        if (addTask(tasks, no_tasks, title, description, time))
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
        if (deleteTask(tasks, no_tasks, num))
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
// void runTodoApp()
// {

//   char command[MAX_LENGTH_COMMAND + 1];
//   while (true)
//   {

//     fgets(command, MAX_LENGTH_COMMAND + 1, stdin);
//     command[strlen(command) - 1] = '\0';
//     enum CommandType commandType = getCommandType(command);
//     printf("Command : %s\n", command);
//     printf("Command type: %s\n", command_name[commandType]);
//     break;
//   }
// }

int main()
{

  Todoapp();
  return 0;
}
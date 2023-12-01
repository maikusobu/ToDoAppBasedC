/**
 * This code defines a task management system in C.
 * It includes functions for adding, editing, showing, and deleting tasks.
 * The tasks are stored in an array of structures, where each structure represents a task.
 * The code also includes helper functions for parsing commands and validating inputs.
 * The tasks can be filtered and displayed based on various criteria such as title, description, and status.
 * The maximum number of tasks that can be stored is defined by MAX_NO_TASKS.
 * The code also includes functions for printing tasks and checking the validity of time inputs.
 */
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

#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20
/**
 * @file todoapp.c
 * @brief This file contains the implementation of a todo application.
 */

/**
 * @enum Status
 * @brief Enumeration representing the status of a task.
 */
enum Status
{
  IN_PROGRESS, /**< Task is in progress */
  DONE,        /**< Task is done */
  ARCHIVED     /**< Task is archived */
};

/**
 * @var status_name
 * @brief Array of status names corresponding to the Status enumeration.
 */
char *status_name[] = {"In Progress", "Done", "Archived"};

/**
 * @enum CommandType
 * @brief Enumeration representing the type of command for the todo application.
 */
/**
 * @var command_name
 * @brief Array of command names corresponding to the CommandType enumeration.
 */
char *command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};

/**
 * @struct Task
 * @brief Structure representing a task in the todo application.
 */
struct Task
{
  int num;                                      /**< Task number */
  char title[MAX_LENGTH_TITLE + 1];             /**< Task title */
  char description[MAX_LENGTH_DESCRIPTION + 1]; /**< Task description */
  char time[MAX_LENGTH_TIME + 1];               /**< Task time */
  enum Status status;                           /**< Task status */
};

/**
 * @brief Prints the details of a task.
 * @param task Pointer to the task structure.
 */
void printTask(struct Task *task)
{
  printf("--------------------------------------------\n");
  printf("Num: #%d. Title: %s\n", task->num, task->title);
  printf("Description: %s\n", task->description);
  printf("Status: %s\n", status_name[task->status]);
  printf("--------------------------------------------\n");
}

/**
 * @brief Prints an error message for unsupported time with non-zero minutes.
 * @param task Pointer to the task structure.
 */
void printUnsupportedTime(struct Task *task)
{
  printf("----- Show week view -----\n");
  printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
  printf("In Task:\n");
  printTask(task);
}
/**
 * @brief Enumeration representing different command types.
 *
 * This enumeration defines the different types of commands that can be performed in the application.
 * The command types include ADD, EDIT, SHOW, DELETE, QUIT, and INVALID.
 */
enum CommandType
{
  ADD,    /**< Add command */
  EDIT,   /**< Edit command */
  SHOW,   /**< Show command */
  DELETE, /**< Delete command */
  QUIT,   /**< Quit command */
  INVALID /**< Invalid command */
};
/**
 * @brief Determines the type of command based on the given command string.
 *
 * @param command The command string.
 * @return enum CommandType The type of command.
 *         - ADD: If the command is "Add".
 *         - EDIT: If the command is "Edit".
 *         - SHOW: If the command is "Show".
 *         - DELETE: If the command is "Delete".
 *         - QUIT: If the command is "Quit".
 *         - INVALID: If the command is not recognized.
 **/
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

/**
 * Extracts the title from the given command string.
 * The command string should be in the format "[title] additional text".
 * The extracted title is stored in the out_title parameter.
 *
 * @param command The command string containing the title.
 * @param out_title The output buffer to store the extracted title.
 */
void getTitleFromAdd(char *command, char *out_title)
{
  char *start = strchr(command, '[') + 1;
  char *end = strchr(start, ']');
  strncpy(out_title, start, end - start);
  out_title[end - start] = '\0';
}

/**
 * Extracts the description from the given command string.
 * The command string should be in the format "[command][description]".
 * The extracted description is stored in the out_description parameter.
 *
 * @param command The command string containing the description.
 * @param out_description The output buffer to store the extracted description.
 */
void getDescriptionFromAdd(char *command, char *out_description)
{
  char *start = strchr(strchr(command, '[') + 1, '[') + 1;
  char *end = strchr(start, ']');
  strncpy(out_description, start, end - start);
  out_description[end - start] = '\0';
}
/**
 * Extracts the time from the given command and stores it in the output string.
 *
 * @param command The command string containing the time in square brackets.
 * @param out_time The output string to store the extracted time.
 */
void getTimeFromAdd(char *command, char *out_time)
{
  char *start = strchr(strchr(strchr(command, '[') + 1, '[') + 1, '[') + 1;
  char *end = strchr(start, ']');
  strncpy(out_time, start, end - start);
  out_time[end - start] = '\0';
}
/**
 * Checks if a character is a space.
 *
 * @param c The character to check.
 * @return true if the character is a space, false otherwise.
 */
bool is_space(char c)
{
  return c == ' ';
}

/**
 * Checks if a character is alphanumeric.
 *
 * @param c The character to check.
 * @return true if the character is alphanumeric, false otherwise.
 */
bool is_alphanumeric(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/**
 * Checks the validity of a title string.
 *
 * @param raw_title The title string to be checked.
 * @return If the title is valid, returns -1. If the title length exceeds 100 characters, returns the length of the title.
 *         If the title starts or ends with a space character, returns 0. If the title contains an invalid character,
 *         returns the index of the first invalid character.
 */
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
    if (!(is_alphanumeric(raw_title[i]) || strchr(" ,.-:|/", raw_title[i]) != NULL))
    {
      return i;
    }
  }
  return -1;
}
/**
 * Checks the validity of a description.
 *
 * This function checks if the given description meets the required criteria.
 * The description should not exceed 200 characters in length and should not
 * start or end with a space. Additionally, it should only contain alphanumeric
 * characters, spaces, and certain special characters (, . - : | /).
 *
 * @param raw_description The description to be checked.
 * @return If the description is valid, returns -1. If the description exceeds
 *         the maximum length, returns the length of the description. If the
 *         description starts or ends with a space, returns 0. If the description
 *         contains an invalid character, returns the index of the first invalid
 *         character.
 */
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

    if (!(is_alphanumeric(raw_description[i]) || strchr(" ,.-:|/", raw_description[i]) != NULL))
    {
      return i;
    }
  }
  return -1;
}
/**
 * @brief Checks if a given year is a leap year.
 *
 * @param year The year to be checked.
 * @return int Returns 1 if the year is a leap year, 0 otherwise.
 */
int isLeapYear(int year)
{
  return year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);
}

/**
 * @brief Checks the validity of a time range.
 *
 * This function checks if the given time range is valid, including the hours, minutes, days, months, and years.
 *
 * @param raw_time The raw time string in the format "hour1:minute1|day1/month1/year1-hour2:minute2|day2/month2/year2".
 * @return int Returns 0 if the time range is valid, otherwise returns an error code.
 *         The error codes are as follows:
 *         - 1100 + hour1: Invalid hour1 value.
 *         - 2100 + minute1: Invalid minute1 value.
 *         - 3100 + day1: Invalid day1 value.
 *         - 4100 + month1: Invalid month1 value.
 *         - 510000 + year1: Invalid year1 value.
 *         - 1200 + hour2: Invalid hour2 value.
 *         - 2200 + minute2: Invalid minute2 value.
 *         - 3200 + day2: Invalid day2 value.
 *         - 4200 + month2: Invalid month2 value.
 *         - 520000 + year2: Invalid year2 value.
 */
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

/**
 * @brief Parses the edit command and returns the corresponding status.
 *
 * This function takes an edit command as input and extracts the status from it.
 * The edit command should be in the format "[status]", where status can be one of the following:
 * - "I" for in progress
 * - "D" for done
 * - "A" for archived
 *
 * @param edit_cmd The edit command string.
 * @return The corresponding status enum value.
 *         - IN_PROGRESS if the status is "I"
 *         - DONE if the status is "D"
 *         - ARCHIVED if the status is "A"
 *         - INVALID if the status is not recognized.
 */
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

bool addTask(struct Task *array_tasks, int no_tasks, char *new_title, char *new_description, char *new_time)
{

  if (no_tasks >= MAX_NO_TASKS)
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
/**
 * @brief Prints a space character.
 */
void printSpace()
{
  printf(" ");
}

/**
 * @brief Prints a hyphen character.
 */
void printHyphen()
{
  printf("-");
}

/**
 * @brief Checks if the given time string has all even minutes.
 *
 * @param time The time string in the format "HH:MM|HH:MM|HH:MM|...".
 * @return 1 if all minutes are even, 0 otherwise.
 */
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

/**
 * @brief Gets the day number corresponding to the given day abbreviation.
 *
 * @param day The day abbreviation (e.g., "mon", "tue", etc.).
 * @return The day number (1 for Monday, 2 for Tuesday, etc.), or -1 if the day abbreviation is invalid.
 */
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

/**
 * @brief Gets the number of days in the given month of the given year.
 *
 * @param month The month number (1 for January, 2 for February, etc.).
 * @param year The year.
 * @return The number of days in the month, or -1 if the month is invalid.
 */
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

/**
 * @brief Generates an array of dates for a given input date.
 *
 * This function takes a date in the format "day/date/month/year" and generates an array of dates for the week starting from that date.
 * The generated array includes the input date and the next 7 consecutive dates.
 *
 * @param input The input date in the format "day/date/month/year".
 * @return char** An array of dates for the week starting from the input date.
 *         The first element of the array is the year in string format.
 *         The remaining elements of the array are the dates in the format "dd/mm".
 *         The array has a size of 8.
 *         The caller is responsible for freeing the memory allocated for the array.
 */
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
/**
 * Prints a task with a specified width, centered within a cell.
 *
 * @param task The task to be printed.
 * @param width The width of the cell.
 */
void printTaskByTime(char *task, int width)
{
  // Print left border of the cell
  printf("|");

  // Calculate the length of the task
  int length = strlen(task);

  // Calculate the number of spaces to be printed before the task
  int size = (width - length) / 2;

  // Print spaces before the task
  for (int i = 0; i < size; i++)
  {
    printSpace();
  }

  // Print the task
  printf("%s", task);

  // Calculate the number of spaces to be printed after the task
  int remainingSize = width - length - size;

  // Print spaces after the task
  for (int i = 0; i < remainingSize; i++)
  {
    printSpace();
  }

  // Print right border of the cell
  printf("|");
}

/**
 * Prints a line of the week schedule table.
 *
 * @param n The hour value to be printed in the first column.
 * @param arr An array representing the availability of each day in the week.
 */
void printLine(int n, int arr[7])
{
  // Calculate the number of hyphens to be printed before the first column
  int p = (WEEK_CELL_FIRST_COL_WIDTH - 5) / 2;

  // Print hyphens before the first column
  printHyphen();
  for (int i = 0; i < p; i++)
  {
    printHyphen();
  }

  int k = 0;
  if (n >= 0)
  {
    // Print the hour value in the first column
    printf("%02d:%02d", n, 0);
    k = 5;
  }

  // Calculate the number of hyphens to be printed after the first column
  int remainingHyphens = WEEK_CELL_FIRST_COL_WIDTH - k - p;

  // Print hyphens after the first column
  for (int i = 0; i < remainingHyphens; i++)
  {
    printHyphen();
  }

  // Print hyphens for each day in the week
  for (int j = 0; j < 7; j++)
  {
    printHyphen();
    for (int i = 0; i < WEEK_CELL_OTHER_COL_WIDTH; i++)
    {
      if (arr[j] == 1)
      {
        printSpace();
      }
      else
        printHyphen();
    }
    printHyphen();
  }

  // Move to the next line
  printf("\n");
}
/**
 * @brief Processes tasks for a specific time slot.
 *
 * This function checks if there are any tasks scheduled for a specific time slot.
 * It iterates through the array of tasks and compares the starting and ending times
 * with the given time slot. If a task is found within the time slot, the corresponding
 * element in the 'arr' array is set to 1.
 *
 * @param i The time slot to process (in hours). Set to -1 to process all time slots.
 * @param no_tasks The number of tasks in the 'array_tasks' array.
 * @param array_tasks An array of Task structures containing task information.
 * @param week An array of strings representing the days of the week.
 * @param arr An array of integers to store the results. Each element represents a day of the week.
 * @param first_line Flag indicating if it's the first line of time slots.
 */

void processTasksForTime(int i, int no_tasks, struct Task *array_tasks, char **week, int *arr, bool first_line)
{
  char time[6];
  sprintf(time, "%02d:00", i);

  for (int l = 0; l < 7; l++)
  {
    char full_date[11];
    sprintf(full_date, "%s/%s", week[l + 1], week[0]);
    for (int t = 0; t < no_tasks; t++)
    {

      char startingTime[6];
      char endingTime[6];
      char startingDay[3];
      char startingMonth[3];
      char startingDate[11];

      char startingYear[5];
      sscanf(array_tasks[t].time, "%5[^|]|%2[^/]/%2[^/]/%4[^-]-%5[^|]", startingTime, startingDay, startingMonth, startingYear, endingTime);
      sprintf(startingDate, "%s/%s/%s", startingDay, startingMonth, startingYear);
      int hour1, hour2;
      sscanf(startingTime, "%d:", &hour1);
      sscanf(endingTime, "%d:", &hour2);
      if (first_line)
      {

        if (i != -1 && hour2 > i && hour1 < i && strstr(startingDate, full_date) != NULL)
        {
          arr[l] = 1;
          break;
        }
      }
      else
      {
        char time2[6];
        sprintf(time2, "%02d:00", i + 1);
        if (i != -1 && hour2 > i + 1 && strstr(startingDate, full_date) != NULL && (i == hour1 || i + 1 > hour1))
        {
          arr[l] = 1;
          break;
        }
      }
    }
  }
}
/**
 * Prints the weekly schedule for tasks based on the given date.
 *
 * @param array_tasks An array of Task structures representing the tasks.
 * @param no_tasks The number of tasks in the array.
 * @param date The date for which the weekly schedule is to be printed.
 * @return The index of the first task with unsupported time format, or -1 if all tasks have supported time format.
 */
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
            char startingDate[11];
            char startingYear[5];
            sscanf(array_tasks[t].time, "%5[^|]|%2[^/]/%2[^/]/%4[^-]-%5[^|]", startingTime, startingDay, startingMonth, startingYear, endingTime);

            sprintf(startingDate, "%s/%s/%s", startingDay, startingMonth, startingYear);
            char full_date[11];
            sprintf(full_date, "%s/%s", week[k], week[0]);
            sprintf(startingDate, "%s/%s/%s", startingDay, startingMonth, startingYear);

            if (strstr(startingTime, time) != NULL && strstr(startingDate, full_date) != NULL && j == 0)
            {
              char timeStamp[16];
              sprintf(timeStamp, "#%d|%s-%s", array_tasks[t].num, startingTime, endingTime);
              printTaskByTime(timeStamp, WEEK_CELL_OTHER_COL_WIDTH);
              hasTasks = true;
              break;
            }
            else if (strstr(startingTime, time) != NULL && strstr(startingDate, full_date) != NULL && j == 1)
            {
              if (strlen(array_tasks[t].title) <= WEEK_CELL_OTHER_COL_WIDTH - 2)
              {
                printTaskByTime(array_tasks[t].title, WEEK_CELL_OTHER_COL_WIDTH);
              }
              else
              {
                char contractedTitle[WEEK_CELL_OTHER_COL_WIDTH + 1];
                if (strlen(array_tasks[t].title) > WEEK_CELL_OTHER_COL_WIDTH)
                {
                  snprintf(contractedTitle, WEEK_CELL_OTHER_COL_WIDTH - 2, "%s", array_tasks[t].title);
                  strcat(contractedTitle, "...");
                }
                else
                {
                  snprintf(contractedTitle, WEEK_CELL_OTHER_COL_WIDTH + 1, "%s", array_tasks[t].title);
                }
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

void sampleTc21()
{
  printf("----- Sample testcase 21 -----\n");
  printf("Test printWeekTime:\n");
  struct Task array_tasks[5] = {
      {1, "Course Intro to Programming", "Room 701-H6 - orange", "07:00|16/10/2023-09:00|16/10/2023", IN_PROGRESS},
      {2, "Meeting Project Apple", "Room 701-H6 - apple", "08:00|17/10/2023-09:00|17/10/2023", IN_PROGRESS},
      {3, "Meeting Project Banana", "Room 701-H6 - orange", "07:00|19/10/2023-08:00|19/10/2023", IN_PROGRESS},
      {4, "Do an Tong hop - TTNT", "Room 701-H6 - orange", "09:00|21/10/2023-10:00|21/10/2023", IN_PROGRESS},
      {5, "Course Intro to Programming - apple", "Room 701-H6 - banana", "07:00|01/10/2023-12:00|01/10/2023", DONE},
  };
  int no_tasks = 5;
  char date[] = "wed/18/10/2023";
  int code = printWeekTime(array_tasks, no_tasks, date);
  if (code != -1)
  {
    printf("Unsupported task at index: %d\n", code);
  }
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
int main()
{
  Todoapp(); // Run todo
}

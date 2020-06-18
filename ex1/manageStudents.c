#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS_AMOUNT 5000
#define ARGS_NUM 6
#define LOWEST_GRADE 0
#define MAX_GRADE 100
#define LOWEST_AGE 18
#define HIGHEST_AGE 120
#define LOWEST_ID 1000000000
#define HIGHEST_ID 10000000000
#define MAXIMUM_LINE_LENGTH 151
#define MAX_PARAM_SIZE 41
#define FUNCTION_SUCCESS 0
#define FUNCTION_FAILED 1
#define END_OF_INPUT 0
#define DECIMAL 10

// ASCII defines
#define ASCII_FOR_0 48
#define ASCII_FOR_9 57
#define TAB_IN_ASCII 9
#define Z_ASCII 90
#define A_ASCII 65
#define LOWERCAST_A_ASCII 97
#define LOWERCAST_Z_ASCII 122
#define DASH_ASCII 45
#define SPACEBAR_ASCII 32
#define ENDOFLINE_ASCII 10

/**
 * A struct that represents a student.
 */
struct student
{
    int age, grade;
    unsigned long id;
    char name[MAX_PARAM_SIZE];
    char country[MAX_PARAM_SIZE];
    char city[MAX_PARAM_SIZE];
    float rating;
    int highestRating;
};

/**
 * a method that checks if a given str is a num or if it has letters or signs in him.
 * @param str a string pointer.
 * @return 0 if true, 1 otherwise
 */
int checkNum(char const *str)
{
    for (int i = 0; i < MAX_PARAM_SIZE; i++)
    {
        if ((int) str[i] == END_OF_INPUT)
        {
            return FUNCTION_SUCCESS;
        }
        if (((int) str[i] >= ASCII_FOR_0) && ((int) str[i] <= ASCII_FOR_9))
        {
            continue;
        }
        return FUNCTION_FAILED;
    }
    return FUNCTION_SUCCESS;
}

/**
 * a method that checks if a given str is word made of letters and '-' or if it has numbers and
 * other signs.
 * if isName it can get spacebars too.
 * @param str a string pointer.
 * @param isName if the check string is a name equals 1, 0 otherwise.
 * @return 0 if true, 1 otherwise
 */
int checkStr(char const *str, int isName)
{
    for (int i = 0; i < MAX_PARAM_SIZE; i++)
    {
        if ((int) str[i] == END_OF_INPUT)
        {
            return FUNCTION_SUCCESS;
        }
        if (((int) str[i] >= A_ASCII) && ((int)str[i] <= Z_ASCII))
        {
            continue;
        }
        if (((int) str[i] >= LOWERCAST_A_ASCII) && ((int) str[i] <= LOWERCAST_Z_ASCII))
        {
            continue;
        }
        if ((int) str[i] == DASH_ASCII)
        {
            continue;
        }
        if ((isName) && ((int) str[i] == SPACEBAR_ASCII))
        {
            continue;
        }
        return FUNCTION_FAILED;
    }
return FUNCTION_SUCCESS;
}

/**
 * a function that checks all the if arguments given from the input are correct, according
 * to the instructions. if they are correct return 1, else return 0.
 * @param studentList a list of Struct student.
 * @param studentNum the current student in the studentList index.
 * @param tempid a temporary id string.
 * @param tempage a temporary age string.
 * @param tempgrade a temporary grade string.
 * @param lineNum the number of line the command came from.
 * @return 1 if the arguments are correct according to the instructions, false otherwise.
 */
int checkArgs(struct student studentList[], int studentNum, char *tempid, char *tempage, char *tempgrade, int lineNum)
{
    if (!checkNum(tempid) && !checkNum(tempage) && !checkNum(tempgrade))
    {
        studentList[studentNum].age = (int) strtol(tempage, (char **)NULL, DECIMAL);
        studentList[studentNum].grade = (int) strtol(tempgrade, (char **)NULL, DECIMAL);
        studentList[studentNum].id = (unsigned long) strtol(tempid, (char **)NULL, DECIMAL);
    }
    else
    {
        printf("ERROR: grade, age and id contain only digits,"
               " got a non digit.\nin line %d\n", lineNum);
        return FUNCTION_FAILED;
    }
    if ((studentList[studentNum].grade > MAX_GRADE) || (studentList[studentNum].grade < LOWEST_GRADE))
    {
        printf("ERROR: grade must be higher or equal "
               "to 0 and lower or equal to 100.\nin line %d\n",
               lineNum);
        return FUNCTION_FAILED;
    }
    if ((studentList[studentNum].age < LOWEST_AGE) || (studentList[studentNum].age > HIGHEST_AGE))
    {
        printf("ERROR: age must be higher or equal "
               "to 18 and lower or equal to 120.\nin line %d\n", lineNum);
        return FUNCTION_FAILED;
    }
    if ((LOWEST_ID > studentList[studentNum].id) || (studentList[studentNum].id >= HIGHEST_ID))
    {
        printf("ERROR: bad id, id must contain 10 digits,"
               " cannot start with 0.\nin line %d\n", lineNum);
        return FUNCTION_FAILED;
    }
    if (checkStr(studentList[studentNum].name, FUNCTION_FAILED))
    {
        printf("ERROR: bad name, name can only contain "
               "letters and '-' and spacebars.\nin line %d\n", lineNum);
        return FUNCTION_FAILED;
    }
    if (checkStr(studentList[studentNum].country, FUNCTION_SUCCESS))
    {
        printf("ERROR: bad country, country can "
               "only contain letters and '-'.\nin line %d\n", lineNum);
        return FUNCTION_FAILED;
    }
    if (checkStr(studentList[studentNum].city, FUNCTION_SUCCESS))
    {
        printf("ERROR: bad city, city can only contain "
               "letters and '-'.\nin line %d\n", lineNum);
        return FUNCTION_FAILED;
    }
    return FUNCTION_SUCCESS;
}

/**
 * a function that counts the number of tabs in a given string.
 * @param buffer the string to count the tabs in.
 * @return the number of tabs in the string.
 */
int tabCounter(char const *buffer)
{
    int tabs = FUNCTION_SUCCESS;
    for (int i = FUNCTION_SUCCESS; i < MAXIMUM_LINE_LENGTH; i++)
    {
        if ((int)buffer[i] == ENDOFLINE_ASCII)
        {
            return tabs;
        }
        if ((int) buffer[i] == TAB_IN_ASCII)
        {
            tabs++;
        }
    }
    return tabs;
}

/**
 * A function that asks for the user's input of the students, continue to ask for
 * students until the user enters "q".
 * the function also checks that the given inputs are correct.
 * @param studentList a list of Struct students.
 * @return the number of students added to the list from the user.
 */
int getStudentsInput(struct student *studentList)
{
    int studentNum = 0;
    int highestRate = 0;
    int lineNum = -1;
    char buffer[MAXIMUM_LINE_LENGTH];
    char tempId[MAX_PARAM_SIZE], tempgrade[MAX_PARAM_SIZE], tempage[MAX_PARAM_SIZE];
    while ((strcmp(&buffer[0], "q\r\n") != 0) || ((strcmp(&buffer[0], "q\n") != 0)))
    {
        printf("Enter student info. To exit press q, then enter\n");
        fgets(buffer, MAXIMUM_LINE_LENGTH, stdin);
        lineNum++;
        if (((strcmp(&buffer[0], "q\r\n") == 0)) || (strcmp(&buffer[0], "q\n") == 0))
        {
            break;
        }
        int fieldsScanned = sscanf(buffer, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]",
                                   tempId, studentList[studentNum].name,
                                   tempgrade, tempage,
                                   studentList[studentNum].country, studentList[studentNum].city);
        if ((fieldsScanned != ARGS_NUM) || (tabCounter(buffer) != ARGS_NUM))
        {
            printf("ERROR: bad number of inputs, or missing tab.\nin line %d\n", lineNum);
            continue;
        }
        if (checkArgs(studentList, studentNum, tempId, tempage, tempgrade, lineNum))
        {
            continue;
        }
        studentList[studentNum].rating = ((float) studentList[studentNum].grade) / ((float)studentList[studentNum].age);
        if (studentList[highestRate].rating < studentList[studentNum].rating)
        {
            highestRate = studentNum;
            studentList->highestRating = studentNum;
        }
        studentNum++;
    }
    return studentNum;
}

/**
 * merges two subarrays of arr[].
 * First subarray is arr[l..m]
 * Second subarray is arr[m+1..r]
 * @param studentList a list of struct students
 * @param start the left array index
 * @param mid the middle array index
 * @param last the right array index
 */
void merge(struct student *studentList, int start, int mid, int last)
{
    int pivot = mid + 1;

    // checks if already sorted.
    if (studentList[mid].grade <= studentList[pivot].grade)
    {
        return;
    }

    // Two pointers to maintain start of both arrays to merge
    while (start <= mid && pivot <= last)
    {

        // If no need to replace.
        if (studentList[start].grade <= studentList[pivot].grade)
        {
            start++;
        }
        else
        {
            struct student value = studentList[pivot];
            int index = pivot;

            // making the replacement of the algorithm.
            while (index != start)
            {
                studentList[index] = studentList[index - 1];
                index--;
            }
            studentList[start] = value;

            // Updates pointers
            start++;
            mid++;
            pivot++;
        }
    }
}

/**
 * The known mergeSort function, this time used to sort a list of struct students by grade.
 * this function uses the merge function
 * @param studentList a list of struct students
 * @param first the left array index.
 * @param last the right array index.
 */
void mergeSort(struct student *studentList, int first, int last)
{
    if (first < last)
    {
        // avoiding overflow.
        int mid = first + (last - first) / 2;

        mergeSort(studentList, first, mid);
        mergeSort(studentList, mid + 1, last);

        merge(studentList, first, mid, last);
    }
}

/**
 * The known quicksort function, this time used to sort a list of struct students by name.
 * this function uses the merge function
 * @param studentList a list of struct students
 * @param l the left array index.
 * @param r the right array index.
 */
void quicksort(struct student *studentList, int first, int last)
{
    int i, j, pivot;
    struct student temp;
    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;
        while (i < j)
        {
            int comp = strcmp(studentList[i].name, studentList[pivot].name);
            while (comp <= 0 && i < last)
            {
                i++;
                comp = strcmp(studentList[i].name, studentList[pivot].name);
            }
            comp = strcmp(studentList[j].name, studentList[pivot].name);
            while (comp > 0)
            {
                j--;
                comp = strcmp(studentList[j].name, studentList[pivot].name);
            }
            if (i < j)
            {
                temp = studentList[i];
                studentList[i] = studentList[j];
                studentList[j] = temp;
            }
        }
        temp = studentList[pivot];
        studentList[pivot] = studentList[j];
        studentList[j] = temp;
        quicksort(studentList, first, j - 1);
        quicksort(studentList, j + 1, last);
    }
}

/**
 * a method that gets the user's input and then sort it according to the
 * user's input in command line.
 * @param quick 1 if the user used quick, 0 if the user wrote merge.
 * @return 0 upon success, 1 otherwise.
 */
void sortInputs(int quick)
{
    struct student studentList[MAX_STUDENTS_AMOUNT];
    int studentNum = getStudentsInput(studentList);
    if (quick == 1)
    {
        quicksort(studentList, 0, studentNum - 1);
    }
    else
    {
        mergeSort(studentList, 0, studentNum - 1);
    }
    for (int i = 0; i < studentNum; i++)
    {
        printf("%ld\t%s\t%d\t%d\t%s\t%s\t\n", studentList[i].id,
               studentList[i].name, studentList[i].grade, studentList[i].age,
               studentList[i].country, studentList[i].city);
    }
}

/**
 * A function that gets the user's input for students, and returns and prints the one with the
 * highest grade/age.
 * @return
 */
void bestStudent()
{
    struct student studentList[MAX_STUDENTS_AMOUNT];
    int studentNum = getStudentsInput(studentList);
    int highestRate = studentList->highestRating;
    if (studentNum == 0)
    {
        return;
    }
    printf("best student info is: %ld\t%s\t%d\t%d\t%s\t%s\t\n",
           studentList[highestRate].id, studentList[highestRate].name,
           studentList[highestRate].grade, studentList[highestRate].age,
           studentList[highestRate].country, studentList[highestRate].city);
}

/**
 * the main function, gets arguments from the command line and chooses what to do
 * by the input.
 * @param argc num of arguments.
 * @param argv char array of the arguments.
 * @return 0 upon success, 1 otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("USAGE: please use 1 of the given options: best, merge or quick");
        return FUNCTION_FAILED;
    }
    if (strcmp(argv[1], "best") == 0)
    {
        bestStudent();
        return FUNCTION_SUCCESS;
    }
    if (strcmp(argv[1], "merge") == 0)
    {
        sortInputs(0);
        return FUNCTION_SUCCESS;
    }
    if (strcmp(argv[1], "quick") == 0)
    {
        sortInputs(1);
        return FUNCTION_SUCCESS;
    }
    printf("USAGE: please use 1 of the given options: best, merge or quick");
    return FUNCTION_FAILED;
}


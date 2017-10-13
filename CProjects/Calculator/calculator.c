#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STRING 256

int tokenise_ops(char buffer[], int startingIndex, char result[], char ops[]);
double compute(char **stringArray, int stringCounter, char ops[]);
struct stack *createSpace(int size);
void push(struct stack *theStack, double value);
double pop(struct stack *theStack);
int computeWithOperator(char ops[], char nextChar, struct stack *theStack);
char **changeToPostFix(char ops[], char **stringArray, int stringCharacter, int *arrayCounter);
int checkPrecedence(char character);
double get(struct stack *theStack, int x);

struct stack{
    double * pointer;
    int top;
};

int main(){
    char buffer[MAX_STRING];
    memset(buffer, 0, MAX_STRING-1);
    fgets(buffer, MAX_STRING, stdin);

    char result[MAX_STRING];
    int start;
    char ops[] = {'+', '-', '*', '/', '^'};

    char **stringArray;
    stringArray = malloc(sizeof(char *) * MAX_STRING);
    int stringCounter = 0;

    start = tokenise_ops(buffer, 0, result, ops);

while(start != -1) {
    stringArray[stringCounter] = malloc(sizeof(char) * MAX_STRING);
    strcpy(stringArray[stringCounter], result);
    stringCounter++;
    memset(result, 0, MAX_STRING-1);
    start = tokenise_ops(buffer, start, result, ops);
}

    int *arrayIndex = malloc(sizeof(int) * 8); //doesnt need big amount of memory.... It's just an index.
    stringArray = changeToPostFix(ops, stringArray, stringCounter, arrayIndex);

    double actualResult = compute(stringArray, (*arrayIndex),  ops);

    free(stringArray); // free memory that was malloc'd.
    free(arrayIndex);
    printf(" =  %f\n", actualResult);
    return 0;
}

// changes tokenised input from infix to postfix (RPN, reversed polish notation)
// char ops[] : array of allowed operators, char **stringArray : tokenised input in 2d pointer array format, int stringCounter : how large stringArray[] is, int *arrayIndex : index for **realArrayString. Incrementing after each token is proccessed.
// returns buffer string in postfix format.
char **changeToPostFix(char ops[], char **stringArray, int stringCounter, int *arrayIndex) {
    char **realStringArray = malloc(sizeof(char *) * MAX_STRING);
    struct stack *theStack = createSpace(MAX_STRING);
    (*arrayIndex) = 0;
    for(int i = 0; i < stringCounter; i++) {
        realStringArray[(*arrayIndex)] = malloc(sizeof(char) * 50);
        if((stringArray[i][0] <= '9' && stringArray[i][0] >= '0') || stringArray[i][0] == '.') {
            int j = 0;
            while(stringArray[i][j] != 0x00) {
                realStringArray[(*arrayIndex)][j] = stringArray[i][j];
                j++;
            }
            (*arrayIndex)++;
        }
        else if(stringArray[i][0] == '(') {
            push(theStack, '(');
        }
        //basically if(nextCharacter == anyOf(+, -, *, /, ^))
        else if(stringArray[i][0] == ops[0] || stringArray[i][0] == ops[1] || stringArray[i][0] == ops[2] || stringArray[i][0] == ops[3] || stringArray[i][0] == ops[4]) {
            int head = theStack->top;
            char token = get(theStack, theStack->top);
            while((checkPrecedence(token) > checkPrecedence(stringArray[i][0])) && head > 0) {
                token = pop(theStack);
                realStringArray[(*arrayIndex)][0] = token;
                (*arrayIndex)++;
                realStringArray[*(arrayIndex)] = malloc(sizeof(char) * 50);
                token = get(theStack, theStack->top);
                head--;
            }
            push(theStack, stringArray[i][0]);
        }
        else if(stringArray[i][0] == ')'){
            while(get(theStack, theStack->top) != '(') {
                char token = pop(theStack);
                realStringArray[*arrayIndex][0] = token;
                (*arrayIndex)++;
             }
             pop(theStack);
        }
    }


    /*
    Anything that is left on the stack, gets popped into realStringArray.
    */
    int head = theStack->top;
    while(head > 0) {
        realStringArray[(*arrayIndex)] = malloc(sizeof(char) * 50 );
        char token = pop(theStack);
        realStringArray[(*arrayIndex)][0] = token;
        (*arrayIndex)++;
        head--;
    }

    (*arrayIndex)--;    // decrement arrayIndex, because of the last increment ((*arrayIndex)++), int eh last while loop.
    free(theStack);
    return realStringArray;
}

// Checks the precedence of an operator.
// char character: character who's precedence is going to be checked.
// returns precedence value.
int checkPrecedence(char character) {
    if(character == '+' || character == '-') {
        return 1;
    }
    else if(character == '*' || character == '/') {
        return 2;
    }
    else if(character == '^') {
        return 3;
    }    else   return -1;          // These are for '(' and ')'
}

// Creates necessary space in a stack.
// Increment afterwards (IA)... full Ascending stack.
// int size : size of stack.
// returns struct stack pointer.
struct stack *createSpace(int size) {
    struct stack *theStack;
    theStack = (struct stack*) malloc(sizeof(struct stack));
    theStack->pointer = malloc(size * sizeof(double));
    theStack->top = 0;
    return theStack;
}

// Pushs double values onto the stack.
// struct stack *theStack : stack pointer pointing to space in memory. double value : value to push.
void push(struct stack *theStack, double value) {
    double *array = theStack->pointer;
    int *currentTop = &(theStack->top);
    array[*currentTop] = value;
    (*currentTop)++;
}

// Pops int values off the stack.
// struct stack *theStack : stack pointer pointing to space in memory.
// returns double value popped from stack.
double pop(struct stack *theStack) {
    double *array = theStack->pointer;
    int *currentTop = &(theStack->top);
    (*(currentTop))--;
    double value = array[*currentTop];
    return value;
}

// get value at position x.
// struct stack *theStack : stack pointer pointing to space in memory, int x : position in stack to check.
// return value at position x.
double get(struct stack *theStack, int x) {
    double *array = theStack->pointer;
    return *(array + --x);
}

// computes the sum passed as a parameter(char **stringArray). Condisering that the sum is in RPN (reverse polish notation) or Postfix notation.
// char **stringArray :
// returns double value... the result of the computation.
double compute(char **stringArray, int stringCounter, char ops[]) {
    struct stack * theStack = createSpace(MAX_STRING);
    for(int i = 0; i <= stringCounter; i++) {
        int j = 0;
        char nextChar = stringArray[i][0]; // + 0 for the first element.
        if(computeWithOperator(ops, nextChar, theStack) == -1) {    // if(character == isIn(+, -, *, /, ^))
            double num = atof(stringArray[i]); // atof() function returns a double value from a character array[].
            push(theStack, num);
        }
    }
    double result = pop(theStack);
    free(theStack);  // free space.
    return result;
}

// Checks to see if the operator/character passed is an operator. If it is one of the 5 operators, then pop two values off the stack to operate on.
// char ops[] : array of operators which are accepted, char nextChar : the operator/character, struct stack *theStack : the stack from which to pop next 2 values to operate on if nextChar is one of the 5 operators.
// return the value of the computation. else return -1 if nextChar was not one of the 5 allowed operators.
int computeWithOperator(char ops[], char nextChar, struct stack *theStack) {
    if(nextChar == ops[0] || nextChar == ops[1] || nextChar == ops[2] || nextChar == ops[3] || nextChar == ops[4]) {
        double num1 = pop(theStack);
        double num2 = pop(theStack);
        double result;
        if(nextChar == '+') {
            result = num2 + num1;
        }   else if(nextChar == '-') {
            result = num2 - num1;
        }   else if(nextChar == '/') {
            result = num2 / num1;
        }   else if(nextChar == '*') {
            result = num2 * num1;
        }   else if(nextChar == '^') {
        result = pow(num2, num1);
    }
    push(theStack, result);
        return 1;  //returns 1 if the operator was accepted and the sum was calculated.
    }   else    {
        return -1; //returns -1 if the character wasnt one of the allowed operators.
    }
}

// tokenises the input from the users input.
// char buffer[] : input stream string, int startingIndex : index as to where to start the tokenizing function, char result[] : array that stores the resulting token, char ops[] : array of operators
// returns next index to start tokeninsing, else if next character is NULL or array index exceeds MAX_STRING then return -1.
int tokenise_ops(char buffer[], int startingIndex, char result[], char ops[]) {
    int nextPosition = startingIndex;
    int foundPosition = 0;
    while(nextPosition < MAX_STRING && foundPosition == 0){
        char character = buffer[nextPosition];
        // basically, if(nextCharacter == ifIn(+, -, *, /, ^))
        if(character == ops[0] || character == ops[1] || character == ops[2] || character == ops[3] || character == ops[4] || character == ')' || character == '(') {
            foundPosition = 1;
            result[0] = character;
            nextPosition++;
        }   else if((character >= '0' && character <= '9') || character == '.') {
            int i = 0;
            while((character >= '0' && character <= '9') || character == '.') {
                result[i] = character;
                i++;
                nextPosition++;
                character = buffer[nextPosition];
            }
            foundPosition = 1;
        }   else    {
            nextPosition++;
        }
    }
    if(nextPosition >= MAX_STRING || buffer[nextPosition] == 0x00) { 
        return - 1;     // return -1 if if the next character is NULL(0x00) or the index exceeds the buffer length size.
    }   else    {
        return nextPosition;        // else return next position.
    }
}

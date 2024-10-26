
///////------//lecture 7 - intro to functions//------///////

/*
 this is a small top down design example
 we need to create a quiz of adding or subtracting 2 numbers
 the user need to input the answer
    and we will display the score at the end
*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()

// using section
using ::std::cout;
using ::std::endl;
// const section
 const int TOTAL_QUESTIONS = 5;
// prototypes section
void startQuiz();
void generateQuestion(int &num1, int &num2, char &operation);
void presentQuestion(int num1, int num2, char operation);
int getUserAnswer();
bool checkAnswer(int userAnswer, int correctAnswer);
void displayScore(int score, int totalQuestions);
int correctAnsweris(int num1, int num2, char operation);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
    // we need to create a seed ranodmly
    srand(time(0)); // Seed the random number generator

    //we will start the quiz
    startQuiz();
   
   
    return (EXIT_SUCCESS);

}
void startQuiz()
{

    int score = 0;
    //loop for every question
    for (int i = 0; i < TOTAL_QUESTIONS; i++) {
        int num1, num2, correctAnswer;
        char operation; // the only thing you havnt learn
        //char is a single character , for example 'a' or 'b'.

        //create a random question
        generateQuestion(num1, num2, operation);
        //show the question itself to the user
        presentQuestion(num1, num2, operation);
        //get the answer from the user
        int userAnswer = getUserAnswer();
       
        // Calculate the correct answer based on the operation
        correctAnswer = correctAnsweris(num1, num2, operation);
       
        // Check if the user's answer is correct
        if (checkAnswer(userAnswer, correctAnswer)) {
            cout << "Correct!" << endl;
            score++;
        } else {
            cout << "Wrong answer. The correct answer was " << correctAnswer << "." << endl;
        }
    }
    //display the score
     displayScore(score, TOTAL_QUESTIONS);

}
//---------------------------------------------
int correctAnsweris(int num1, int num2, char operation)
{
    if (operation == '+') {
        return num1 + num2;
    } else {
        return num1 - num2;
    }
}
//---------------------------------------------


void generateQuestion(int &num1, int &num2, char &operation) {
    num1 = rand() % 100; // Generate a random number between 0 and 99
    num2 = rand() % 100; // Generate a second random number between 0 and 99
    operation = rand() % 2 == 0 ? '+' : '-'; // Randomly choose between addition and subtraction
}

void presentQuestion(int num1, int num2, char operation) {
    cout << "What is " << num1 << " " << operation << " " << num2 << "? ";
}

int getUserAnswer() {
    int answer;
    cin >> answer;
    return answer;
}

bool checkAnswer(int userAnswer, int correctAnswer) {
    return userAnswer == correctAnswer;
}

void displayScore(int score, int totalQuestions) {
    cout << "You scored " << score << " out of " << totalQuestions << "." << endl;
}

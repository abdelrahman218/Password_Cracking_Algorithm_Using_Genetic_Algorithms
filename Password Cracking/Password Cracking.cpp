#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

//Global Percentages
const double PERCENTAGE_OF_BEST_FIT = 0.05;
const double PERCENTAGE_OF_PASSWORDS_TO_BE_MERGED=0.05;

//Global Variable That will be needed during the whole program
#define NO_OF_PASSWORDS_PER_ITERARTION 200
const string POSSIBLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890_-@";
string correctPassword;

//Function That generates a random number within a given range
int randomNumber(int start, int end) {
    return start + (rand() % (end - start + 1));
}

//Function That return a random character from the possible characters
char get_random_character(){
    return POSSIBLE_CHARS[randomNumber(0,POSSIBLE_CHARS.length() - 1)];
}

//Function that return a random combination of characters
string generate_random_password() {
    string randPass = "";
    for (int i = 0; i < correctPassword.length(); i++)
        randPass += get_random_character();
    return randPass;
}

//Class that keeps the password attemp and its fitness score
class Password {
public:
    string combination;
    int fitnessScore;
    Password(string combination) {
        this->combination = combination;
        calc_fitness();
    }
    Password merge(const Password& pass2) {
        string newComb = "";
        float random;
        for (int i = 0; i < correctPassword.length(); i++) {
            random = (float)(randomNumber(0,100)) / 100;
            if (random < 0.45)
                newComb += combination[i];
            else if (random < 0.90)
                newComb += pass2.combination[i];
            else
                newComb += get_random_character();
        }
        return Password(newComb);
    }
private:
    void calc_fitness() {
        fitnessScore = 0;
        for (int i = 0; i < correctPassword.length();i++) {
            if (combination[i] != correctPassword[i])
                fitnessScore++;
        }
    }
};

// Overloading < operator 
bool operator<(const Password& ind1, const Password& ind2)
{
    return ind1.fitnessScore < ind2.fitnessScore;
}

//The Main Function That does all the work
void crackPassword(string password) {
    srand((unsigned)time(NULL));
    correctPassword = password;

    //Where we store all passwords in each iteration
    vector<Password> passwords;

    //Indicates The iteration number
    int iterNum = 1;

    //Beginning with a random start
    for (int i = 0; i < NO_OF_PASSWORDS_PER_ITERARTION; i++)
        passwords.push_back(Password(generate_random_password()));

    //Iterative Operation
    while (true) {

        //Sorting the vector according to the fitness score
        sort(passwords.begin(), passwords.end());

        //if the first password reached fitness score 0 then we have reached our target
        if (passwords[0].fitnessScore <= 0) {
            break;
        }

        int s =  PERCENTAGE_OF_BEST_FIT * NO_OF_PASSWORDS_PER_ITERARTION;
        vector<Password> next_iteration;

        //Best fitted Passwords will be passed to the next iteration
        for (int i = 0; i < s; i++)
            next_iteration.push_back(passwords[i]);

        s = (1-PERCENTAGE_OF_BEST_FIT) * NO_OF_PASSWORDS_PER_ITERARTION;

        //The Rest (95%) will be merging from the best (5%) fitted passwords
        for (int i = 0; i < s; i++) {
            int r = randomNumber(0, (int)(PERCENTAGE_OF_PASSWORDS_TO_BE_MERGED * NO_OF_PASSWORDS_PER_ITERARTION));
            Password pass1 = passwords[r];
            r = randomNumber(0, (int)(PERCENTAGE_OF_PASSWORDS_TO_BE_MERGED * NO_OF_PASSWORDS_PER_ITERARTION));
            Password pass2 = passwords[r];
            Password newPass = pass1.merge(pass2);
            next_iteration.push_back(newPass);
        }
        passwords = next_iteration;
        cout << "Iteration: " << iterNum << "\t";
        cout << "String: " << passwords[0].combination << "\t";
        cout << "Fitness Score: " << passwords[0].fitnessScore << "\n";
        iterNum++;
    }
    cout << "\n\nThe Password \"" << passwords[0].combination << "\" was cracked after " << iterNum-1 << " attempts.\n";
}

int main()
{
    crackPassword("obbg85LiAogyBtvbGtP7HCCCvh7GSakU5YGystFBkeLprvbvSf");
}
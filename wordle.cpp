#include <iostream>
#include <vector>

using namespace std;

/*

***WORDLE GAME***

You have six tries to guess a five-letter word
- Green letters:    letter exists in answer, and is in the correct position
- Yellow letters:   letter exists in answer, but is in the incorrect position
- Grey letters:     letter does not exist in answer

*/

// make an entire string uppercase.
string toupper_string(string s) {
    string ret_s = "";
    for (int i = 0; i < s.length(); i++)
        ret_s.push_back(toupper(s[i]));
    return ret_s;
}

// - if c is in s, remove first instance.
// - return s otherwise.
string remove_char(char c, string s) {
    string ret_s = "";
    bool flag = false;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == c && !(flag)) flag = true;
        else ret_s.push_back(s[i]);
    }

    return ret_s;
}

string bold_grey(char c) { return "\033[1;30m" + string(1, c) + "\033[0m"; }

string bold_yellow(char c) { return "\033[1;33m" + string(1, c) + "\033[0m"; }

string bold_green(char c) { return "\033[1;32m" + string(1, c) + "\033[0m"; }

// when the user inputs a guess, flag each letter in the alphabet to white, green, yellow, or grey.
vector<pair <int, char> > update_guessed_letters(string guess, string answer, vector<pair <int, char> > guessed_letters) {
    guess = toupper_string(guess);
    answer = toupper_string(answer);

    string wrong_letters = answer;
    string ngl = ""; // a string of non-green letters in the answer

    // build ngl
    for (int i = 0; i < guess.length(); i++)
        if (guess[i] != answer[i])
            ngl.push_back(answer[i]);

    /*  EXAMPLE:    ans = "ROBOT"; guess = "MOTOR".
                    ngl = "RBT".
    */

    // now set the guessed letters to the appropriate colours
    for (int i = 0; i < guess.length(); i++) {

        if (guess[i] == answer[i]) {
            /*  if the i-th letter in guess == i-th letter in answer, set letter to green. */
            guessed_letters[guess[i]-65].first = 3;

        } else if (ngl.find(guess[i]) != string::npos) {
            /*  otherwise, if the i-th letter in guess is still in ngl:
                    - remove the letter from ngl ONCE
                    - set letter to yellow
            */
            ngl = remove_char(guess[i], ngl);
            guessed_letters[guess[i]-65].first = 2;

        } else {
            /*  otherwise, the i-th letter in guess is not in ngl at all, so set it to grey. */
            guessed_letters[guess[i]-65].first = 1;
        }
    }
    return guessed_letters;
}

void print_guess(string guess, vector<pair <int, char> > guessed_letters) {
    guess = toupper_string(guess);
    
    for (int i = 0; i < guess.length(); i++) {
        switch (guessed_letters[guess[i]-65].first) {
            case 0:
                break;
            case 1:
                cout << bold_grey(guess[i]);
                break;
            case 2:
                cout << bold_yellow(guess[i]);
                break;
            case 3:
                cout << bold_green(guess[i]);
                break;
        }
    }
    cout << "\n";
}

void print_alphabet(vector<pair <int, char> > guessed_letters) {
    for (int i = 0; i < 26; i++) {
        switch (guessed_letters[i].first) {
            case 0:
                cout << guessed_letters[i].second << " ";
                break;
            case 1:
                cout << bold_grey(guessed_letters[i].second) << " ";
                break;
            case 2:
                cout << bold_yellow(guessed_letters[i].second) << " ";
                break;
            case 3:
                cout << bold_green(guessed_letters[i].second) << " ";
                break;
        }
    }
    cout << "\n";
}


int main() {
    
    int guesses_left = 6;
    string guess;
    string answer;
    bool won = false;

    vector <pair <int, char> > guessed_letters;
    for (int i = 0; i < 26; i++)
        guessed_letters.push_back(make_pair(0, i + 65));

    cout << "\n";

    while (1) {
        cout << "\nCome up with a 5-letter word for someone to guess. ";
        cin >> answer;
        cout << "\n";
        if (answer.length() != 5)
            cout << "\033[1;37mPlease make sure your word is 5 letters.\033[0m\n";
        else if (answer.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos)
            cout << "\033[1;37mPlease do not include numbers or special characters in your word.\033[0m\n";
        else {
            for (int i = 0; i < 100; i++) cout << "\n";
            break;
        }
    }
    
    while (guesses_left > 0) {
        if (guesses_left == 1)
            cout << "\nYou have 1 guess left.\n";
        else
            cout << "\nYou have " << guesses_left << " guesses left.\n";
        cout << "Guess a 5-letter word: ";
        cin >> guess;
        cout << "\n";

        if (toupper_string(guess) == toupper_string(answer)) {
            guessed_letters = update_guessed_letters(guess, answer, guessed_letters);
            print_guess(guess, guessed_letters);
            won = true;
            break;
        } else if (guess.length() != 5)
            cout << "\033[1;37mPlease make sure the word you guess is 5 letters.\033[0m\n";
        else if (guess.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos)
            cout << "\033[1;37mPlease do not include numbers or special characters in your guess.\033[0m\n";
        else {
            guessed_letters = update_guessed_letters(guess, answer, guessed_letters);
            print_guess(guess, guessed_letters);
            print_alphabet(guessed_letters);
            guesses_left--;
        }
    }

    if (won) {
        cout << "\nCongratulations, you did it!\n\n";
    } else {
        cout << "\nYou ran out of guesses. The word was: " << "\033[1;32m" + toupper_string(answer) + "\033[0m\n" << ".\n\n";
    }

    // cout << "\033[1;30mbold grey text\033[0m\n";
    // cout << "\033[1;32mbold green text\033[0m\n";
    // cout << "\033[1;33mbold yellow text\033[0m\n";

    return 0;
}
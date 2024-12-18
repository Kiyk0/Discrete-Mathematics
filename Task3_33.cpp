#include <iostream>
#include <string>
using namespace std;

string allowed = "ABCDEFGHILMNOPQRSTUVZ ";                            // Allowed letters are Italian Alphabetics + spaces.

bool isValidMessage(string message) {                                 // Check if entered message is composed of Italian alphabet or not.

    for (int i = 0; i < message.length(); ++i) {
        if (allowed.find(message[i]) == string::npos)                 // (.find) check if each character exists in the allowed alphabetics or not 
            return false;                                             // (string::npos) is a special value indicating "not found" letter.
    }
    return true;                                                      // The function returns false if a letter is not found and if not it returns true.
}                  

string cipheredMessage(string message, int a, int b) {                // This function ciphers the message sent by the user.
    string result = "";                                               // First we clear the result each time we call this function.

    for (int i = 0; i < message.length(); i++) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            int index = allowed.find(message[i]);                     // Find the index of the character in the Italian Alphabet.
            if (index != string::npos) {                              // If the character is found in the Italian Alphabet.
                int AFFINE = (a * index + b) % (allowed.length() - 1);// Apply the affine transformation using mod (allowed.length) as they are 21 + space
                if (AFFINE < 0) {                                     // if we changed the allowed length, it will be automatically modified here.
                    AFFINE += (allowed.length() - 1);                 // Add 21 to check the result is non-negative.
                }
                result += allowed[AFFINE];                            // Return back to the Italian alphabet.
            }
        }
        else if (message[i] == ' ')
            result += ' ';                                            // Spaces are not changed they are printed as they are.
    }
    return result;
}

void getInputsAB(int& a, int& b) {                              // This function takes 'a' and 'b' as referenced parameters
    while (true) {                                              // Repeat the method taking inputs from user until they are valid
        cout << "Enter Your Ciphering Key as Follow (a * message + b):\n";
        cout << "Enter value of a: ";

        if (!(cin >> a)) {                                      // Check that input 'a' is an integer not to cause a run-time error
            cout << "Invalid input!! Please enter an integer value for 'a'.\n";
            cin.clear();                                        // Clear the input to take a new one
            continue;                                           // Skip the remaining code in the current iteration of the loop
        }                                                       // and start the next iteration
        
        cout << "Enter value of b: ";
        if (!(cin >> b)) {                                      // Check that input 'b' is an integer not to cause a run-time error
            cout << "Invalid input!! Please enter an integer value for 'b'.\n";
            cin.clear();                                        // Clear the input to take a new one
            continue;                                           // Skip the remaining code in the current iteration of the loop
        }                                                       // and start the next iteration

        break;                                                  // Break out only if 'a' and 'b' are valid inputs
    }
}

int main() {
    string message, result = "";
    int a, b, choice, solution = 0;
    bool valid = true, check = false;

    do {
        cout << "1) Cipher a Message.\n2) Proposed Ciphered Codes.\n3) End Program.\n";
        check = false;
        while(!check) {
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice < 1 || choice > 3)
                cout << "Invalid choice!! Please enter an integer number between 1 and 3.\n";
            else
                check = true;
        }

        cin.ignore();                                                 // Clear unwanted characters left in the input buffer
        switch (choice) {
        case 1:
            do {
                cout << "Enter your message (only uppercase letters from Italian Alphabet (ABCDEFGHILMNOPQRSTUVZ) and spaces): ";
                getline(cin, message);                                // Take an input from the user as a whole line to take spaces also.

                valid = isValidMessage(message);
                if (!valid) {
                    cout << "Invalid input. Please enter only uppercase letters from Italian Alphabet (ABCDEFGHILMNOPQRSTUVZ) and spaces.\n";
                }
            } while (!valid);
            
            getInputsAB(a, b);                                        // Invoke a method to take 'a' and 'b' from the user.

            cout << "Your ciphered message is: " << cipheredMessage(message, a, b) << endl;
            cout << "---------------------------------\n";
            break;

        case 2:
            cout << "Our Proposed Cipher Keys are: \n";
            cout << "1) 5x+8\n2) 6x-7\n";
            cout << "---------------------------------\n";
            cout << "1) Use 1st Proposed Code.\n2) Use 2nd Proposed Code.\n";
            cout << "Enter your choice: ";
            cin >> solution;
            if (solution == 1 || solution == 2) {
                do {
                    cout << "Enter your desired message: ";
                    cin.ignore();
                    getline(cin, message);                            // Take an input from the user as a whole line to take spaces also.

                    valid = isValidMessage(message);
                    if (!valid) {
                        cout << "Invalid input!! Please enter only uppercase letters from Italian Alphabet (ABCDEFGHILMNOPQRSTUVZ) and spaces.\n";
                    }
                } while (!valid);
            }

            switch (solution) {
            case 1:
                cout << "Your ciphered message is: " << cipheredMessage(message, 5, 8) << endl;
                cout << "---------------------------------\n";
                break;

            case 2:
                cout << "Your ciphered message is: " << cipheredMessage(message, 6, -7) << endl;
                cout << "---------------------------------\n";
                result = "";
                break;

            default:
                cout << "Invalid Input!!\n";
                break;
            }
            break;

        case 3:
            cout << "GOOD BYE!! ^-^\n";
            return 0;

        default:
            cout << "Invalid Input!! Please enter an integer number between 1 and 3.\n";
            break;
        }
    } while (choice != 3);
    return 0;
}
 
// Test Case:
/*
Message to be Ciphered : MI PIACE IL GELATO
Ciphering Keys: a = 7 , b = 9
Ciphered Message: SC SCLCS CL LSLLCL
*/


// NOTE:
/*
We will find that there are many letters that are repeated, this is because the entered value of *a* isn't coprime for 21.
You should choose a values that are coprime with 21. These include:
a=1
a=2
a=4
a=5
a=8
a=10
a=11
a=13
a=16
a=17
a=19
a=20
the value of 'a' must be coprime with 21 to ensure that the cipher of every letter maps to a unique letter.
*/

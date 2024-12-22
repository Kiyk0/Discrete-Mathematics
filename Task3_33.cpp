#include <iostream>
#include <string>
#include <limits>
#include <numeric>
using namespace std;

string allowed = " ABCDEFGHILMNOPQRSTUVZ";                            // Allowed letters are Italian Alphabetics + spaces.

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
            int index = allowed.find(message[i]);                     // Find the index of the character in the Italian Alphabet.
            if (index != string::npos) {                              // If the character is found in the Italian Alphabet.
                int AFFINE = (a * index + b) % allowed.length();// Apply the affine transformation using mod (allowed.length) as they are 21 + space
                if (AFFINE < 0) {                                     // if we changed the allowed length, it will be automatically modified here.
                    AFFINE += allowed.length();                 // Add 22 to check the result is non-negative.
                }
                result += allowed[AFFINE];                            // Return back to the Italian alphabet.
                //}
            }
        }
    return result;
}

void getInputsAB(int& a, int& b) {                              // This function takes 'a' and 'b' as referenced parameters
    while (true) {                                              // Repeat the method taking inputs from user until they are valid
        cout << "Enter Your Ciphering Key as Follow (a * message + b):\n";
        cout << "Enter value of a: ";

        if (!(cin >> a)) {                                      // Check that input 'a' is an integer not to cause a run-time error
            cout << "Invalid input!! Please enter an integer value for 'a'.\n";
            cin.clear();    // Clear the input to take a new one
            cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore all the characters until either the deimeter '\n' or the buffer in cin is clear
            continue;                                           // Skip the remaining code in the current iteration of the loop
        }                                                       // and start the next iteration
        if (gcd(a, allowed.length()) != 1) {
            cout << "'a' must be coprime with 22. Please enter a valid value.\n";
            continue;
        }
        
        cout << "Enter value of b: ";
        if (!(cin >> b)) {                                      // Check that input 'b' is an integer not to cause a run-time error
            cout << "Invalid input!! Please enter an integer value for 'b'.\n";
            cin.clear();    // Clear the input to take a new one
            cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore all the characters until either the deimeter '\n' or the buffer in cin is clear
            continue;                                           // Skip the remaining code in the current iteration of the loop
        }                                                       // and start the next iteration

        break;                                                  // Break out only if 'a' and 'b' are valid inputs
    }
}

int main() {
    string message, result = "";
    int a, b, choice;
    bool valid = true, check = false;

    do {
        cout << "1) Cipher a Message.\n2) End Program.\n";
        check = false;
        while(!check) {
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice < 1 || choice > 2)
                cout << "Invalid choice!! Please enter an integer number between 1 and 2.\n";
            else
                check = true;
        }

        cin.ignore();                                                 // Clear unwanted characters left in the input buffer
        switch (choice) {
        case 1:
            do {
                cout << "Enter your message (only uppercase letters from Italian Alphabet (ABCDEFGHILMNOPQRSTUVZ) and spaces): ";
                getline(cin, message);                                

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
            cout << "GOOD BYE!! ^-^\n";
            return 0;

        default:
            cout << "Invalid Input!! Please enter an integer number between 1 and 2.\n";
            break;
        }
    } while (choice != 2);
    return 0;
}
 
// Test Case:
/*
Message to be Ciphered : MI PIACE IL GELATO
Ciphering Keys: a = 11 , b = 0
Ciphered Message: MM  MMMM M  MM M M
*/


// NOTE:
/*
We will find that there are many letters that are repeated, this is because the entered value of *a* isn't coprime for 22.
You should choose a values that are coprime with 22. These include:
a=1
a=3
a=5
a=7
a=9
a=13
a=15
a=17
a=19
a=21
the value of 'a' must be coprime with 21 to ensure that the cipher of every letter maps to a unique letter.
*/

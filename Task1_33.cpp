#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <string>

using namespace std;

bool flag1 = true;
bool flag2 = false;
vector<bool> R;
int r = 0;

// Function to evaluate a single Expression
bool evaluateExpression(vector<bool> values, string Expression) {
    // Replace variables with their truth values
    for (int i = 0; i < values.size(); i++) {
        char var = 'A' + i;
        for (int j = 0; j < Expression.size(); j++) {
            if (Expression[j] == var) {
                if (Expression[j - 1] == '!' && j > 0) Expression[j] = values[i] ? '0' : '1';
                else Expression[j] = values[i] ? '1' : '0';
            }
        }
    }

    // Evaluate the Expression
    while (Expression.find('(') != string::npos) {
        int start = Expression.find_last_of('(');
        int end = Expression.find(')', start);
        string subExpression = Expression.substr(start + 1, end - start - 1);

        bool result = false;
        if (subExpression.find('&') != string::npos) {
            result = true;
            for (char c : subExpression) {
                if (c == '0') {
                    result = false;
                    break;
                }
            }
        }
        else if (subExpression.find('|') != string::npos) {
            result = false;
            for (char c : subExpression) {
                if (c == '1') {
                    result = true;
                    break;
                }
            }
        }
        else if (subExpression.find('1') != string::npos) {
            result = true;
        }

        Expression.replace(start, end - start + 1, result ? "1" : "0");
    }

    if (flag2) {
        R.push_back(Expression == "1");
        flag2 = false;
    }
    return Expression == "1";
}

// Function to generate all possible truth values
vector<vector<bool>> generateTruthValues(int numVars) {
    vector<vector<bool>> truthValues;
    int numRows = pow(2, numVars);

    for (int i = 0; i < numRows; i++) {
        vector<bool> row;
        for (int j = numVars - 1; j >= 0; j--) {
            row.push_back((i & (1 << j)) != 0);
        }
        truthValues.push_back(row);
    }
    return truthValues;
}

// Function to print the truth table
void printTruthTable(vector<vector<bool>> truthValues, const vector<string>& premises, string conclusion) {
    int numVars = truthValues[0].size();
    int count=1;
    // Print header
    for (int i = 0; i < numVars; i++) {
        cout << (char)('A' + i) << " ";
    }
    for (const auto& premise : premises) {
        cout << count << " ";
        count++;
    }
    cout << 'C' << endl;

    // Print rows
    for (const auto& row : truthValues) {
        for (bool val : row) {
            cout << val << " ";
        }

        vector<bool> premiseResults;
        for (const auto& premise : premises) {
            premiseResults.push_back(evaluateExpression(row, premise));
        }

        for (bool result : premiseResults) {
            cout << result << " ";
        }

        cout << evaluateExpression(row, conclusion) << endl;
    }
}

// Function to check if the expression has balanced parentheses
bool isValidExpression(const std::string& expr) {
    std::stack<char> s;
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (ch == '(') {
            s.push(ch);
        }
        else if (ch == ')') {
            if (s.empty() || s.top() != '(') {
                return false;
            }
            s.pop();
        }
        else if (ch == '!' && (i == expr.length() - 1 || !(std::isalpha(expr[i + 1]) || expr[i + 1] == '('))) {
            return false;
        }
    }
    return s.empty();
}

int main() {
    vector<string> premises;
    string line, conclusion;
    int numVars;

    cout << "The correct form of expression should be enclosed by brackets -> (A&B)" << endl;
    cout << "Enter the number of variables: ";

    // Input validation for numVars
    while (true) {
        cin >> numVars;
        if (cin.fail() || numVars <= 0) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid number of variables. Please enter a positive integer: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume leftover newline
            break;
        }
    }

    cout << "Enter multiple premises on each line (type '0' to finish): ";
    while (getline(cin, line)) {
        if (line == "0") break;
        if (line.empty() || !isValidExpression(line)) {
            cout << "Invalid expression." << endl;
            return 0;
        }
        premises.push_back(line);
    }
    cout << "Enter the conclusion: ";
    getline(cin, conclusion);

    vector<vector<bool>> truthValues = generateTruthValues(numVars);

    bool isSatisfiable = false;
    bool isValid = true;

    for (const auto& values : truthValues) {
        bool premisesTrue = true;
        for (const auto& premise : premises) {
            if (!evaluateExpression(values, premise)) {
                premisesTrue = false;
                break;
            }
        }
        if (premisesTrue && !evaluateExpression(values, conclusion)) {
            isValid = false;
        }
        if (premisesTrue && evaluateExpression(values, conclusion)) {
            isSatisfiable = true;
        }
    }

    // Print truth table
    printTruthTable(truthValues, premises, conclusion);

    cout << "The set of formulas is " << (isSatisfiable ? "Satisfiable" : "Not satisfiable") << endl;
    cout << "The argument is " << (isValid ? "Valid" : "Falsifiable") << endl;

    return 0;
}

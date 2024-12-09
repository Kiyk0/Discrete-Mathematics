#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <string>

using namespace std;
bool flag1 = true;
bool flag2 = false;
bool flag3 = false;
vector<bool> O,S;
int r=0;

// Function to evaluate a Expression
bool evaluateExpression(vector<bool> values, string Expression) {
    // Replace variables with their truth values
    for (int i = 0; i < values.size(); i++) {
        char var = 'A' + i;
        for (int j = 0; j < Expression.size(); j++) {
            if (Expression[j] == var) {
                if (Expression[j-1] == '!' && j>0) Expression[j] = values[i] ? '0' : '1';
                else Expression[j] = values[i] ? '1' : '0';
                //cout << var << " = " << Expression[j] << endl;
            }
        }
    }
    // Evaluate the Expression (basic implementation for AND, OR, NOT)
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
        } else if (subExpression.find('|') != string::npos) {
            result = false;
            for (char c : subExpression) {
                if (c == '1') {
                    result = true;
                    break;
                }
            }
        } else if (subExpression.find('1') != string::npos) {
            result = true;
        }

        Expression.replace(start, end - start + 1, result ? "1" : "0");
    }
    //cout << "Expression = " << Expression << endl << "-----------------------" << endl;
    if (flag2) {
        O.push_back(Expression == "1");
        flag2 = false;
    }
    if (flag3) {
        S.push_back(Expression == "1");
        flag3 = false;
    }
    return Expression == "1";
}

// Function to generate all possible truth values
vector<vector<bool>> generateTruthValues(int numVars) {
    if (flag2) cout << "-----------------------" << endl;
    vector<vector<bool>> truthValues;
    int numRows = pow(2, numVars);
    bool val;
    if (flag1 && (flag2 || flag3)) {
        for (size_t i = 0; i < numVars; i++) {
            cout << (char)('A'+ i) << " ";
            flag1 = false;
        }
        if (flag2) cout << "O" << " ";
        if (flag3) cout << "S";
    }
    if (flag2 || flag3) cout << endl;
    for (int i = 0; i < numRows; i++) {
        vector<bool> row;
        for (int j = numVars - 1; j >= 0; j--) {
            val = ((i & (1 << j)) != 0);
            if (flag2 || flag3) cout << val << " ";
            row.push_back(val);
        }
        if (flag2) cout << O[i] << " ";
        if (flag3) cout << S[i] << endl;
        truthValues.push_back(row);
    }
    if (flag2) cout << "-----------------------" << endl;
    return truthValues;
}

// Function to check if the expression has balanced parentheses
bool isValidExpression(const std::string& expr) {
    std::stack<char> s;
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (ch == '(') {
            s.push(ch);
        } else if (ch == ')') {
            if (s.empty() || s.top() != '(') {
                return false;
            }
            s.pop();
        } else if (ch == '!') {
            // Ensure '!' is followed by a valid variable or '('
            if (i == expr.length() - 1 || !(std::isalpha(expr[i + 1]) || expr[i + 1] == '(')) {
                return false;
            }
        } else if (ch == '&' || ch == '|' || std::isalpha(ch) || std::isspace(ch)) {
            // Allow valid operators, alphabets (variables), and spaces
            continue;
        } else {
            // Invalid character found
            return false;
        }
    }
    return s.empty();
}

// Function to ensure all operators are enclosed by valid expressions
bool checkOperatorsEnclosed(const std::string& expr) {
    for (size_t i = 0; i < expr.length(); ++i) {
        if (expr[i] == '&' || expr[i] == '|') {
            // Ensure there is a valid character or ')' before the operator
            if (i == 0 || !(std::isalpha(expr[i - 1]) || expr[i - 1] == ')')) {
                return false;
            }
            // Ensure there is a valid character or '(' after the operator
            if (i == expr.length() - 1 || !(std::isalpha(expr[i + 1]) || expr[i + 1] == '(' || expr[i + 1] == '!')) {
                return false;
            }
        } else if (expr[i] == '!') {
            // Ensure '!' is followed by a valid variable or '('
            if (i == expr.length() - 1 || !(std::isalpha(expr[i + 1]) || expr[i + 1] == '(')) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    string originalExpression,simplifiedExpression;
    int numVars;
    cout << "The correct form of expression should enclosed by brackets -> (A&B)" << endl;
    cout << "Enter the number of variables: ";
    cin >> numVars; // Number of variables (A, B, C)
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter the original expression: ";
    getline(cin, originalExpression);
    if (originalExpression.empty()) {
        cout << "The expression is invalid (empty input)." << endl;
        return 0;
    } else if (!isValidExpression(originalExpression) || !checkOperatorsEnclosed(originalExpression)) {
        cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
        return 0;
    }

    cout << "Enter the simplified expression: ";
    getline(cin, simplifiedExpression);
    if (simplifiedExpression.empty()) {
        cout << "The expression is invalid (empty input)." << endl;
        return 0;
    } else if (!isValidExpression(simplifiedExpression) || !checkOperatorsEnclosed(simplifiedExpression)) {
        cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
        return 0;
    }

    vector<vector<bool>> truthValues = generateTruthValues(numVars);

    bool areEquivalent = true;
    bool isSatisfiableOriginal = false;
    bool isSatisfiableSimplified = false;

    for (vector<bool> values : truthValues) {
        flag2 = true;
        bool originalResult = evaluateExpression(values, originalExpression);
        flag3 = true;
        bool simplifiedResult = evaluateExpression(values, simplifiedExpression);
        if (originalResult != simplifiedResult) {
            areEquivalent = false;
        }
        if (originalResult) {
            isSatisfiableOriginal = true;
        }
        if (simplifiedResult) {
            isSatisfiableSimplified = true;
        }
    }
    flag2 = flag3 = true;
    generateTruthValues(numVars);
    cout << "The original and simplified expressions are " << (areEquivalent ? "equivalent" : "not equivalent") << endl;
    cout << "The original expression is " << (isSatisfiableOriginal ? "satisfiable" : "not satisfiable") << endl;
    cout << "The simplified expression is " << (isSatisfiableSimplified ? "satisfiable" : "not satisfiable") << endl;

    if (!isSatisfiableOriginal) {
        cout << "Changing one gate to make the original expression satisfiable..." << endl;
        // Example: Change OR to AND in the original expression
        getline(cin, originalExpression);
        if (originalExpression.empty()) {
        cout << "The expression is invalid (empty input)." << endl;
        return 0;
        } else if (!isValidExpression(originalExpression) || !checkOperatorsEnclosed(originalExpression)) {
        cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
        return 0;
        }

        isSatisfiableOriginal = false;
        for (auto values : truthValues) {
            if (evaluateExpression(values, originalExpression)) {
                isSatisfiableOriginal = true;
                break;
            }
        }
        cout << "The modified original expression is " << (isSatisfiableOriginal ? "satisfiable" : "not satisfiable") << endl;
    }
    return 0;
}
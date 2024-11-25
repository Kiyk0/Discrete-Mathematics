#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <string>

using namespace std;
bool flag1 = true;
bool flag2 = false;
vector<bool> R;
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
        R.push_back(Expression == "1");
        flag2 = false;
        }
    return Expression == "1";
}

// Function to generate all possible truth values
vector<vector<bool>> generateTruthValues(int numVars) {
    if (flag2) cout << "-----------------------" << endl;
    vector<vector<bool>> truthValues;
    int numRows = pow(2, numVars);
    bool val;
    if (flag1 && flag2) {
        for (size_t i = 0; i < numVars; i++) {
            cout << (char)('A'+ i) << " ";
            flag1 = false;
        }
        if (flag2) cout << "R";
    }
    if (flag2) cout << endl;
    for (int i = 0; i < numRows; i++) {
        vector<bool> row;
        for (int j = numVars - 1; j >= 0; j--) {
            val = ((i & (1 << j)) != 0);
            if (flag2) cout << val << " ";
            row.push_back(val);
        }
        if (flag2) cout << R[i] << endl;
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
    vector<string> premises; //{(A|!B),(!A|B)} {(A|!B),(!A|B),(!A|!B)|(!C)} {((!A)|(B|!C)),((!B)|(A&C))};
    string line, conclusion; //{(A&B)} {(!A|C)}
    int numVars;
    cout << "The correct form of expression should enclosed by brackets -> (A&B)" << endl;
    cout << "Enter the number of variables: ";
    cin >> numVars; // Number of variables (A, B, C)
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter multiple premise on each line (type '0' to finish): ";
    while (getline(cin, line)) {
        if (line == "0") {
            break;
        }
        if (line.empty()) {
        cout << "The expression is invalid (empty input)." << endl;
        return 0;
        } else if (!isValidExpression(line) || !checkOperatorsEnclosed(line)) {
        cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
        return 0;
        }
    premises.push_back(line);
    }
    cout << "Enter the conclusion: ";
    getline(cin, conclusion);

    vector<vector<bool>> truthValues = generateTruthValues(numVars);

    bool isSatisfiable = false;
    bool isValid = true;

    for (vector<bool> values : truthValues) {
        bool premisesTrue = true;
        for (string premise : premises) {
            if (!evaluateExpression(values, premise)) {
                premisesTrue = false;
                break;
            }
        }
        flag2 = true;
        bool conclusionTrue = evaluateExpression(values, conclusion);

        if (premisesTrue && conclusionTrue) {
            isSatisfiable = true;
        }
        if (premisesTrue && !conclusionTrue) {
            isValid = false;
        }
    }
    flag2 = true;
    generateTruthValues(numVars);
    cout << "The set of formulas is " << (isSatisfiable ? "Satisfiable" : "Not satisfiable") << endl;
    cout << "The argument is " << (isValid ? "Valid" : "Falsifiable") << endl;

    return 0;
}
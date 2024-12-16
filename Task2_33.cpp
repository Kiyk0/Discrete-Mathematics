#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <string>

using namespace std;

bool flag1 = true;
bool flag2 = false;
bool flag3 = false;
vector<bool> O, S;

// Function to evaluate an Expression
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
            cout << (char)('A' + i) << " ";
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
bool isValidExpression(const string& expr) {
    stack<char> s;
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
        else if (ch == '!') {
            if (i == expr.length() - 1 || !(isalpha(expr[i + 1]) || expr[i + 1] == '(')) {
                return false;
            }
        }
        else if (ch == '&' || ch == '|' || isalpha(ch) || isspace(ch)) {
            continue;
        }
        else {
            return false;
        }
    }
    return s.empty();
}

bool checkOperatorsEnclosed(const string& expr) {
    for (size_t i = 0; i < expr.length(); ++i) {
        if (expr[i] == '&' || expr[i] == '|') {
            if (i == 0 || !(isalpha(expr[i - 1]) || expr[i - 1] == ')')) {
                return false;
            }
            if (i == expr.length() - 1 || !(isalpha(expr[i + 1]) || expr[i + 1] == '(' || expr[i + 1] == '!')) {
                return false;
            }
        }
        else if (expr[i] == '!') {
            if (i == expr.length() - 1 || !(isalpha(expr[i + 1]) || expr[i + 1] == '(')) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    string originalExpression, simplifiedExpression;
    int numVars;
    while (true) {
        cout << "The correct form of expression should be enclosed by brackets -> (A&B)" << endl;
        cout << "Enter the number of variables: ";
        cin >> numVars;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter the original expression: ";
        getline(cin, originalExpression);
        if (originalExpression.empty() || !isValidExpression(originalExpression) || !checkOperatorsEnclosed(originalExpression)) {
            cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
            continue;
        }

        cout << "Enter the simplified expression: ";
        getline(cin, simplifiedExpression);
        if (simplifiedExpression.empty() || !isValidExpression(simplifiedExpression) || !checkOperatorsEnclosed(simplifiedExpression)) {
            cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
            continue;
        }

        vector<vector<bool>> truthValues = generateTruthValues(numVars);

        bool areEquivalent = true;
        bool isSatisfiableOriginal = false;
        bool isSatisfiableSimplified = false;
        vector<vector<bool>> satisfiableOriginalValues;
        vector<vector<bool>> satisfiableSimplifiedValues;
        vector<vector<bool>> commonValues;

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
                satisfiableOriginalValues.push_back(values);
            }
            if (simplifiedResult) {
                isSatisfiableSimplified = true;
                satisfiableSimplifiedValues.push_back(values);
            }
            if (originalResult && simplifiedResult) {
                commonValues.push_back(values);
            }
        }

        flag2 = flag3 = true;
        generateTruthValues(numVars);
        cout << "The original and simplified expressions are " << (areEquivalent ? "equivalent" : "not equivalent") << endl;
        cout << "The original expression is " << (isSatisfiableOriginal ? "satisfiable" : "not satisfiable") << endl;
        cout << "The simplified expression is " << (isSatisfiableSimplified ? "satisfiable" : "not satisfiable") << endl;

        if (!commonValues.empty()) {
            cout << "The common truth values that satisfy both expressions are: " << endl;
            for (const auto& values : commonValues) {
                for (bool val : values) {
                    cout << val << " ";
                }
                cout << endl;
            }
            break;
        }
        else {
            cout << "There are no common values that make both expressions satisfiable. Please enter new expressions." << endl;
        }
    }

    return 0;
}

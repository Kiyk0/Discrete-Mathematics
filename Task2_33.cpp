#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <string>

using namespace std;

bool flag1 = true;
bool flag2 = false;
bool flag3 = false;
bool flag4 = true;
bool flag5 = false;
int iii = 0;
vector<bool> O, S;

// Function to evaluate an Expression
bool evaluateExpression(vector<bool> values, string Expression)
{
    // Replace variables with their truth values
    for (int i = 0; i < values.size(); i++)
    {
        char var = 'A' + i;
        for (int j = 0; j < Expression.size(); j++)
        {
            if (Expression[j] == var)
            {
                if (Expression[j - 1] == '!' && j > 0)
                    Expression[j] = values[i] ? '0' : '1';
                else
                    Expression[j] = values[i] ? '1' : '0';
            }
        }
    }
    // Evaluate the Expression (basic implementation for AND, OR, NOT)
    while (Expression.find('(') != string::npos)
    {
        int start = Expression.find_last_of('(');
        int end = Expression.find(')', start);
        string subExpression = Expression.substr(start + 1, end - start - 1);

        bool result = false;
        if (subExpression.find('&') != string::npos)
        {
            result = true;
            for (char c : subExpression)
            {
                if (c == '0')
                {
                    result = false;
                    break;
                }
            }
        }
        else if (subExpression.find('|') != string::npos)
        {
            result = false;
            for (char c : subExpression)
            {
                if (c == '1')
                {
                    result = true;
                    break;
                }
            }
        }
        else if (subExpression.find('1') != string::npos)
        {
            result = true;
        }

        Expression.replace(start, end - start + 1, result ? "1" : "0");
    }
    if (flag2)
    {
        O.push_back(Expression == "1");
        flag2 = false;
    }
    if (flag3)
    {
        S.push_back(Expression == "1");
        flag3 = false;
    }
    return Expression == "1";
}

// Function to generate all possible truth values
vector<vector<bool>> generateTruthValues(int numVars)
{
    if (flag2)
        cout << "-----------------------" << endl;
    vector<vector<bool>> truthValues;
    int numRows = pow(2, numVars);
    bool val;
    if (flag1 && (flag2 || flag3))
    {
        for (size_t i = 0; i < numVars; i++)
        {
            cout << (char)('A' + i) << " ";
            flag1 = false;
        }
        if (flag2)
            cout << "O" << " ";
        if (flag3)
            cout << "S";
    }
    if (flag2 || flag3)
        cout << endl;
    for (int i = 0; i < numRows; i++)
    {
        vector<bool> row;
        for (int j = numVars - 1; j >= 0; j--)
        {
            val = ((i & (1 << j)) != 0);
            if (flag2 || flag3)
                cout << val << " ";
            row.push_back(val);
        }
        if (flag2)
            cout << O[i] << " ";
        if (flag3)
            cout << S[i] << endl;
        truthValues.push_back(row);
    }
    if (flag2)
        cout << "-----------------------" << endl;
    return truthValues;
}

// Function to check if the expression has balanced parentheses
bool isValidExpression(const string& expr)
{
    stack<char> s;
    for (size_t i = 0; i < expr.length(); ++i)
    {
        char ch = expr[i];
        if (ch == '(')
        {
            s.push(ch);
        }
        else if (ch == ')')
        {
            if (s.empty() || s.top() != '(')
            {
                return false;
            }
            s.pop();
        }
        else if (ch == '!')
        {
            if (i == expr.length() - 1 || !(isalpha(expr[i + 1]) || expr[i + 1] == '('))
            {
                return false;
            }
        }
        else if (ch == '&' || ch == '|' || isalpha(ch) || isspace(ch))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return s.empty();
}

bool checkOperatorsEnclosed(const string& expr)
{
    for (size_t i = 0; i < expr.length(); ++i)
    {
        if (expr[i] == '&' || expr[i] == '|')
        {
            if (i == 0 || !(isalpha(expr[i - 1]) || expr[i - 1] == ')'))
            {
                return false;
            }
            if (i == expr.length() - 1 || !(isalpha(expr[i + 1]) || expr[i + 1] == '(' || expr[i + 1] == '!'))
            {
                return false;
            }
        }
        else if (expr[i] == '!')
        {
            if (i == expr.length() - 1 || !(isalpha(expr[i + 1]) || expr[i + 1] == '('))
            {
                return false;
            }
        }
    }
    return true;
}

// New function to modify an expression to make it satisfiable
string modifyToSatisfiable(string expr)
{
    string modifiedExpr = expr;
    for (; iii < expr.size(); ++iii)
    {
        if (expr[iii] == '&')
        {
            modifiedExpr[iii] = '|'; // Try changing AND to OR
            iii++;
            return modifiedExpr;
        }
        else if (expr[iii] == '|')
        {
            modifiedExpr[iii] = '&'; // Try changing OR to AND
            iii++;
            return modifiedExpr;
        }
        else if (expr[iii] == '!')
        {
            modifiedExpr.erase(iii, 1); // Remove a NOT gate
            iii++;
            return modifiedExpr;
        }
    }

    return modifiedExpr; // Return the modified expression
}

int main()
{
    string originalExpression, simplifiedExpression;
    int numVars;
    cout << "Welcome, read the following steps to learn how to use the program" << endl;
    cout << "The correct form of expression should be enclosed by brackets -> (!(A|B)|!C) and (B|C) and (!A|!C)" << endl;
    cout << "You use can only use this version of AND -> '&', OR -> '|' and NOT -> '!'" << endl;
    cout << "Use uppercase alphabet in sequential order starting from A B C ... Z" << endl;
    cout << "Enter the number of variables: ";
    cin >> numVars;

    // Check for invalid input or negative/zero variables
    if (cin.fail() || numVars <= 0)
    {
        cin.clear();                                         // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "Invalid number of variables. Exiting..." << endl;
        return 0;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

    cout << "Enter the original expression: ";
    getline(cin, originalExpression);
    if (originalExpression.empty() || !isValidExpression(originalExpression) || !checkOperatorsEnclosed(originalExpression))
    {
        cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
    }

    cout << "Enter the simplified expression: ";
    getline(cin, simplifiedExpression);
    if (simplifiedExpression.empty() || !isValidExpression(simplifiedExpression) || !checkOperatorsEnclosed(simplifiedExpression))
    {
        cout << "The expression is invalid or not all operators are enclosed by brackets." << endl;
    }

    vector<vector<bool>> truthValues = generateTruthValues(numVars);

    bool areEquivalent = true;
    bool isSatisfiableOriginal = false;
    bool isSatisfiableSimplified = false;
    vector<vector<bool>> satisfiableOriginalValues;
    vector<vector<bool>> satisfiableSimplifiedValues;
    vector<vector<bool>> commonValues;

    for (vector<bool> values : truthValues)
    {
        flag2 = true;
        bool originalResult = evaluateExpression(values, originalExpression);
        flag3 = true;
        bool simplifiedResult = evaluateExpression(values, simplifiedExpression);

        if (originalResult != simplifiedResult)
        {
            areEquivalent = false;
        }
        if (originalResult)
        {
            isSatisfiableOriginal = true;
            satisfiableOriginalValues.push_back(values);
        }
        if (simplifiedResult)
        {
            isSatisfiableSimplified = true;
            satisfiableSimplifiedValues.push_back(values);
        }
        if (originalResult && simplifiedResult)
        {
            commonValues.push_back(values);
        }
    }

    flag2 = flag3 = true;
    generateTruthValues(numVars);
    cout << "The original and simplified expressions are " << (areEquivalent ? "equivalent" : "not equivalent") << endl;
    cout << "The original expression is " << (isSatisfiableOriginal ? "satisfiable" : "not satisfiable") << endl;
    cout << "The simplified expression is " << (isSatisfiableSimplified ? "satisfiable" : "not satisfiable") << endl;

    if (!commonValues.empty())
    {
        cout << "The common truth values that satisfy both expressions are: " << endl;
        for (const auto& values : commonValues)
        {
            for (bool val : values)
            {
                cout << val << " ";
            }
            cout << endl;
        }
    }
    else
    {

        string modifiedExpression;
        do
        {
            if (iii == originalExpression.size() || iii == simplifiedExpression.size())
            {
                cout << "The expression can't be Satisfiable by modifying one gate" << endl;
                break;
            }

            if (!isSatisfiableOriginal)
            {
                cout << "The original expression is unsatisfiable. Attempting to modify the original expression..." << endl;
                modifiedExpression = modifyToSatisfiable(originalExpression);
                cout << "Modified expression to make it satisfiable: " << modifiedExpression << endl;
                flag1 = true;
                O.clear();
                S.clear();

                // originalExpression = modifiedExpression;
                areEquivalent = true;
                isSatisfiableOriginal = false;
                isSatisfiableSimplified = false;
                satisfiableOriginalValues.clear();
                satisfiableSimplifiedValues.clear();
                commonValues.clear();
                flag2 = flag3 = false;

                for (vector<bool> values : truthValues)
                {
                    flag2 = true;
                    bool originalResult = evaluateExpression(values, modifiedExpression);
                    flag3 = true;
                    bool simplifiedResult = evaluateExpression(values, simplifiedExpression);

                    if (originalResult != simplifiedResult)
                    {
                        areEquivalent = false;
                    }
                    if (originalResult)
                    {
                        isSatisfiableOriginal = true;
                        satisfiableOriginalValues.push_back(values);
                    }
                    if (simplifiedResult)
                    {
                        isSatisfiableSimplified = true;
                        satisfiableSimplifiedValues.push_back(values);
                    }
                    if (originalResult && simplifiedResult)
                    {
                        commonValues.push_back(values);
                    }
                }

                flag2 = flag3 = true;
                generateTruthValues(numVars);
                cout << "The original and simplified expressions are " << (areEquivalent ? "equivalent" : "not equivalent") << endl;
                cout << "The original expression is " << (isSatisfiableOriginal ? "satisfiable" : "not satisfiable") << endl;
                cout << "The simplified expression is " << (isSatisfiableSimplified ? "satisfiable" : "not satisfiable") << endl;

                if (!commonValues.empty())
                {
                    cout << "The common truth values that satisfy both expressions are: " << endl;
                    for (const auto& values : commonValues)
                    {
                        for (bool val : values)
                        {
                            cout << val << " ";
                        }
                        cout << endl;
                    }
                }
                flag5 = true;
            }
            if (isSatisfiableOriginal && flag4 && flag5)
            {
                iii = 0;
                flag4 = false;
                originalExpression = modifiedExpression;
            }

            if (!isSatisfiableSimplified && isSatisfiableOriginal)
            {
                cout << "The simplified expression is unsatisfiable. Attempting to modify the simplified expression..." << endl;
                modifiedExpression = modifyToSatisfiable(simplifiedExpression);
                cout << "Modified expression to make it satisfiable: " << modifiedExpression << endl;
                flag1 = true;
                O.clear();
                S.clear();

                // simplifiedExpression = modifiedExpression;
                areEquivalent = true;
                isSatisfiableOriginal = false;
                isSatisfiableSimplified = false;
                satisfiableOriginalValues.clear();
                satisfiableSimplifiedValues.clear();
                commonValues.clear();
                flag2 = flag3 = false;

                for (vector<bool> values : truthValues)
                {
                    flag2 = true;
                    bool originalResult = evaluateExpression(values, originalExpression);
                    flag3 = true;
                    bool simplifiedResult = evaluateExpression(values, modifiedExpression);

                    if (originalResult != simplifiedResult)
                    {
                        areEquivalent = false;
                    }
                    if (originalResult)
                    {
                        isSatisfiableOriginal = true;
                        satisfiableOriginalValues.push_back(values);
                    }
                    if (simplifiedResult)
                    {
                        isSatisfiableSimplified = true;
                        satisfiableSimplifiedValues.push_back(values);
                    }
                    if (originalResult && simplifiedResult)
                    {
                        commonValues.push_back(values);
                    }
                }

                flag2 = flag3 = true;
                generateTruthValues(numVars);
                cout << "The original and simplified expressions are " << (areEquivalent ? "equivalent" : "not equivalent") << endl;
                cout << "The original expression is " << (isSatisfiableOriginal ? "satisfiable" : "not satisfiable") << endl;
                cout << "The simplified expression is " << (isSatisfiableSimplified ? "satisfiable" : "not satisfiable") << endl;

                if (!commonValues.empty())
                {
                    cout << "The common truth values that satisfy both expressions are: " << endl;
                    for (const auto& values : commonValues)
                    {
                        for (bool val : values)
                        {
                            cout << val << " ";
                        }
                        cout << endl;
                    }
                }
            }
        } while (!isSatisfiableOriginal || !isSatisfiableSimplified);
    }
    return 0;
}
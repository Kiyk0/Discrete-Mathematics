#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <string>

using namespace std;

// Global variables for flags and results vector
bool flag1 = true;  // General-purpose flag (not used in logic here)
bool flag2 = false; // Secondary flag to handle specific logic in evaluation
vector<bool> R;     // Stores intermediate results of evaluations
//int r = 0;

// Function to evaluate a single logical expression
// Parameters:
// - values: Truth values for the variables (A, B, C, etc.)
// - Expression: Logical expression to evaluate
bool evaluateExpression(vector<bool> values, string Expression)
{
    // Replace variables (A, B, etc.) in the expression with their truth values
    for (int i = 0; i < values.size(); i++)
    {
        char var = 'A' + i; // Variable name corresponding to index
        for (int j = 0; j < Expression.size(); j++)
        {
            // Replace variable with truth value considering negation
            if (Expression[j] == var)
            {
                if (Expression[j - 1] == '!' && j > 0)
                    Expression[j] = values[i] ? '0' : '1'; // Negated value
                else
                    Expression[j] = values[i] ? '1' : '0'; // Direct value
            }
        }
    }

    // Evaluate parentheses in the expression recursively
    while (Expression.find('(') != string::npos)
    {
        int start = Expression.find_last_of('(');                             // Find innermost '('
        int end = Expression.find(')', start);                                // Find matching ')'
        string subExpression = Expression.substr(start + 1, end - start - 1); // Extract inner expression

        // Evaluate the sub-expression using AND, OR, and literals
        bool result = false;
        if (subExpression.find('&') != string::npos) // AND operation
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
        else if (subExpression.find('|') != string::npos) // OR operation
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
        else if (subExpression.find('1') != string::npos) // Literal true
        {
            result = true;
        }

        // Replace evaluated sub-expression with its result
        Expression.replace(start, end - start + 1, result ? "1" : "0");
    }

    // If flag2 is set, update the results vector R
    if (flag2)
    {
        R.push_back(Expression == "1");
        flag2 = false;
    }
    return Expression == "1"; // Return final evaluation result
}

// Function to generate all possible truth value combinations for n variables
vector<vector<bool>> generateTruthValues(int numVars)
{
    vector<vector<bool>> truthValues;
    int numRows = pow(2, numVars); // Total rows in the truth table

    for (int i = 0; i < numRows; i++)
    {
        vector<bool> row;
        for (int j = numVars - 1; j >= 0; j--)
        {
            row.push_back((i & (1 << j)) != 0); // Generate truth values using bit manipulation
        }
        truthValues.push_back(row);
    }
    return truthValues;
}

// Function to print the truth table
void printTruthTable(vector<vector<bool>> truthValues, const vector<string> &premises, string conclusion)
{
    int numVars = truthValues[0].size(); // Number of variables
    int count = 1;                       // Counter for premise numbering

    // Print the header row
    for (int i = 0; i < numVars; i++)
    {
        cout << (char)('A' + i) << " "; // Variable names (A, B, C, ...)
    }
    for (const auto &premise : premises)
    {
        cout << count << " "; // Premise numbers
        count++;
    }
    cout << 'C' << endl; // Conclusion column

    // Print truth table rows
    for (const auto &row : truthValues)
    {
        for (bool val : row)
        {
            cout << val << " "; // Print truth values
        }

        vector<bool> premiseResults;
        for (const auto &premise : premises)
        {
            premiseResults.push_back(evaluateExpression(row, premise)); // Evaluate each premise
        }

        for (bool result : premiseResults)
        {
            cout << result << " "; // Print premise results
        }

        cout << evaluateExpression(row, conclusion) << endl; // Evaluate and print conclusion result
    }
}

// Function to check if a logical expression has balanced parentheses
bool isValidExpression(const std::string &expr)
{
    std::stack<char> s;
    for (size_t i = 0; i < expr.length(); ++i)
    {
        char ch = expr[i];
        if (ch == '(')
        {
            s.push(ch); // Push opening parentheses
        }
        else if (ch == ')')
        {
            if (s.empty() || s.top() != '(')
            {
                return false; // Unmatched closing parentheses
            }
            s.pop(); // Matched parentheses
        }
        else if (ch == '!' && (i == expr.length() - 1 || !(std::isalpha(expr[i + 1]) || expr[i + 1] == '(')))
        {
            return false; // Invalid negation placement
        }
    }
    return s.empty(); // Check if all parentheses are matched
}

int main()
{
    vector<string> premises; // List of premises
    string line, conclusion; // Input for premises and conclusion
    int numVars;             // Number of variables

    // Input prompt for number of variables
    cout << "Welcome, read the following steps to learn how to use the program." << endl;
    cout << "The correct form of expression should be enclosed by brackets -> ((!A&!B)|!C) , (B|C) -> (!A|!C)." << endl;
    cout << "You use can only use this version of AND -> '&', OR -> '|' and NOT -> '!'." << endl;
    cout << "Use uppercase alphabet in sequential order starting from A B C ... Z." << endl;
    cout << "Enter the number of variables: ";

    // Input validation for number of variables
    while (true)
    {
        cin >> numVars;
        if (cin.fail() || numVars <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number of variables. Please enter a positive integer: ";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    // Input premises
    cout << "Enter multiple premises on each line (type '0' to finish): ";
    while (getline(cin, line))
    {
        if (line == "0") // End input on '0'
            break;
        if (line.empty() || !isValidExpression(line))
        {
            cout << "Invalid expression." << endl;
            return 0;
        }
        premises.push_back(line);
    }

    // Input conclusion
    cout << "Enter the conclusion: ";
    getline(cin, conclusion);

    // Generate truth table
    vector<vector<bool>> truthValues = generateTruthValues(numVars);

    bool isSatisfiable = false; // Whether the formulas are satisfiable
    bool isValid = true;        // Whether the argument is valid

    // Evaluate truth table for satisfiability and validity
    for (const auto &values : truthValues)
    {
        bool premisesTrue = true;
        for (const auto &premise : premises)
        {
            if (!evaluateExpression(values, premise))
            {
                premisesTrue = false;
                break;
            }
        }
        if (premisesTrue && !evaluateExpression(values, conclusion))
        {
            isValid = false; // Counterexample found
        }
        if (premisesTrue && evaluateExpression(values, conclusion))
        {
            isSatisfiable = true; // Conclusion follows from premises
        }
    }

    // Print the truth table
    printTruthTable(truthValues, premises, conclusion);

    // Output final results
    cout << "The set of formulas is " << (isSatisfiable ? "Satisfiable" : "Not satisfiable") << endl;
    cout << "The argument is " << (isValid ? "Valid" : "Falsifiable") << endl;

    return 0; // Exit program
}

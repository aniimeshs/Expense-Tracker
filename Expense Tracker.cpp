#include <iostream>
#include <string>
#include <vector>
#include <ctime> // For getting current time
#include <fstream> // For file operations
#include <unordered_map>

using namespace std;

// Structure to represent an expense
struct Expense {
    double amount;
    int categoryId;
    string date;
    string notes;
};

// Structure to represent an income entry
struct Income {
    double amount;
    string date;
    string notes;
};

// Structure to represent a user
struct User {
    string username;
    string passwordHash;
    string salt;
    vector<Expense> expenses;
    vector<Income> incomes;
    double income;
    unordered_map<int, double> budgets; // Map of category IDs to budget amounts
};

// Map of category IDs to category names
unordered_map<int, string> categoryMap = {
    {1, "Groceries"},
    {2, "Rent"},
    {3, "Utilities"},
    {4, "Entertainment"},
    // Add more categories as needed
};

// Function to log an expense
void logExpense(User& user, double amount, int categoryId, const string& notes = "") {
    // Get current date
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    char date[11]; // Format: YYYY-MM-DD
    strftime(date, sizeof(date), "%Y-%m-%d", currentTime);

    // Create expense object
    Expense newExpense;
    newExpense.amount = amount;
    newExpense.categoryId = categoryId;
    newExpense.date = date;
    newExpense.notes = notes;

    // Add expense to user's expenses
    user.expenses.push_back(newExpense);

    cout << "Expense logged successfully." << endl;
}

// Function to set budget for a category
void setBudget(User& user, int categoryId, double amount) {
    // Update budget for the specified category
    user.budgets[categoryId] = amount;

    cout << "Budget set successfully for category: " << categoryMap[categoryId] << endl;
}

// Function to check if expense exceeds budget
void checkBudget(User& user, int categoryId, double expenseAmount) {
    if (user.budgets.find(categoryId) != user.budgets.end()) {
        double budgetAmount = user.budgets[categoryId];
        if (expenseAmount > budgetAmount) {
            cout << "Warning: Expense for category " << categoryMap[categoryId] << " exceeds budget!" << endl;
        }
    }
}

// Function to display expense summary
void displayExpenseSummary(const User& user) {
    cout << "Expense Summary:" << endl;
    for (const Expense& expense : user.expenses) {
        cout << "Date: " << expense.date << " | Amount: $" << expense.amount
             << " | Category: " << categoryMap[expense.categoryId]
             << " | Notes: " << expense.notes << endl;
    }
}

// Function to download expense data as CSV file
void downloadExpenseData(const User& user) {
    ofstream outFile("expense_data.csv");
    if (outFile.is_open()) {
        // Write header
        outFile << "Date,Amount,Category,Notes" << endl;
        
        // Write expense data
        for (const Expense& expense : user.expenses) {
            outFile << expense.date << ","
                    << expense.amount << ","
                    << categoryMap[expense.categoryId] << ","
                    << expense.notes << endl;
        }
        
        outFile.close();
        cout << "Expense data downloaded successfully as CSV file." << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
    }
}

// Function to display expense visualization (total expenses per category)
void displayExpenseVisualization(const User& user) {
    unordered_map<string, double> categoryTotals;
    for (const Expense& expense : user.expenses) {
        categoryTotals[categoryMap[expense.categoryId]] += expense.amount;
    }

    cout << "Expense Visualization:" << endl;
    cout << "Category\tTotal Expense" << endl;
    for (const auto& pair : categoryTotals) {
        cout << pair.first << "\t\t$" << pair.second << endl;
    }
}

int main() {
    User user1 {"user1", "hashed_password", "random_salt", {}, {}, 0.0, {}}; // Initialize user

    // Example: Set budget for groceries category (Category ID: 1)
    setBudget(user1, 1, 400.0);

    // Example: Log multiple expenses
    logExpense(user1, 50.0, 1, "Weekly grocery shopping");
    logExpense(user1, 80.0, 4, "Movie night");
    logExpense(user1, 100.0, 2, "Monthly rent");
    logExpense(user1, 30.0, 1, "Snacks");

    // Check if expenses exceed budget
    checkBudget(user1, 1, 50.0);
    checkBudget(user1, 4, 80.0);
    checkBudget(user1, 2, 100.0);
    checkBudget(user1, 1, 30.0);

    // Display expense summary
    displayExpenseSummary(user1);

    // Download expense data as CSV file
    downloadExpenseData(user1);

    // Display expense visualization
    displayExpenseVisualization(user1);

    return 0;
}

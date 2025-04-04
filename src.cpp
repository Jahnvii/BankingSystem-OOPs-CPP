#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Account {
protected:
    string name;
    int accountNumber;
    double balance;
    static int accCount;
    vector<string> transactions;

public:
    Account(string n, double b) : name(n), balance(b) {
        accountNumber = ++accCount;
    }
    virtual void deposit(double amount) {
        balance += amount;
        transactions.push_back("Deposited: " + to_string(amount));
        cout << "Deposited: " << amount << " | New Balance: " << balance << endl;
    }
    virtual bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            return false;
        }
        balance -= amount;
        transactions.push_back("Withdrawn: " + to_string(amount));
        cout << "Withdrawn: " << amount << " | New Balance: " << balance << endl;
        return true;
    }
    virtual void display() const {
        cout << "Account Number: " << accountNumber << " | Name: " << name << " | Balance: " << balance << endl;
    }
    void showTransactionHistory() const {
        cout << "Transaction History for Account " << accountNumber << ":" << endl;
        for (const auto& t : transactions) {
            cout << t << endl;
        }
    }
    int getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
};

int Account::accCount = 1000;

class SavingsAccount : public Account {
    double interestRate;
public:
    SavingsAccount(string n, double b, double rate) : Account(n, b), interestRate(rate) {}
    void addInterest() {
        double interest = balance * interestRate / 100;
        balance += interest;
        transactions.push_back("Interest Added: " + to_string(interest));
        cout << "Interest Added: " << interest << " | New Balance: " << balance << endl;
    }
};

class Bank {
    vector<Account*> accounts;
    string adminUsername = "admin";
    string adminPassword = "admin123";
    bool isAdmin = false;

public:
    bool adminLogin() {
        string inputUsername, inputPassword;
        cout << "Enter Admin Username: ";
        cin >> inputUsername;
        cout << "Enter Admin Password: ";
        cin >> inputPassword;
        if (inputUsername == adminUsername && inputPassword == adminPassword) {
            cout << "Admin Access Granted!" << endl;
            isAdmin = true;
            return true;
        } else {
            cout << "Incorrect Username or Password!" << endl;
            return false;
        }
    }
    void createAccount(string name, double initialBalance, bool isSavings, double interestRate = 0) {
        if (!isAdmin) {
            cout << "Access Denied! Only admin can create accounts." << endl;
            return;
        }
        Account* newAccount;
        if (isSavings) {
            newAccount = new SavingsAccount(name, initialBalance, interestRate);
        } else {
            newAccount = new Account(name, initialBalance);
        }
        accounts.push_back(newAccount);
        cout << "Account created successfully! Your Account Number is: " << newAccount->getAccountNumber() << endl;
    }
    void deleteAccount(int accNo) {
        if (!isAdmin) {
            cout << "Access Denied! Only admin can delete accounts." << endl;
            return;
        }
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if ((*it)->getAccountNumber() == accNo) {
                delete *it;
                accounts.erase(it);
                cout << "Account deleted successfully!" << endl;
                return;
            }
        }
        cout << "Account not found!" << endl;
    }
    void deposit(int accNo, double amount) {
        for (auto acc : accounts) {
            if (acc->getAccountNumber() == accNo) {
                acc->deposit(amount);
                return;
            }
        }
        cout << "Account not found!" << endl;
    }
    void withdraw(int accNo, double amount) {
        for (auto acc : accounts) {
            if (acc->getAccountNumber() == accNo) {
                acc->withdraw(amount);
                return;
            }
        }
        cout << "Account not found!" << endl;
    }
    void displayAccount(int accNo) {
        for (auto acc : accounts) {
            if (acc->getAccountNumber() == accNo) {
                acc->display();
                return;
            }
        }
        cout << "Account not found!" << endl;
    }
    ~Bank() {
        for (auto acc : accounts) delete acc;
    }
};

int main() {
    Bank bank;
    int choice, accNo;
    string name;
    double amount, interestRate;
    bool isSavings;
    
    cout << "Admin login required to proceed." << endl;
    if (!bank.adminLogin()) {
        return 0;
    }
    
    while (true) {
        cout << "\nBank Management System";
        cout << "\n1. Open an Account";
        cout << "\n2. Show Account Details";
        cout << "\n3. Deposit Money";
        cout << "\n4. Withdraw Money";
        cout << "\n5. Delete an Account";
        cout << "\n6. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Initial Balance: ";
                cin >> amount;
                cout << "Savings Account? (1 for Yes, 0 for No): ";
                cin >> isSavings;
                if (isSavings) {
                    cout << "Enter Interest Rate: ";
                    cin >> interestRate;
                }
                bank.createAccount(name, amount, isSavings, interestRate);
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accNo;
                bank.displayAccount(accNo);
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                bank.deposit(accNo, amount);
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                bank.withdraw(accNo, amount);
                break;
            case 5:
                cout << "Enter Account Number to Delete: ";
                cin >> accNo;
                bank.deleteAccount(accNo);
                break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

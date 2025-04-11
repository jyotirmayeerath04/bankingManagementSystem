#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;
class Transaction {
private:
    string type; // "Deposit" or "Withdraw"
    double amount;
    string timestamp;

public:
    Transaction(string t, double a) : type(t), amount(a) {
        // Get current time
        time_t now = time(0);
        timestamp = ctime(&now);
    }

    void display() const {
        cout << setw(10) << type << " | " << setw(10) << amount << " | " << timestamp;
    }
};

class Account {
private:
    string accountNumber;
    string accountHolderName;
    string password;
    double balance;
    vector<Transaction> transactions;

public:
    Account(string accNumber, string accHolderName, string pwd, double initialBalance)
        : accountNumber(accNumber), accountHolderName(accHolderName), password(pwd), balance(initialBalance) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transactions.emplace_back("Deposit", amount);
            cout << "Deposited " << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Invalid deposit amount!" << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            transactions.emplace_back("Withdraw", amount);
            cout << "Withdrew " << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Invalid withdrawal amount or insufficient balance!" << endl;
        }
    }

    void addInterest(double rate) {
        double interest = balance * rate / 100;
        balance += interest;
        transactions.emplace_back("Interest", interest);
        cout << "Interest added: " << interest << ". New balance: " << balance << endl;
    }

    void display() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }

    void displayTransactionHistory() const {
        cout << "Transaction History for Account: " << accountNumber << endl;
        cout << "Type       | Amount     | Timestamp" << endl;
        for (const auto& transaction : transactions) {
            transaction.display();
        }
    }

    bool checkPassword(string pwd) const {
        return password == pwd;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }
};

class Bank {
private:
    vector<Account> accounts;

public:
    void createAccount(string accNumber, string accHolderName, string pwd, double initialBalance) {
        accounts.emplace_back(accNumber, accHolderName, pwd, initialBalance);
        cout << "Account created successfully!" << endl;
    }

    void deleteAccount(string accNumber) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if (it->getAccountNumber() == accNumber) {
                accounts.erase(it);
                cout << "Account deleted successfully!" << endl;
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    Account* findAccount(const string& accNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accNumber) {
                return &account;
            }
        }
        return nullptr;
    }

    void deposit(const string& accNumber, double amount) {
        Account* account = findAccount(accNumber);
        if (account) {
            account->deposit(amount);
        } else {
            cout << "Account not found!" << endl;
        }
    }

   void withdraw(const string& accNumber, double amount) {
    Account* account = findAccount(accNumber);
    if (account) {
        string password;
        cout << "Enter Password: ";
        cin >> password;
        if (account->checkPassword(password)) {
            account->withdraw(amount); // Proceed with withdrawal if password is correct
        } else {
            cout << "Incorrect Password! Withdrawal denied." << endl;
        }
    } else {
        cout << "Account not found!" << endl;
    }
	}

    void transfer(const string& fromAccNumber, const string& toAccNumber, double amount) {
        Account* fromAccount = findAccount(fromAccNumber);
        Account* toAccount = findAccount(toAccNumber);
        if (fromAccount && toAccount) {
            if (fromAccount->getBalance() >= amount) {
                fromAccount->withdraw(amount);
                toAccount->deposit(amount);
                cout << "Transfer successful!" << endl;
            } else {
                cout << "Insufficient balance for transfer!" << endl;
            }
        } else {
            cout << "One or both accounts not found!" << endl;
        }
    }

    void addInterestToAllAccounts(double rate) {
        for (auto& account : accounts) {
            account.addInterest(rate);
        }
        cout << "Interest added to all accounts!" << endl;
    }

    void displayAllAccounts() const {
        cout << "All Accounts in the Bank:" << endl;
        for (const auto& account : accounts) {
            account.display();
            cout << "-------------------------" << endl;
        }
    }
};

int main() {
    Bank bank;
    int choice;
    string accNumber, accHolderName, password, fromAccNumber, toAccNumber;
    double amount, rate;

    do {
        cout << "\nBank Management System\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Transfer Funds\n";
        cout << "5. Display Account Details\n";
        cout << "6. Display Transaction History\n";
        cout << "7. Delete Account\n";
        cout << "8. Add Interest to All Accounts\n";
        cout << "9. Display All Accounts (Admin)\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Account Number: ";
                cin >> accNumber;
                cout << "Enter Account Holder Name: ";
                cin.ignore();
                getline(cin, accHolderName);
                cout << "Enter Password: ";
                cin >> password;
                cout << "Enter Initial Balance: ";
                cin >> amount;
                bank.createAccount(accNumber, accHolderName, password, amount);
                break;

            case 2:
                cout << "Enter Account Number: ";
                cin >> accNumber;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                bank.deposit(accNumber, amount);
                break;

            case 3:
                cout << "Enter Account Number: ";
                cin >> accNumber;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                bank.withdraw(accNumber, amount);
                break;

            case 4:
                cout << "Enter From Account Number: ";
                cin >> fromAccNumber;
                cout << "Enter To Account Number: ";
                cin >> toAccNumber;
                cout << "Enter Amount to Transfer: ";
                cin >> amount;
                bank.transfer(fromAccNumber, toAccNumber, amount);
                break;

            case 5:
                cout << "Enter Account Number: ";
                cin >> accNumber;
                {
                    Account* account = bank.findAccount(accNumber);
                    if (account) {
                        cout << "Enter Password: ";
                        cin >> password;
                        if (account->checkPassword(password)) {
                            account->display();
                        } else {
                            cout << "Incorrect Password!" << endl;
                        }
                    } else {
                        cout << "Account not found!" << endl;
                    }
                }
                break;

            case 6:
                cout << "Enter Account Number: ";
                cin >> accNumber;
                {
                    Account* account = bank.findAccount(accNumber);
                    if (account) {
                        cout << "Enter Password: ";
                        cin >> password;
                        if (account->checkPassword(password)) {
                            account->displayTransactionHistory();
                        } else {
                            cout << "Incorrect Password!" << endl;
                        }
                    } else {
                        cout << "Account not found!" << endl;
                    }
                }
                break;

            case 7:
                cout << "Enter Account Number: ";
                cin >> accNumber;
                bank.deleteAccount(accNumber);
                break;

            case 8:
                cout << "Enter Interest Rate: ";
                cin >> rate;
                bank.addInterestToAllAccounts(rate);
                break;

            case 9:
                bank.displayAllAccounts();
                break;

            case 10:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 10);

    return 0;
}

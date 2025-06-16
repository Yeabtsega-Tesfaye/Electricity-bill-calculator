#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

const int MAX_CUSTOMERS = 100;

class Usage
{
public:
    double kWh; // electricity usage in kilowatt-hours

    Usage() : kWh(0.0) {}
};

class Bill
{
public:
    double amount;
    string paymentStatus;

    Bill() : amount(0.0), paymentStatus("Unpaid") {}

    void calculate(double usage)
    {
        // The tariff rates set by Ethiopian Electric Utility(EEU)
        double rate1 = 0.2733;    // 0 - 50 kWh
        double rate2 = 0.3568;    // 51 - 100 kWh
        double rate3 = 0.7568;    // 101 - 200 kWh
        double rate4 = 1.3700;    // above 200 kWh
        double fixedCharge = 5.0; // fixed monthly service charge
        double VAT = 0.15;        // 15% value added tax

        if (usage <= 50)
        {
            amount = usage * rate1;
        }
        else if (usage <= 100)
        {
            amount = (50 * rate1) + ((usage - 50) * rate2);
        }
        else if (usage <= 200)
        {
            amount = (50 * rate1) + (50 * rate2) + ((usage - 100) * rate3);
        }
        else
        {
            amount = (50 * rate1) + (50 * rate2) + (100 * rate3) + ((usage - 200) * rate4);
        }

        amount += fixedCharge;              // add fixed charge
        amount += amount * VAT;             // add VAT
        amount = round(amount * 100) / 100; // round to two decimal places
    }
};

class Customer
{
public:
    int accountNumber;
    string name;
    Usage usage;
    Bill bill;

    Customer() : accountNumber(0), name("") {}
};

class ElectricityBillCalculator
{
private:
    Customer *customers[MAX_CUSTOMERS];
    int customerCount;

public:
    ElectricityBillCalculator() : customerCount(0) {}

    void addCustomer()
    {
        if (customerCount >= MAX_CUSTOMERS)
        {
            cout << "Error: Maximum customer limit reached.\n";
            return;
        }

        customers[customerCount] = new Customer;
        cout << "Enter account number: ";
        cin >> customers[customerCount]->accountNumber;
        cin.ignore();
        cout << "Enter customer name: ";
        getline(cin, customers[customerCount]->name);
        cout << "Enter electricity usage (kWh): ";
        cin >> customers[customerCount]->usage.kWh;

        customers[customerCount]->bill.calculate(customers[customerCount]->usage.kWh);
        customers[customerCount]->bill.paymentStatus = "Unpaid";
        customerCount++;
        cout << "Customer added successfully!\n";
    }

    void deleteCustomer(int accountNumber)
    {
        bool found = false;
        for (int i = 0; i < customerCount; i++)
        {
            if (customers[i]->accountNumber == accountNumber)
            {
                delete customers[i];
                for (int j = i; j < customerCount - 1; j++)
                {
                    customers[j] = customers[j + 1];
                }
                customerCount--;
                found = true;
                cout << "Customer deleted successfully!\n";
                break;
            }
        }
        if (!found)
        {
            cout << "Customer not found.\n";
        }
    }

    void searchCustomer()
    {
        string keyword;
        cin.ignore();
        cout << "Enter account number or name to search: ";
        getline(cin, keyword);

        for (int i = 0; i < customerCount; i++)
        {
            if (to_string(customers[i]->accountNumber) == keyword || customers[i]->name == keyword)
            {
                displayCustomer(customers[i]);
                return;
            }
        }

        cout << "Customer not found.\n";
    }

    void modifyCustomer()
    {
        int accountNumber;
        cout << "Enter account number to modify: ";
        cin >> accountNumber;

        for (int i = 0; i < customerCount; i++)
        {
            if (customers[i]->accountNumber == accountNumber)
            {
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, customers[i]->name);
                cout << "Enter new usage (kWh): ";
                cin >> customers[i]->usage.kWh;

                customers[i]->bill.calculate(customers[i]->usage.kWh);
                cout << "Customer details updated successfully!\n";
                return;
            }
        }
        cout << "Customer not found.\n";
    }

    void viewAllCustomers()
    {
        if (customerCount == 0)
        {
            cout << "No customers found.\n";
            return;
        }

        for (int i = 0; i < customerCount; i++)
        {
            displayCustomer(customers[i]);
            cout << "-----------------------------------\n";
        }
    }

    void saveToFile()
    {
        ofstream file("customers.txt");
        if (!file)
        {
            cout << "Error opening file for writing.\n";

            return;
        }

        for (int i = 0; i < customerCount; i++)
        {
            file << customers[i]->accountNumber << "\n"
                 << customers[i]->name << "\n"
                 << customers[i]->usage.kWh << "\n"
                 << customers[i]->bill.amount << "\n"
                 << customers[i]->bill.paymentStatus << "\n";
        }
        file.close();
        cout << "Data saved successfully.\n";
    }

    void loadFromFile()
    {
        ifstream file("customers.txt");
        if (!file)
        {
            cout << "Error opening file for reading.\n";
            return;
        }

        while (!file.eof() && customerCount < MAX_CUSTOMERS)
        {
            customers[customerCount] = new Customer;
            file >> customers[customerCount]->accountNumber;
            file.ignore();
            getline(file, customers[customerCount]->name);
            file >> customers[customerCount]->usage.kWh;
            file >> customers[customerCount]->bill.amount;
            file.ignore();
            getline(file, customers[customerCount]->bill.paymentStatus);

            if (file)
            {
                customerCount++;
            }
            else
            {
                delete customers[customerCount];
            }
        }
        file.close();
        cout << "Data loaded successfully.\n";
    }

    void displayCustomer(Customer *customer)
    {
        cout << "Account Number: " << customer->accountNumber << "\n";
        cout << "Name: " << customer->name << "\n";
        cout << "Usage: " << customer->usage.kWh << " kWh\n";
        cout << "Bill Amount: " << customer->bill.amount << " ETB\n";
        cout << "Payment Status: " << customer->bill.paymentStatus << "\n";
    }

    ~ElectricityBillCalculator()
    {
        for (int i = 0; i < customerCount; i++)
        {
            delete customers[i];
        }
    }
    void displayUsageGraph()
    {
        for (int i = 0; i < customerCount; i++)
        {
            cout << customers[i]->name << ": ";
            int bars = customers[i]->usage.kWh / 10;
            for (int j = 0; j < bars; j++)
            {
                cout << "|";
            }
            cout << " (" << customers[i]->usage.kWh << " kWh)\n";
        }
    }
};

bool login()
{
    string username, password;
    cout << "\nEnter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    if (username == "admin" && password == "1234")
    {
        cout << "Login successful.\n";
        return true;
    }
    else
    {
        cout << "Invalid credentials.\n";
        return false;
    }
}

int main()
{
    cout << "\nW\tE\tL\tC\tO\tM\tE\t!!\n\n"
         << "Login to continue\n";

    if (!login())
    {
        return 0;
    }

    ElectricityBillCalculator system;
    int choice;
    do
    {
        cout << "\n*******************************\n";
        cout << "* Electricity Bill Calculator *\n";
        cout << "*******************************\n";
        cout << "1. Add Customer\n";
        cout << "2. Delete Customer\n";
        cout << "3. Search Customer\n";
        cout << "4. Modify Customer\n";
        cout << "5. View All Customers\n";
        cout << "6. View usage graphically(simple bar chart)\n";
        cout << "7. Save Data to File\n";
        cout << "8. Load Data from File\n";
        cout << "9. Exit\n";
        cout << "-----------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "-----------------------------------\n";

        switch (choice)
        {
        case 1:
            system.addCustomer();
            break;
        case 2:
        {
            int accNo;
            cout << "Enter account number to delete: ";
            cin >> accNo;
            system.deleteCustomer(accNo);
            break;
        }
        case 3:
            system.searchCustomer();
            break;
        case 4:
            system.modifyCustomer();
            break;
        case 5:
            system.viewAllCustomers();
            break;
        case 6:
            system.displayUsageGraph();
            break;
        case 7:
            system.saveToFile();
            break;
        case 8:
            system.loadFromFile();
            break;
        case 9:
            cout << "Exiting the program...\n\n\n"
                 << "\nThis Electricity Bill Calculator have been Done By : second year Woldia univeristy students\n\n"
                 << "  Full-Name :  \t\t ID-Number:\n\n"
                 << "1, YEABTSEGA TESFAYE ...............161299\n"
                 << "2, TESHOME SISAY ...................161207\n"
                 << "3, ETSEGENET DAGNACHEWU ............160512\n"
                 << "4, GETASIL SETEGN ..................160614\n"
                 << "5, TAMENECH MISSA ..................161169\n"
                 << "6, SAMRAWIT MOLLA ..................161055\n"
                 << "\n\nSUBMITTED TO :   Mr. GETASEW ABEBA\n"
                 << "SUBMISSION DATE : 30/04/2017 E.C\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);

    return 0;
}
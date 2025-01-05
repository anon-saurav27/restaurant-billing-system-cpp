#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h> // for system("cls") for clearing screen

using namespace std;

// Function to clear screen
void clearScreen() {
    system("cls");
}

// Function to display a border
void displayBorder() {
    cout << "**************************************************" << endl;
}

// Function to display a header
void displayHeader(const string& title) {
    displayBorder();
    cout << "*              " << title << "               *" << endl;
    displayBorder();
}

// Class to represent each menu item
class MenuItem {
private:
    string name;
    float price;

public:
    MenuItem() {}
    MenuItem(string n, float p) : name(n), price(p) {}

    string getName() const { return name; }
    float getPrice() const { return price; }
};

// Class to manage the menu
class Menu {
private:
    static const int MAX_ITEMS = 100; // Maximum number of menu items
    MenuItem items[MAX_ITEMS];
    int menuSize;

public:
    Menu() : menuSize(0) {}

    void addItem(const MenuItem& item) {
        if (menuSize < MAX_ITEMS) {
            items[menuSize++] = item;
        } else {
            cout << "Menu is full! Cannot add more items.\n";
        }
    }

    void editItem(int index, const MenuItem& newItem) {
        if (index >= 0 && index < menuSize) {
            items[index] = newItem;
        } else {
            cout << "Invalid index!\n";
        }
    }

    void deleteItem(int index) {
        if (index >= 0 && index < menuSize) {
            for (int i = index; i < menuSize - 1; ++i) {
                items[i] = items[i + 1];
            }
            menuSize--;
        } else {
            cout << "Invalid index!\n";
        }
    }

    void displayMenu() const {
        displayHeader("MENU");
        cout<<"Item Name\t\tPrice"<<endl;
        for (int i = 0; i < menuSize; ++i) {
            cout << i + 1 << ". " << items[i].getName() << " \t\t- Rs. " << items[i].getPrice() << endl;
        }
        displayBorder();
    }

    int getMenuSize() const {
        return menuSize;
    }

    const MenuItem* getMenuItems() const {
        return items;
    }
};

// Function prototypes
void saveMenuToFile(const Menu& menu);
void loadMenuFromFile(Menu& menu);
float generateBill(const Menu& menu, const int order[], const int quantity[], int orderSize, const string& customerName);
void generateBillAndPlaceOrder(Menu& menu);
void registerUser();
bool loginUser();
string getPasswordInput();

// Function to save user credentials to file
void saveUserCredentials(const string& username, const string& password) {
    ofstream file("login.txt", ios::app);
    if (file.is_open()) {
        file << username << " " << password << endl;
        file.close();
    } else {
        cout << "Error: Unable to open file.\n";
    }
}

// Function to register a new user
// Function to register a new user
void registerUser() {
    string username, password, confirm;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    password = getPasswordInput();
    cout << "Confirm password: ";
    confirm = getPasswordInput();
    if (password == confirm) {
        saveUserCredentials(username, password);
        cout << "Registration successful!\n";
    } else {
        cout << "Error: Passwords do not match.\n";
    }
}

// Function to check if a user exists in the login file
bool loginUser() {
    string username, password, u, p;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    password = getPasswordInput();
    ifstream file("login.txt");
    if (file.is_open()) {
        while (file >> u >> p) {
            if (u == username && p == password) {
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}

// Function to get password input without displaying characters
string getPasswordInput() {
    const char BACKSPACE = 8;
    const char ENTER = 13;
    string password;
    char ch;
    while ((ch = getch()) != ENTER) {
        if (ch == BACKSPACE) {
            if (password.length() > 0) {
                cout << "\b \b";
                password.erase(password.length() - 1);
            }
        } else {
            cout << '*';
            password += ch;
        }
    }
    cout << endl;
    return password;
}


// Function to save menu to file
void saveMenuToFile(const Menu& menu) {
    ofstream file("menu.txt");
    if (file.is_open()) {
        const MenuItem* items = menu.getMenuItems();
        for (int i = 0; i < menu.getMenuSize(); ++i) {
            file << items[i].getName() << " " << items[i].getPrice() << endl;
        }
        file.close();
    }
}

// Function to load menu from file
void loadMenuFromFile(Menu& menu) {
    ifstream file("menu.txt");
    if (file.is_open()) {
        string name;
        float price;
        while (file >> name >> price) {
            menu.addItem(MenuItem(name, price));
        }
        file.close();
    }
}

// Function to generate bill
float generateBill(const Menu& menu, const int order[], const int quantity[], int orderSize, const string& customerName) {
    const float VAT_RATE = 0.13; // 13% VAT rate
    const float DISCOUNT_RATE = 0.10; // 10% discount rate
    
    const MenuItem* items = menu.getMenuItems();
    if (orderSize == 0) {
        cout << "No items ordered!\n";
        return 0.0; // No items ordered, so return 0 as total
    }

    float total = 0.0;
    clearScreen();
    cout << "Generating Bill...\n";
    displayBorder();
    cout << "Name: " << customerName << endl;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    cout << "Date: " << ltm->tm_mday << "/" << ltm->tm_mon + 1 << "/" << 1900 + ltm->tm_year << endl;
    displayBorder();
    cout << "-------------------------------------------\n";
    cout << "Items\t\tQuantity\tPrice\n";
    cout << "-------------------------------------------\n";
    for (int i = 0; i < orderSize; ++i) {
        int index = order[i] - 1;
        if (index >= 0 && index < menu.getMenuSize()) {
            cout << items[index].getName() << "\t\t" << quantity[i] << "\t\tRs." << items[index].getPrice() * quantity[i] << endl;
            total += items[index].getPrice() * quantity[i];
        }
    }
    cout << "-------------------------------------------\n";
    cout << "SubTotal:    \t\t\tRs." << total << endl;
   
   	// Apply discount
    float totalWithDiscount = total * (1 - DISCOUNT_RATE);
    // Apply VAT
    float totalWithVAT = totalWithDiscount * (1 + VAT_RATE);
    
    // Calculate discount amount
    float discountAmount = total*DISCOUNT_RATE;
    
    // Calculate VAT amount
    float vatAmount = (total-discountAmount)*VAT_RATE;
    
    
    
    cout << "Discount (10%):\t\t\tRs." << discountAmount << endl;
    cout << "VAT (13%):\t\t\tRs." << vatAmount << endl;
    
    
    cout << "-------------------------------------------\n";
    cout << "Total:\t\t\t\tRs." << totalWithVAT << endl;
    cout << "-------------------------------------------\n";
    return totalWithDiscount;
}

// Function to generate bill and place order
void generateBillAndPlaceOrder(Menu& menu) {
    const int MAX_ORDER_SIZE = 100; // Maximum number of items in an order
    int order[MAX_ORDER_SIZE];
    int quantity[MAX_ORDER_SIZE];
    int orderSize = 0;
    char choice;
    string customerName;
    float total = 0.0;
    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, customerName);
    do {
        clearScreen();
        menu.displayMenu();
        cout << "Enter index of item to add to order (0 to finish): ";
        int index;
        cin >> index;
        if (index >= 1 && index <= menu.getMenuSize() && orderSize < MAX_ORDER_SIZE) {
            order[orderSize] = index;
            cout << "Enter quantity: ";
            cin >> quantity[orderSize];
            total += menu.getMenuItems()[index - 1].getPrice() * quantity[orderSize]; // Calculate total price
            orderSize++;
            cout << "Item added to order.\n";
            getch();
            clearScreen();
        } else if (index == 0) {
            if (orderSize == 0) {
                cout << "No items ordered!\n";
            } else {
                // Calculate total price with VAT and discount, and display bill
                total = generateBill(menu, order, quantity, orderSize, customerName);
                cout << "Do you want to place this order? (y/n): ";
                cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    cout << "Order placed.\n";
                    // Additional logic to handle placing order (e.g., save to file)
                    orderSize = 0; // Reset order size after placing order
                    total = 0.0;
                }
            }
        } else {
            cout << "Invalid index!\n";
        }
    } while (choice != 'n' && choice != 'N');
}

int main() {
    int choice;
    do {
        clearScreen();
        displayHeader("RESTAURANT BILLING SYSTEM");
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                if (loginUser()) {
                    cout << "Login successful!\n";
                    // Proceed to the main menu
                    Menu menu;
                    loadMenuFromFile(menu);
                    int mainChoice;
                    do {
                        clearScreen();
                        displayHeader("MAIN MENU");
                        cout << "1. Insert Menu Item\n";
                        cout << "2. Edit Menu Item\n";
                        cout << "3. Delete Menu Item\n";
                        cout << "4. Display Menu\n";
                        cout << "5. Generate Bill\n";
                        cout << "6. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> mainChoice;
                        switch (mainChoice) {
                            case 1: {
                string name;
                float price;
                clearScreen();
                menu.displayMenu();
                cout<<"Add Items On Current Menu:"<<endl<<endl;
                cout << "Enter name of item: ";
                cin >> name;
                cout << "Enter price of item: ";
                cin >> price;
                menu.addItem(MenuItem(name, price));
                saveMenuToFile(menu);
                break;
            }
            case 2: {
                int index;
                clearScreen();
                menu.displayMenu();
                cout << "Enter index of item to edit: ";
                cin >> index;
                if (index >= 1 && index <= menu.getMenuSize()) {
                    string name;
                    float price;
                    cout << "Enter new name of item: ";
                    cin >> name;
                    cout << "Enter new price of item: ";
                    cin >> price;
                    menu.editItem(index - 1, MenuItem(name, price));
                    saveMenuToFile(menu);
                } else {
                    cout << "Invalid index!\n";
                }
                break;
            }
            case 3: {
                int index;
                 clearScreen();
                menu.displayMenu();
                cout << "Enter index of item to delete: ";
                cin >> index;
                menu.deleteItem(index - 1);
                saveMenuToFile(menu);
                break;
            }
            case 4: {
                clearScreen();
                menu.displayMenu();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 5: {
                generateBillAndPlaceOrder(menu);
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
                            case 6:
                                cout << "Logging out...\n";
                                break;
                            default:
                                cout << "Invalid choice! Please try again.\n";
                        }
                    } while (mainChoice != 6);
                } else {
                    cout << "Invalid username or password!\n";
                    getch();
                }
                break;
            }
            case 2:
                registerUser();
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// =========================== STRUCTURES ===========================
struct User {
    string userID, password;
};

struct Admin {
    string userID, password;
};

struct Book {
    string title;
    int copies;
};

struct IssuedBook {
    string title, borrower;
};

// =========================== GLOBAL VECTORS ===========================
vector<User> users;
vector<Admin> admins;
vector<Book> books;
vector<IssuedBook> issuedBooks;

// =========================== FILE HANDLING FUNCTIONS ===========================

// Load users from CSV file
void loadUsers() {
    ifstream file("student.csv");
    if (!file) return;
    string line, userID, password;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, userID, ',');
        getline(ss, password, ',');
        users.push_back({userID, password});
    }
    file.close();
}

// Save users to CSV file
void saveUsers() {
    ofstream file("student.csv");
    file << "UserID,Password\n";
    for (auto &u : users) {
        file << u.userID << "," << u.password << "\n";
    }
    file.close();
}

// Load admin credentials from CSV file
void loadAdmins() {
    ifstream file("admin.csv");
    if (!file) return;
    string line, userID, password;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, userID, ',');
        getline(ss, password, ',');
        admins.push_back({userID, password});
    }
    file.close();
}

// Load books from CSV file
void loadBooks() {
    ifstream file("books.csv");
    if (!file) return;
    string line, title;
    int copies;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, title, ',');
        ss >> copies;
        books.push_back({title, copies});
    }
    file.close();
}

// Save books to CSV file
void saveBooks() {
    ofstream file("books.csv");
    file << "BookTitle,Copies\n";
    for (auto &b : books) {
        file << b.title << "," << b.copies << "\n";
    }
    file.close();
}

// Load issued books from CSV file
void loadIssuedBooks() {
    ifstream file("issued_books.csv");
    if (!file) return;
    string line, title, borrower;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, title, ',');
        getline(ss, borrower, ',');
        issuedBooks.push_back({title, borrower});
    }
    file.close();
}

// Save issued books to CSV file
void saveIssuedBooks() {
    ofstream file("issued_books.csv");
    file << "BookTitle,Borrower\n";
    for (auto &ib : issuedBooks) {
        file << ib.title << "," << ib.borrower << "\n";
    }
    file.close();
}

// =========================== USER FUNCTIONS ===========================

// Sign-up function
void signUp() {
    string newUserID, newPassword;
    cout << "Enter new User ID: ";
    cin >> newUserID;

    for (auto &u : users) {
        if (u.userID == newUserID) {
            cout << "User ID already exists! Try again.\n";
            return;
        }
    }

    cout << "Enter new Password: ";
    cin >> newPassword;
    users.push_back({newUserID, newPassword});
    saveUsers();
    cout << "Sign-Up Successful! You can now log in.\n";
}

// Authenticate student login
User* authenticateStudent(string userID, string password) {
    for (auto &u : users) {
        if (u.userID == userID && u.password == password) return &u;
    }
    return nullptr;
}

// Authenticate admin login
Admin* authenticateAdmin(string userID, string password) {
    for (auto &a : admins) {
        if (a.userID == userID && a.password == password) return &a;
    }
    return nullptr;
}

// Show all available books
void showAvailableBooks() {
    cout << "\nAvailable Books:\n";
    for (auto &b : books) {
        if (b.copies > 0) {
            cout << "- " << b.title << " (" << b.copies << " available)\n";
        }
    }
}

// Rent a book
void rentBook(User *user) {
    cin.ignore(); // Fix input issue
    string bookName;
    cout << "Enter book title: ";
    getline(cin, bookName);

    for (auto &b : books) {
        if (b.title == bookName && b.copies > 0) {
            b.copies--;
            issuedBooks.push_back({bookName, user->userID});
            saveBooks();
            saveIssuedBooks();
            cout << "Book issued successfully!\n";
            return;
        }
    }
    cout << "Book out of stock!\n";
}

// Return a book
void returnBook(User *user) {
    cin.ignore(); // Fix input issue
    string bookName;
    cout << "Enter book title: ";
    getline(cin, bookName);

    for (auto it = issuedBooks.begin(); it != issuedBooks.end(); ++it) {
        if (it->title == bookName && it->borrower == user->userID) {
            issuedBooks.erase(it);
            for (auto &b : books) {
                if (b.title == bookName) {
                    b.copies++;
                    break;
                }
            }
            saveBooks();
            saveIssuedBooks();
            cout << "Book returned successfully!\n";
            return;
        }
    }
    cout << "You didn't rent this book!\n";
}

// =========================== ADMIN FUNCTIONS ===========================

// Edit books in the library
void editBooks() {
    cin.ignore();
    string bookName;
    int newQuantity;
    cout << "Enter book title: ";
    getline(cin, bookName);
    cout << "Enter new quantity: ";
    cin >> newQuantity;

    for (auto &b : books) {
        if (b.title == bookName) {
            b.copies = newQuantity;
            saveBooks();
            cout << "Book quantity updated!\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

// View issued books
void viewIssuedBooks() {
    cout << "\nIssued Books:\n";
    for (auto &ib : issuedBooks) {
        cout << ib.title << " -> " << ib.borrower << "\n";
    }
}

// =========================== MAIN MENU ===========================

int main() {
    loadUsers();
    loadAdmins();
    loadBooks();
    loadIssuedBooks();

    while (true) {
        int option;
        cout << "\n1. Login as Student\n2. Login as Admin\n3. Sign Up\n4. Exit\nChoose an option: ";
        cin >> option;

        if (option == 1 || option == 2) {
            string userID, password;
            cout << "Enter User ID: ";
            cin >> userID;
            cout << "Enter Password: ";
            cin >> password;

            if (option == 1) {
                User* student = authenticateStudent(userID, password);
                if (student) {
                    int choice;
                    while (true) {
                        cout << "\n1. Show Books\n2. Rent Book\n3. Return Book\n4. Logout\nChoice: ";
                        cin >> choice;
                        if (choice == 1) showAvailableBooks();
                        else if (choice == 2) rentBook(student);
                        else if (choice == 3) returnBook(student);
                        else break;
                    }
                }
            } else {
                Admin* admin = authenticateAdmin(userID, password);
                if (admin) {
                    int choice;
                    while (true) {
                        cout << "\n1. Edit Books\n2. View Issued Books\n3. Show Books\n4. Logout\nChoice: ";
                        cin >> choice;
                        if (choice == 1) editBooks();
                        else if (choice == 2) viewIssuedBooks();
                        else if (choice == 3) showAvailableBooks();
                        else break;
                    }
                }
            }
        } else if (option == 3) signUp();
        else break;
    }
    return 0;
}

//Thank you ChatGPT for staying up with me and helping me debug till 2am 
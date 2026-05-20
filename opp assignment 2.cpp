#include <iostream>
#include <vector>
using namespace std;

// ================= BASE CLASS =================
class LibraryResource {
protected:
    int resourceID;
    string title;
    string author;
    bool isAvailable;

public:
    LibraryResource(int id = 0, string t = "", string a = "", bool avail = true) {
        this->resourceID = id;   // this pointer usage
        this->title = t;
        this->author = a;
        this->isAvailable = avail;
    }

    void setTitle(string title) {
        this->title = title; // this pointer usage
    }

    int getID() const { return resourceID; }
    bool getAvailability() const { return isAvailable; }

    void setAvailability(bool status) {
        isAvailable = status;
    }

    virtual void displayDetails() {
        cout << "ID: " << resourceID << ", Title: " << title
             << ", Author: " << author
             << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    virtual int calculateLateFee(int daysLate) {
        return 0;
    }
};

// ================= DERIVED CLASSES =================
class Book : public LibraryResource {
    string ISBN;
    int pageCount;

public:
    Book(int id, string t, string a, string isbn, int pages)
        : LibraryResource(id, t, a) {
        ISBN = isbn;
        pageCount = pages;
    }

    int calculateLateFee(int daysLate) {
        return daysLate * 5;
    }
};

class Magazine : public LibraryResource {
    int issueNumber;

public:
    Magazine(int id, string t, string a, int issue)
        : LibraryResource(id, t, a) {
        issueNumber = issue;
    }

    int calculateLateFee(int daysLate) {
        return daysLate * 3;
    }
};

class DVD : public LibraryResource {
    int duration;

public:
    DVD(int id, string t, string a, int d)
        : LibraryResource(id, t, a) {
        duration = d;
    }

    int calculateLateFee(int daysLate) {
        return daysLate * 10;
    }
};

// ================= MEMBER CLASS (COMPOSITION) =================
class LibraryMember {
    int memberID;
    string name;

    vector<LibraryResource> borrowedItems; // vector (NOT array, NOT pointers)

public:
    LibraryMember(int id = 0, string n = "") {
        memberID = id;
        name = n;
    }

    int getID() const { return memberID; }

    void borrowResource(LibraryResource res) {
        if (res.getAvailability()) {
            res.setAvailability(false);
            borrowedItems.push_back(res);
            cout << "Resource borrowed successfully.\n";
        } else {
            cout << "Resource already borrowed.\n";
        }
    }

    void returnResource(int resourceID) {
        for (int i = 0; i < borrowedItems.size(); i++) {
            if (borrowedItems[i].getID() == resourceID) {
                borrowedItems[i].setAvailability(true);
                borrowedItems.erase(borrowedItems.begin() + i);
                cout << "Returned successfully.\n";
                return;
            }
        }
        cout << "Resource not found.\n";
    }

    void displayBorrowedItems() {
        for (auto &item : borrowedItems) {
            item.displayDetails();
        }
    }

    int calculateTotalLateFee(int daysLate) {
        int total = 0;
        for (auto &item : borrowedItems) {
            total += item.calculateLateFee(daysLate);
        }
        return total;
    }

    // FRIEND FUNCTION
    friend void adminView(class Library &lib, vector<LibraryMember> &members);
};

// ================= LIBRARY CLASS =================
class Library {
public:
    vector<LibraryResource> resources;

    void addResource(LibraryResource res) {
        resources.push_back(res);
    }

    void displayResources() {
        for (auto &res : resources) {
            res.displayDetails();
        }
    }

    friend void adminView(Library &lib, vector<LibraryMember> &members);
};

// ================= FRIEND FUNCTION =================
void adminView(Library &lib, vector<LibraryMember> &members) {
    cout << "\n--- All Resources ---\n";
    for (auto &res : lib.resources) {
        res.displayDetails();
    }

    cout << "\n--- All Members ---\n";
    for (auto &mem : members) {
        cout << "Member ID: " << mem.memberID << endl;
        mem.displayBorrowedItems();
    }
}

// ================= MAIN (MENU) =================
int main() {
    Library lib;
    vector<LibraryMember> members;

    int choice;

    do {
        cout << "\n1. Add Resource\n2. Register Member\n3. Borrow\n4. Return\n5. Display Resources\n6. Display Member Items\n7. Late Fee\n8. Admin View\n9. Exit\n";
        cin >> choice;

        if (choice == 1) {
            int id;
            string title, author;
            cout << "Enter ID, Title, Author: ";
            cin >> id >> title >> author;

            lib.addResource(LibraryResource(id, title, author));
        }

        else if (choice == 2) {
            int id;
            string name;
            cout << "Enter Member ID and Name: ";
            cin >> id >> name;

            members.push_back(LibraryMember(id, name));
        }

        else if (choice == 5) {
            lib.displayResources();
        }

        else if (choice == 8) {
            adminView(lib, members);
        }

    } while (choice != 9);

    return 0;
}
// Travel Booking System
// Use std::map to manage travel bookings by date and std::priority_queue to handle urgent bookings."
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// Booking structure
struct Booking {
    string name;
    string date; 
    string destination;
    int urgencyLevel;

    Booking(string n, string d, string dest, int level)
        : name(n), date(d), destination(dest), urgencyLevel(level) {}
};

// Comparator for priority_queue (max-heap by urgency)
struct BookingComparator {
    bool operator()(Booking* a, Booking* b) {
        return a->urgencyLevel < b->urgencyLevel;
    }
};

// Global data structures
map<string, vector<Booking*>> bookingsByDate;
priority_queue<Booking*, vector<Booking*>, BookingComparator> urgentBookings;

// Add Booking
void addBooking(bool isUrgent) {
    string name, date, destination;
    int urgency;
    cout << "Enter name: ";
    cin >> name;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Enter destination: ";
    cin >> destination;
    cout << "Enter urgency level (0=normal, 1=more urgent): ";
    cin >> urgency;

    Booking* newBooking = new Booking(name, date, destination, urgency);

    if (isUrgent)
        urgentBookings.push(newBooking);
    else
        bookingsByDate[date].push_back(newBooking);

    cout << "Booking added successfully!\n";
}
// View bookings
void viewBookings() {
    // Display regular bookings by date
    if (bookingsByDate.empty()) {
        cout << "No regular bookings found.\n";
    } else {
        cout << "\n--- Regular Bookings ---\n";
        for (auto& entry : bookingsByDate) {
            cout << "Date: " << entry.first << endl;
            vector<Booking*>& v = entry.second;

            // Sort by name
            sort(v.begin(), v.end(), [](Booking* a, Booking* b) {
                return a->name < b->name;
            });

            for (auto& b : v) {
                cout << "  Name: " << b->name << ", Destination: " << b->destination
                     << ", Urgency: " << b->urgencyLevel << endl;
            }
        }
    }

    // Display urgent bookings
    if (urgentBookings.empty()) {
        cout << "\nNo urgent bookings found.\n";
    } else {
        cout << "\n--- Urgent Bookings ---\n";
        priority_queue<Booking*, vector<Booking*>, BookingComparator> tempQueue = urgentBookings;

        while (!tempQueue.empty()) {
            Booking* b = tempQueue.top();
            tempQueue.pop();
            cout << "  Name: " << b->name << ", Destination: " << b->destination
                 << ", Urgency: " << b->urgencyLevel << endl;
        }
    }
}

// Process Urgent Bookings
void processUrgentBookings() {
    if (urgentBookings.empty()) {
        cout << "No urgent bookings.\n";
        return;
    }

    while (!urgentBookings.empty()) {
        Booking* b = urgentBookings.top();
        urgentBookings.pop();
        cout << "Processing URGENT: " << b->name << " to " << b->destination << " (Urgency: " << b->urgencyLevel << ")\n";
        delete b; // clean up
    }
}

// Cancel Booking by Name and Date
void cancelBooking() {
    string name, date;
    cout << "Enter booking name to cancel: ";
    cin >> name;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> date;

    auto& vec = bookingsByDate[date];
    bool found = false;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if ((*it)->name == name) {
            delete *it;
            vec.erase(it);
            found = true;
            cout << "Booking cancelled.\n";
            break;
        }
    }
    if (!found) {
        cout << "Booking not found on that date.\n";
    }
    if (vec.empty()) {
        bookingsByDate.erase(date); // clean map
    }
}

// Cleanup memory
void cleanup() {
    for (auto& entry : bookingsByDate) {
        for (auto& b : entry.second)
            delete b;
    }
    while (!urgentBookings.empty()) {
        delete urgentBookings.top();
        urgentBookings.pop();
    }
}

int main() {
    int choice;
    do {
        cout << "\n--- Travel Booking System ---\n";
        cout << "1. Add Booking (Normal)\n";
        cout << "2. Add Booking (Urgent)\n";
        cout << "3. View Bookings by Date\n";
        cout << "4. Process Urgent Bookings\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: 
                addBooking(false); 
                break;
            case 2:
                addBooking(true); 
                break;
            case 3: 
                viewBookings(); 
                break;
            case 4: 
                processUrgentBookings(); 
                break;
            case 5: 
                cancelBooking(); 
                break;
            case 6: 
                cleanup(); 
                cout << "Goodbye! Safe Travel\n"; 
                break;
            default: 
                cout << "Invalid choice.\n";
        }
    } while (choice != 6);
   return 0;
}
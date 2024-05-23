#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

struct Resident {
    std::string surname;
    std::string initials;
    int roomNumber;
};

void loadResidentsFromFile(const std::string& filename, std::vector<Resident>& residents);
void saveResidentsToFile(const std::string& filename, const std::vector<Resident>& residents);
void addResident(std::vector<Resident>& residents);
void searchResident(const std::vector<Resident>& residents);
void displayMenu();
std::string getInitials();
bool roomAvailable(int roomNumber, const std::vector<Resident>& residents);

int main() {
    std::vector<Resident> residents;
    std::string filename;

    std::cout << "Enter filename: ";
    std::cin >> filename;

    loadResidentsFromFile(filename, residents);

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            addResident(residents);
            saveResidentsToFile(filename, residents);
            break;
        case 2:
            searchResident(residents);
            break;
        case 3:
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 3);

    return 0;
}

void loadResidentsFromFile(const std::string& filename, std::vector<Resident>& residents) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Resident res;
        iss >> res.surname >> res.initials >> res.roomNumber;
        residents.push_back(res);
    }

    file.close();
}

void saveResidentsToFile(const std::string& filename, const std::vector<Resident>& residents) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    for (const auto& res : residents) {
        file << res.surname << " " << res.initials << " " << res.roomNumber << std::endl;
    }

    file.close();
}

void addResident(std::vector<Resident>& residents) {
    Resident res;

    std::cout << "Enter surname: ";
    std::cin >> res.surname;
    res.initials = getInitials();

    do {
        std::cout << "Enter room number (1-15): ";
        std::cin >> res.roomNumber;
    } while (!roomAvailable(res.roomNumber, residents));

    residents.push_back(res);
}

void searchResident(const std::vector<Resident>& residents) {
    std::string surname;
    std::cout << "Enter surname to search: ";
    std::cin >> surname;

    std::vector<Resident> results;
    for (const auto& res : residents) {
        if (res.surname == surname) {
            results.push_back(res);
        }
    }

    if (results.empty()) {
        std::cout << "No residents with the surname " << surname << " found." << std::endl;
    }
    else if (results.size() == 1) {
        std::cout << "Resident: " << results[0].surname << " " << results[0].initials << " in room " << results[0].roomNumber << std::endl;
    }
    else {
        std::cout << "Multiple residents with the surname " << surname << " found. Please provide initials." << std::endl;
        std::string initials = getInitials();
        bool found = false;
        for (const auto& res : results) {
            if (res.initials == initials) {
                std::cout << "Resident: " << res.surname << " " << res.initials << " in room " << res.roomNumber << std::endl;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "No residents with the initials " << initials << " found." << std::endl;
        }
    }
}

void displayMenu() {
    std::cout << "Menu:\n";
    std::cout << "1. Add resident\n";
    std::cout << "2. Search resident\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

std::string getInitials() {
    std::string initials;
    std::cout << "Enter initials: ";
    std::cin >> initials;
    return initials;
}

bool roomAvailable(int roomNumber, const std::vector<Resident>& residents) {
    if (roomNumber < 1 || roomNumber > 15) {
        std::cout << "Invalid room number. Please enter a number between 1 and 15." << std::endl;
        return false;
    }
    int count = 0;
    for (const auto& res : residents) {
        if (res.roomNumber == roomNumber) {
            count++;
        }
    }
    if ((roomNumber <= 5 && count >= 1) || (roomNumber > 5 && count >= 2)) {
        std::cout << "Room is already fully occupied." << std::endl;
        return false;
    }
    return true;
}

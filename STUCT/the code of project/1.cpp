#include <iostream>
#include <fstream>
#include <string>
#include<algorithm>

using namespace std;
#define MAX 100

struct contact {
	string name;
	string mobile;
	string email;
	string category;
};
struct user {
	string username;
	string pass;
	contact contacts[MAX];
	int contactCounter = 0;
};

user users[MAX]; 
int userCount = 0;

void loadData() {
	ifstream userFile("users.txt");
	if (userFile.is_open()) {
		while (userFile >> users[userCount].username >> users[userCount].pass) {
			userCount++;
		}
		userFile.close();
	}

	for (int i = 0; i < userCount; i++) {
		string filename = "contacts_" + users[i].username + ".txt";
		ifstream contactFile(filename);
		if (contactFile.is_open()) {
			string line;
			while (getline(contactFile, users[i].contacts[users[i].contactCounter].name)) {
				if (!getline(contactFile, users[i].contacts[users[i].contactCounter].mobile)) break;
				if (!getline(contactFile, users[i].contacts[users[i].contactCounter].email)) break;
				if (!getline(contactFile, users[i].contacts[users[i].contactCounter].category)) break;
				users[i].contactCounter++;
			}
			contactFile.close();
		}
	}
}

void saveData() {
	ofstream userFile("users.txt");
	if (userFile.is_open()) {
		for (int i = 0; i < userCount; i++) {
			userFile << users[i].username << " " << users[i].pass << endl;
		}
		userFile.close();
	}

	for (int i = 0; i < userCount; i++) {
		string filename = "contacts_" + users[i].username + ".txt";
		ofstream contactFile(filename);
		if (contactFile.is_open()) {
			for (int j = 0; j < users[i].contactCounter; j++) {
				contactFile << users[i].contacts[j].name << endl;
				contactFile << users[i].contacts[j].mobile << endl;
				contactFile << users[i].contacts[j].email << endl;
				contactFile << users[i].contacts[j].category << endl;
			}
			contactFile.close();
		}
	}
}

void searchContact(int currentUser) {
	if (users[currentUser].contactCounter == 0) // to make sure if the user has contacts
	{
		cout << "No contacts found.\n";
		return;
	}

	cout << "\nEnter what you want to search for: \n";
	cout << "Search by: \n1-Name\n 2-Phone\n 3-Email.\n";
	int choice;
	cin >> choice;
	cin.ignore();
	if (choice == 1) {
		cout << "Searching by Name...\n" << endl;
	}
	else if (choice == 2) {
		cout << "Searching by Phone...\n" << endl;
	}
	else if (choice == 3) {
		cout << "Searching by Email...\n" << endl;
	}
	string searchKey;
	getline(cin, searchKey);
	bool found = false;
	for (int i = 0; i < users[currentUser].contactCounter; i++) {
		if ((choice == 1 && users[currentUser].contacts[i].name == searchKey) ||
			(choice == 2 && users[currentUser].contacts[i].mobile == searchKey) ||
			(choice == 3 && users[currentUser].contacts[i].email == searchKey)) {

			cout << "\nContact Found!\n" << endl;
			cout << "Name: " << users[currentUser].contacts[i].name << endl;
			cout << "Phone: " << users[currentUser].contacts[i].mobile << endl;
			cout << "Email: " << users[currentUser].contacts[i].email << endl;
			cout << "Category:" << users[currentUser].contacts[i].category << endl;
			found = true;
			break;
		}
	}

	if (!found) {
		cout << "Contact not found.\n" << endl;
	}
}

int User_Authentication(string username, string password) {

	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username && users[i].pass == password) {
			return i;
		}
	}
	return -1;
}

void editContact(int currentUser) {
	if (users[currentUser].contactCounter == 0) {
		cout << "No contacts found.\n";
		return;
	}
	string searchName;
	cout << "Enter the name of the contact to edit: \n";
	cin.ignore();
	getline(cin, searchName);

	bool found = false;

	for (int i = 0; i < users[currentUser].contactCounter; i++) {
		if (users[currentUser].contacts[i].name == searchName) {
			found = true;

			cout << "\nCurrent Info:\n";
			cout << "1. Name : " << users[currentUser].contacts[i].name << endl;
			cout << "2. Phone: " << users[currentUser].contacts[i].mobile << endl;
			cout << "3. Email: " << users[currentUser].contacts[i].email << endl;

			int choice;
			cout << "What do you want to edit? (1-Name, 2-Phone, 3-Email): \n";
			cin >> choice;
			cin.ignore();

			switch (choice) {
			case 1:
				cout << "Enter new name: \n";
				getline(cin, users[currentUser].contacts[i].name);
				break;
			case 2:
				cout << "Enter new phone: \n";
				getline(cin, users[currentUser].contacts[i].mobile);
				break;
			case 3:
				cout << "Enter new email: \n";
				getline(cin, users[currentUser].contacts[i].email);
				break;
			default:
				cout << "Invalid choice!\n";
				return;
			}

			cout << "\nContact updated successfully!\n";
			break;
		}
	}

	if (!found) {
		cout << "\nContact not found!\n";
	}
}

void Deletefn(int currentUser)
{

	if (users[currentUser].contactCounter == 0) // to make sure if the user has contacts
	{
		cout << "No contacts found.\n";
		return;
	}

	string contnumber;    
	cout << "Enter the number of contact you want to delete : \n";
	cin.ignore();
	getline(cin, contnumber);
	if (stoi(contnumber) <= 0)
	{
		cout << "Invalid number, please try again.\n";
		return;
	}
	bool found = false;
	for (int i = 0; i < users[currentUser].contactCounter; i++)
	{       // search to find the number and delete it
		if (contnumber == users[currentUser].contacts[i].mobile)
		{
			for (int j = i; j < users[currentUser].contactCounter - 1; j++)
			{
				users[currentUser].contacts[j] = users[currentUser].contacts[j + 1];
			}
			found = true;
			users[currentUser].contactCounter--;
			cout << "Contact deleted successfully.\n";
			break;
		}
	}
	if (!found)
	{
		cout << "Contact not found.\n";
	}

}

void registerUser() {

	string username, password;
	cin.ignore();
	cout << " Enter a new username :  ";
	getline(cin, username);

	// chec if username already exists 
	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username) {
			cout << "Username already exists. Try a different one.\n";
			return;
		}
	}

	// Validate password 

	cout << " Enter a password ( at least 4 characters )\n";
	cin.ignore();
	getline(cin, password);

	while (password.length() < 4) {
		cout << " Password is too short . Try again : \n";
		cin >> password;
	}

	users[userCount].username = username;
	users[userCount].pass = password;
	userCount++;
	cout << "User registered successfully!\n";
}

int loginUser() {
	string username, password;
	cout << " Enter your username : \n";
	cin.ignore();
	getline(cin, username);
	cout << " Enter your password : \n";
	cin.ignore();
	getline(cin, password);


	int userIndex = User_Authentication(username, password);
	if (userIndex != -1) {
		cout << "Login successful!\n";
		return userIndex;
	}
	else {
		cout << "Login failed! Incorrect username or password.\n";
		return -1;
	}

}

bool isvalidphone(const string& phone)  //function to check validity of phone number;
{
	for (char c : phone)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return !phone.empty();
}

bool isvalidemail(const string& email)  //function to check validity of email ;
{
	size_t at_pos = email.find('@');   
	size_t dot_pos = email.rfind('.');  

	return (at_pos != string::npos) &&
		(dot_pos != string::npos) &&
		(at_pos > 0) &&
		(dot_pos > at_pos + 1) &&
		(dot_pos < email.length() - 1);
}

void categorizeContact(int currentUser) {
	if (users[currentUser].contactCounter == 0) {
		cout << "No contacts to categorize.\n";
		return;
	}
	string number;
	cout << "Enter contact number to categorize :\n";
	cin.ignore();
	cin >> number;
	bool found = false;
	for (int i = 0;i <= users[currentUser].contactCounter;i++) {
		if (number == users[currentUser].contacts[i].mobile) {
			cout << "Enter category for this contact (family/friends/work): ";
			cin >> users[currentUser].contacts[i].category;
			cout << "Category added successfully!\n";
			found = true;
		}
		break;
	}
	if (!found) {
		cout << "Invalid contact number.\n";
	}
}

void filterByCategory(int currentUser) {
	if (users[currentUser].contactCounter == 0) {
		cout << "No contacts found.\n";
		return;
	}


	string cat;
	cout << "Enter category to filter: ";
	cin.ignore();
	getline(cin, cat);

	bool found = false;

	cout << "\nContacts in category: " << cat << endl;

	for (int i = 0; i < users[currentUser].contactCounter; i++) {
		if (users[currentUser].contacts[i].category == cat) {
			cout << "--------------------\n";
			cout << "Name: " << users[currentUser].contacts[i].name << endl;
			cout << "Phone: " << users[currentUser].contacts[i].mobile << endl;
			cout << "Email: " << users[currentUser].contacts[i].email << endl;
			found = true;
		}
	}

	if (!found) {
		cout << "No contacts found in this category.\n";
	}
}

void addcontact(int currentUser)
{
	if (users[currentUser].contactCounter >= MAX) {
		cout << "Contact limit reached!\n";
		return;
	}

	cin.ignore();
	cout << "Enter the Name: ";
	getline(cin, users[currentUser].contacts[users[currentUser].contactCounter].name);

	do
	{
		cout << "Enter the Phone Number : ";
		getline(cin, users[currentUser].contacts[users[currentUser].contactCounter].mobile);
		if (!isvalidphone(users[currentUser].contacts[users[currentUser].contactCounter].mobile))
		{
			cout << "Not Valid Number,Insert it again!\n";
		}
	} while (!isvalidphone(users[currentUser].contacts[users[currentUser].contactCounter].mobile));

	do
	{
		cout << "Enter the E-mail : ";
		getline(cin, users[currentUser].contacts[users[currentUser].contactCounter].email);
		if (!isvalidemail(users[currentUser].contacts[users[currentUser].contactCounter].email))
		{
			cout << "Invalid E-mail,Insert it again!\n";

		}
	} while (!isvalidemail(users[currentUser].contacts[users[currentUser].contactCounter].email));

	cout << "Enter the category (family/friends/work) : ";
	getline(cin, users[currentUser].contacts[users[currentUser].contactCounter].category);

	users[currentUser].contactCounter++;
	cout << "Contact added successfully!\n";
}


bool compareContactsByName(const contact& a, const contact& b)
{
	return a.name < b.name;
}

void sortAlphabetically(user& users)
{
	sort(users.contacts, users.contacts + users.contactCounter, compareContactsByName);
}

void displaycontacts(int currentuser)

{
	if (currentuser == -1 || users[currentuser].contactCounter == 0)
	{
		cout << "No contacts to display.\n";
		return;
	}

	sortAlphabetically(users[currentuser]);

	for (int i = 0;i < users[currentuser].contactCounter;i++)
	{

		cout << "Contact Number : " << i + 1 << endl;
		cout << users[currentuser].contacts[i].name << endl;
		cout << users[currentuser].contacts[i].mobile << endl;
		cout << users[currentuser].contacts[i].email << endl;
		cout << users[currentuser].contacts[i].category << endl;
	}
}

int main() {
	loadData();
	int choice;
	int currentUser = -1;

	do {
		cout << "\n----------Contact Management System----------\n";
		if (currentUser == -1) {
			// Pre-login menu
			cout << "1: Register\n";
			cout << "2: Login\n";
			cout << "3: Exit\n";
			cout << "Enter your choice: ";
			cin >> choice;

			switch (choice) {
			case 1:
				registerUser();
				break;
			case 2:
				currentUser = loginUser();
				if (currentUser != -1) {
					cout << "Welcome " << users[currentUser].username << "!\n";
				}
				break;
			case 3:
				saveData();
				cout << "Goodbye.\n";
				return 0;
			default:
				cout << "Invalid choice, try again.\n";
			}
		}
		else {
			// Post-login menu
			cout << "1: Delete Contact\n";
			cout << "2: Add Contact\n";
			cout << "3: Categorize Contact\n";
			cout << "4: Filter by Category\n";
			cout << "5: Edit Contact\n";
			cout << "6: Search Contact\n";
			cout << "7: Displaying the Contacts\n";
			cout << "8: Logout and Exit\n";
			cout << "Enter your choice: ";
			cin >> choice;

			switch (choice) {
			case 1:
				Deletefn(currentUser);
				break;
			case 2:
				addcontact(currentUser); 
				break;
			case 3:
				categorizeContact(currentUser);
				break;
			case 4:
				filterByCategory(currentUser);
				break;
			case 5:
				editContact(currentUser);
				break;
			case 6:
				searchContact(currentUser);
				break;
			case 7:
				cout << "\n\nDisplaying Contacts.....\n\n";
				displaycontacts(currentUser);
				break;
			case 8:
				saveData();
				cout << "Goodbye.\n";
				return 0;
			default:
				cout << "Invalid choice, try again.\n";
			}
		}
	} while (true); 
}

#include <iostream>
#include <fstream>
#include <string>
#include<algorithm>
#include<openssl/sha.h>
#include <conio.h>
#include<cctype>

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

user users[MAX]; //array to store all users 
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

string hiddenpassword() {
	string password = "";
	char ch;

	while (true) {
		ch = _getch();// Read letter and return its ASCII Code 
		if (ch == 13) { // ASCII Code of Enter is 13
			cout << endl;
			break;
		}
		else if (ch == 8) { // ASCII Code of Backspace is 8
			if (!password.empty()) {
				password.pop_back();//Delet the end letter 
				cout << "\b \b";//Delet * 
			}
		}
		else
		{
			password += ch;
			cout << "*";
		}
	}
	return password;
}

// One-Way function 
string hashpassword(const string& password) {
	//Data Type stores one byte from numbers from 0 to 255, not negative    
	unsigned char hash[SHA256_DIGEST_LENGTH];   

	// ( (pointer to numbers not letters) transform string to const char* , length, storage)
	SHA256((unsigned char*)password.c_str(), password.length(), hash);
	string hashed;
	for (int i = 0;i < SHA256_DIGEST_LENGTH;i++) {
		char buffer[3] ; // transform each byte to Hex (2 letters + null '%')

		//transform each byte (hash[i]) to text of Hex and storage in buffer
		sprintf_s(buffer, "%02x", hash[i]);  
		hashed += buffer;
	}
	return hashed;
}

// Clean first and end of text from spaces, tabs, newlines or return characters
string trim(const string& str) {
	// Data type for position 
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	if (first == string::npos) // doesn't exist for any characters, exists spaces, tabs, newlines ....
		return "";// return empty value 
	return str.substr(first, last - first + 1); //sub -> cut part from string (index, length) 
}

string sanitize(const string& input) {
	string result = input;
	for (char& c : result) {
		if (c == ';' || c == '>' || c == '<' || c == '"' || c == ' \ '
			|| c == ' \\ ' || c == '~' || c == '*' || c == '%' || int(c) == 39) //ASCII code of ' by function int(variable)
			c = '_';
	}
	return trim(result);
}

string sanitizepassword(const string& input) {
	string result = input;
	for (char& c : result) {
		// Remove spaces, tabs, newlines or return characters  
		if (c == '\n' || c == '\t' || c == '\r')
			c = '_';
	}
	return trim(result);
}

bool isvalidusername(const string& username) {
	if (username.empty())
		return false;
	for (char c : username) { // for loop to every element in username and stores in c
		if (!isalpha(c) && c != '_')// check c == ( a->z , A->Z or _ )
			return false;
	}
	return true;
}

bool isvalidpassword(const string& password) {
	if (password.length() < 8)
		return false;
	bool    upper = false;
	bool	lower = false;
	bool	digit = false;
	for (char c : password) { // for loop to every element in password and stores in c
		if (isupper(c)) upper = true;
		if (islower(c)) lower = true;
		if (isdigit(c)) digit = true;
	}
	return upper && lower && digit; // must be all true to return true 
}

int User_Authentication(string username, string password) {

	string hashedpassword = hashpassword(password);

	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username && users[i].pass == hashedpassword) {
			return i;
		}
	}
	return -1;
}

void registerUser() {

	string username, password;
	cout << " Enter a new username :  ";
	cin.ignore();
	getline(cin, username);
	username = trim(username);//clean the starting and ending of input 
	username = sanitize(username);

	if (username.length() > 20) {
		cout << "username is too long !! Max 20 characters.\n";
		return;
	}

	if (!isvalidusername(username)) {
		cout << "Invalid username!! use only letters or underscore...\n";
		cout << "No spaces allowed\n";
		return;
	}

	bool usernameexist = false;

	// checK if username already exists 
	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username) {
			usernameexist = true;
			cout << "Username already exists! Try a different one.\n";
			return;
		}
	}

	// Validate password 

	cout << " Enter a password ( at least 8 characters )\n";
	password = hiddenpassword();
	password = trim(password);
	password = sanitizepassword(password);

	while (!isvalidpassword(password)) {
		cout << " Password must contain at least 8 characters, one uppercase letter, one lowercase letter and one digit.. Try again : \n";
		cin.ignore();
		getline(cin, password);
		password = trim(password);
		password = sanitizepassword(password);

	}

	users[userCount].username = username;
	users[userCount].pass = hashpassword(password); // Hashing for password 
	userCount++;
	cout << "User registered successfully!\n";
}

int loginUser() {
	string username, password;
	int attempts = 0;

	while (attempts < 3) {
		cout << " Enter your username : ";
		cin.ignore();
		getline(cin, username);
		username = trim(username);
		username = sanitize(username);

		if (!isvalidusername(username)) {
			cout << " Invalid username format!!\n";
			attempts++;
			cout << "remaining attempts: " << (3 - attempts) << endl;
			continue;
		}

		cout << " Enter your password : ";
		password = hiddenpassword();
		password = trim(password);
		password = sanitizepassword(password);

		int userIndex = User_Authentication(username, password);
		if (userIndex != -1) {
			cout << "Login successful!\n";
			return userIndex;
		}
		else {
			attempts++;
			cout << "Login failed! Incorrect username or password.\n";
			cout << "remaining attempts: " << (3 - attempts) << endl;
		}

	}

	cout << "Too many failed attempts !! try again later...\n";
	return -1;
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
	size_t at_pos = email.find('@');   //evalute the position of '@' in the email ;
	size_t dot_pos = email.rfind('.');  //evaluate the position of '.' in the email;

	return (at_pos != string::npos) &&
		(dot_pos != string::npos) &&
		(at_pos > 0) &&
		(dot_pos > at_pos + 1) &&
		(dot_pos < email.length() - 1);
}

void addcontact(int currentUser)
{
	if (users[currentUser].contactCounter >= MAX) {
		cout << "Contact limit reached!\n";
		return;
	}
	

	cin.ignore();
	
	do
	{
		cout << "Enter the Name : ";
		getline(cin, users[currentUser].contacts[users[currentUser].contactCounter].name);
		if (!isvalidusername(users[currentUser].contacts[users[currentUser].contactCounter].name))
		{
			cout << "Not valid name,please try again \n";
		}
	} while (!isvalidusername(users[currentUser].contacts[users[currentUser].contactCounter].name));
	
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
	while (users[currentUser].contacts[users[currentUser].contactCounter].category != "family" &&
		users[currentUser].contacts[users[currentUser].contactCounter].category != "friends" &&
		users[currentUser].contacts[users[currentUser].contactCounter].category != "work")
	{
		cout << "invalid category.please enter one of:(family/friends/work) : ";
		cin >> users[currentUser].contacts[users[currentUser].contactCounter].category;
	}

	users[currentUser].contactCounter++;
	cout << "Contact added successfully!\n";
}

void searchContact(int currentUser) {
	if (users[currentUser].contactCounter == 0)
	{
		cout << "No contacts found.\n";
		return;
	}

	cout << "\nEnter what you want to search for: \n";
	cout << "Search by:\n1-Name\n2-Phone\n3-Email.\n";
	int choice;
	while (true) {

		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. " << endl;
			cout << "Enter your choice :";
		}
		else
			break;
	}

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
	searchKey = trim(searchKey);
	searchKey = sanitize(searchKey);
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

void editContact(int currentUser) {
	if (users[currentUser].contactCounter == 0) {
		cout << "No contacts found.\n";
		return;
	}
	string searchName;
	cout << "Enter the name of the contact to edit: \n";
	cin.ignore();
	getline(cin, searchName);
	searchName = trim(searchName);
	searchName = sanitize(searchName);

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
			while (true) {

				cin >> choice;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "Invalid input. " << endl;
					cout << "Enter your choice :";
				}
				else
					break;
			}

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

	string contNumber;
	int contactNumber;

	cout << "Enter the number of contact you want to delete : ";
	cin >> contNumber;
	contNumber = trim(contNumber);
	contNumber = sanitize(contNumber);


	// Check if the input is a valid number
	bool validInput = true;
	for (char c : contNumber) {
		if (!isdigit(c)) {
			validInput = false;
			break;
		}
	}
	if (!validInput || contNumber.empty()) {
		cout << "Invalid number, please try again.\n";
		return;
	}

	// Convert string to integer
	contactNumber = stoi(contNumber);

	// Check if the contact number is within range
	if (contactNumber <= 0 || contactNumber > users[currentUser].contactCounter) {
		cout << "Invalid number, please try again.\n";
		return;
	}
	bool found = false;
	for (int i = 0; i < users[currentUser].contactCounter; i++)
	{       // search to find the number and delete it
		if (contNumber == users[currentUser].contacts[i].mobile)
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

void categorizeContact(int currentUser) {
	if (users[currentUser].contactCounter == 0) {
		cout << "No contacts to categorize.\n";
		return;
	}
	string number;
	cout << "Enter contact number to categorize :\n";
	cin >> number;
	number = trim(number);
	number = sanitize(number);

	string category;
	bool found = false;
	for (int i = 0; i < users[currentUser].contactCounter; i++) {
		if (number == users[currentUser].contacts[i].mobile) {
			cout << "Enter category for this contact (family/friends/work): ";
			cin >> category;
			category = trim(category);
			category = sanitize(category);
			while (category != "family" && category != "friends" && category != "work") {
				cout << "invalid category.\nplease enter one of:family/friends/work.\n";
				cin >> category;
				category = trim(category);
				category = sanitize(category);
			}
			users[currentUser].contacts[i].category = category;
			cout << "Category added successfully!\n";
			found = true;
			break;
		}
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
	cout << "Enter category from (family, frinds, work) to filter: ";
	cin.ignore();
	getline(cin, cat);
	cat = trim(cat);
	cat = sanitize(cat);


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
			
			while (true) {

				cin >> choice;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "Invalid input. " << endl;
					cout << "Enter your choice :";
				}
				else
					break;
			}
			
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
					cout << "default Invalid choice, try again.\n";
				}

		}
		else {
			// Post-login menu
			cout << "1: Add Contact\n";
			cout << "2: Search Contact\n";
			cout << "3: Categorize Contact\n";
			cout << "4: Filter by Category\n";
			cout << "5: Edit Contact\n";
			cout << "6: Delete Contact\n";
			cout << "7: Displaying the Contacts\n";
			cout << "8: Logout and Exit\n";
			cout << "Enter your choice: ";
			while (true) {
				
				cin >> choice;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "Invalid input. " << endl;
					cout << "Enter your choice :";
				}
				else
					break;
			}
			
				switch (choice) {
				case 1:
					addcontact(currentUser);
					
					break;
				case 2:
					searchContact(currentUser);
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
					Deletefn(currentUser);
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

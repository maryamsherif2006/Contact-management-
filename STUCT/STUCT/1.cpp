#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define MAX 100
struct contact {
	string name;
	int mobile;
	string email;
};
struct user {
	string username;
	string pass;
	contact contacts[MAX];
};
user users[MAX]; //array to store all users 

int userCount = 0;

//???? ??????? 

void registerUser() {

	string username, password;
	cout << " Enter a new username :  ";
	cin >> username;

	// chec if username already exists 
	ifstream infile("users.txt");
	string u, p;
	bool exists = false;

	if (infile.is_open()) {
		while (infile >> u >> p) {
			if (u == username) {
				exists = true;
				break;
			}
		}
		infile.close();
	}
	if (exists) {
		cout << " Username already existe , Try a different one . \n ";
		return;
	}

	// Validate password 
	cout << " Enter a password ( at least 4 characters )";
	cin >> password;

	while (password.length() < 4) {
		cout << " Password is too short . Try again : ";
		cin >> password;
	}

	// Save to file 
	ofstream outfile("users.txt", ios::app);
	if (outfile.is_open()) {
		outfile << username << " " << password << endl;
		outfile.close();
		cout << " User registered successfully \n";

		// Add to users array 
		users[userCount].username = username;
		users[userCount].pass = password;
		userCount++;
	}
	else
	{
		cout << " Error opening file . \n ";
	}
}


int loginUser() {
	string username, password;
	cout << " Enter your username : ";
	cin >> username;
	cout << " Enter your password : ";
	cin >> password;


	ifstream infile("users.txt");
	string u, p;
	int index = -1;

	if (infile.is_open()) {
		int i = 0;
		while (infile >> u >> p)
		{
			if (u == username && p == password) {
				index = i; // array???? ??? ???????? ?? ?? 
				break;
			}
			i++;
		}
		infile.close();
	}
	if (index != -1)
	{
		cout << " Login successfully \n";
	}
	else {
		cout << " Invalid username or password . \n";
	}
	return index;//?????? ????? (int) ???? ???? ??? ???????? ?????? ? ???? ??????? ?? ????? ?????? ....
}

int main()
{
	int choice;
	int currentUser = -1;

	while (true) {
		cout << " \n ----------Contact Management System----------\n";
		cout << " 1. Register \n ";
		cout << "2. Login \n";
		cout << " 3. Exist \n";
		cout << " Enter your choice : ";
		cin >> choice;

		if (choice == 1) {
			registerUser();
		}
		else if (choice == 2) {
			currentUser = loginUser();
			if (currentUser != -1) {
				cout << " welcom . " << users[currentUser].username << " \n";
			}
		}
		else if (choice == 3) {
			cout << " Goodbye \n";
			break;
		}
		else {
			cout << " Invalid choice . Try again . \n";
		}
	}
	
}
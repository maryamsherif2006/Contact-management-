#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define MAX 100
struct contact {
	string name;
	string mobile;
	string email;
}contactt[MAX];
struct user {
	string username;
	string pass;
	contact contacts[MAX];
	int contactCounter = 0;
};

user users[MAX]; //array to store all users 

int userCount = 0;


void Deletefn(int currentUser) 
{
	if (currentUser == -1)
	{ //to make sure the user already has an account
		cout << "Please log in First!\n";
		return;
	}
	if (users[currentUser].contactCounter == 0) // to make sure if the user has contacts
	{
		cout << "No contacts found.\n";
		return;
	}

	int contnumber;     //store the number we want to delete
	cout << "Enter the number of contact you want to delete : \n";
	cin >> contnumber;
	if (cont.mobile <= 0) 
	{
		cout << "Invalid number, please try again.\n";
		return;
	}
	bool found = false; 
	for (int i = 0;i < users[currentUser].contactCounter;i++)
	{       // search to find the number and delete it
		if (contnumber == users[currentUser].contacts[i].mobile)
		{
			for (int j = i;j < users[currentUser].contactCounter - 1;j++) 
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
	ofstream outfile("contacts_"+ users[currentUser].username + ".txt");  //to store the new contacts in the user's contacts file
	if (outfile.is_open())
	{
		for (int i = 0;i < users[currentUser].contactCounter;i++)
		{
			outfile << users[currentUser].contacts[i].name << "  "
				<< users[currentUser].contacts[i].mobile << "  "
				<< users[currentUser].contacts[i].email << "  " << endl;

		}
		outfile.close();
	}
	else
		cout << "Error updating Contact file\n";

    
}


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
	cout << " Enter a password ( at least 4 characters )\n";
	cin >> password;

	while (password.length() < 4) {
		cout << " Password is too short . Try again : \n";
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
	cout << " Enter your username : \n";
	cin >> username;
	cout << " Enter your password : \n";
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

int contactcount=0;

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

void savecontactToFile() //function to save the new contact to the file
{
	ofstream outfile("contacts.txt");
	if (!outfile)
	{
		cout << "Fail to Open the File!";
		return;
	}
	for (int i = 0;i < 100;i++)  //loop for saving contacts every time i enter a new contact
	{
		outfile << contactt[contactcount].name << "||" <<
			contactt[contactcount].mobile << "||" <<
			contactt[contactcount].email << "\n";
	}
	outfile.close();
	cout << "Saved Successfully!";
}





void addcontact()   //function to add new contact;
{
	
	
	cout << "Enter the Name : ";
	getline(cin, contactt[contactcount].name);

	do
	{
		cout << "Enter the Phone Number : ";
		getline(cin, contactt[contactcount].mobile);
		if (!isvalidphone(contactt[contactcount].mobile))
		{
			cout << "Not Valid Number,Insert it again!\n";
		}
	} while (!isvalidphone(contactt[contactcount].mobile));

	do
	{
		cout << "Enter the E-mail : ";
		getline(cin, contactt[contactcount].email);
		if (!isvalidemail(contactt[contactcount].email))
		{
			cout << "Invalid E-mail,Insert it again!\n";

		}
	} while (!isvalidemail(contactt[contactcount].email));


	savecontactToFile();
	

	contactcount++;
}












int main()
{
	int choice;
	int currentUser = -1;

	while (true) {
		cout << " \n ----------Contact Management System----------\n";
		cout << "1. Register \n ";
		cout << "2. Login \n";
		cout << "3. Delete \n";
		cout << "4. Add \n";
		cout << "5. Exist \n";
		cout << "Enter your choice : \n";
		cin >> choice;

		switch (choice){
		case 1 :
			registerUser();
			break;
		
		case 2 : 
			currentUser = loginUser();
			if (currentUser != -1) {
				cout << " Welcome " << users[currentUser].username << " \n";
			}
			break;
		
		case 3 : 
			Deletefn(currentUser);
			break;
			
		case 4 :
		addcontact();
		break;


		
		case 5 :
         cout << " Goodbye \n";
			break;
		default:
			cout << " Invalid choice, Try again. \n";
		
	}

	





	return 0;
}
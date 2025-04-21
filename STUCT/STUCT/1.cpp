#include <iostream>
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

void add()
{
	contact cont;
	cout << "Enter the name : ";
	cin >> cont.name;
	cout << "Enter the Mobile Number : ";
	cin >> cont.mobile;
	cout << "Enter the E-mail : ";
	cin >> cont.email;
}

int main()
{
	int choise;
	cout << "Choose the Operation : \n";
	cout << "1-Add\n";
	cout << "2-Delete\n";
	cout << "3-Edit\n";
	cout << "4-Search\n";
	cin >> choise;

	switch (choise)
	{
	case 1:
		add();
		break;


	}
}
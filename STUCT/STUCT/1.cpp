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
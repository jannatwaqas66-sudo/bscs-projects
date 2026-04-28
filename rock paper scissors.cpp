#include<iostream>
using namespace std;
int main() {
	int user1, user2;
	int count1 = 0, count2 = 0;
	cout << "1.rock \n 2.paper \n 3.scissor" << endl;
	for (int i = 1; i <= 3; i++) {
		cout << "----------- round" << i << "----------" << endl;
		cout << "player 1"<< endl;
		cout << "enter your choice in the form of number shown in menu" << endl;
		cin >> user1;
		cout << "player 2" << endl;
		cout << "enter your choice in the form of number shown in menu";
		cin >> user2;
		if ((user1 == 1 && user2 == 2) || (user1 == 2 && user2 == 3) || (user1 == 3 && user2 == 1)) {
			cout << "player 2 gain point" << endl;
			count2++;
		}
		else if ((user1 == 2 && user2 == 1) || (user1 == 3 && user2 == 2) || (user1 == 1 && user2 == 3)) {
			cout << "player 1 gain point" << endl;
			count1++;
		}
		else
			cout << "this round draw" << endl;
	}
	cout << "--------------------------------" << endl;
	cout << "so,the final results are :";
	if (count1 > count2) {
		cout << "player 1 won";
	}
	else if (count2 > count1) {
		cout << "player 2 won";
	}
	else
		cout << "draw" << endl;
}
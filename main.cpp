#include <iostream>
#include "HW3_1/HW3_1.hpp"
#include "HW3_2/HW3_2.hpp"
#include "HW3_3/HW3_3.hpp"

int main()
{
	char cmd_input = 0;

	while (cmd_input != 'q')
	{
		cout << "1: 1_1 anwser\n"
			<< "2: 1_2 anwser\n"
			<< "3: 1_3 anwser\n"
			<< "4: 2_1 anwser\n"
			<< "5: 2_2 anwser\n"
			<< "6: 2_3 anwser\n"
			<< "7: 3_1 anwser\n"
			<< "8: 3_2 anwser\n"
			<< "9: 3_3 anwser\n"
			<< "q: quit\n"
			<< "Enter the question number to select output result: ";
		cin >> cmd_input;

		switch (cmd_input)
		{
		case '1':
			HW3_1_1();
			break;

		case '2':
			HW3_1_2();
			break;

		case '3':
			HW3_1_3();
			break;

		case '4':
			HW3_2_1();
			break;

		case '5':
			HW3_2_2();
			break;

		case '6':
			HW3_2_3();
			break;

		case '7':
			HW3_3_1();
			break;

		case '8':
			HW3_3_2();
			break;

		case '9':
			HW3_3_3();
			break;
		}

		cout << endl << endl;
	}
}

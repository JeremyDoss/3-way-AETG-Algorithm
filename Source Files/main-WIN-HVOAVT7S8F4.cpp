/*
Jeremy Doss
HW1 - AETG Algorithm
CSCE 4444 - Software Engineering
2/2/15
*/

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <list>
#include "node.h"

using namespace std;

//Global Variables.
list<int> plist[19000];		//A list of involved pair numbers.

list<node> tuples;			//Stores the list of tuples.

ofstream olog;				//Output file stream for logging.
int runs = 1;				//Number of test suites to pick suite smallest from.
int total_triples = 0;	//Holds total possible pair combinations.
int inputarray[40][10];		//Holds input.
int factors, levels;
double timetotal = 0;

bool keep = true;
int currentrun = 0;

//Function prototypes.
void build_input();
void findtriples();
void testcases();
bool findp(int index, int value);

//Main program loop.
int main() {
	string input, output;

	cout << "Options:\n1: 'run'\n2: 'exit'" << endl;

	//Main program loop.
	while (1) {
		cout << "Input:> ";
		cin >> input;

		if (input == "run" || input == "1") {
			//Clear plist.
			for (int j = 0; j < factors*factors * 2; j++)
				plist[j].clear();
			timetotal = 0;
			currentrun = 0;
			factors = 0;
			levels = 0;
			cout << "Enter the amount of factors: ";
			cin >> factors;
			cout << "Enter the amount of levels per factor: ";
			cin >> levels;
			cout << "Enter the name of the output file: ";
			cin >> output;
			olog.open(output.c_str());

			//Run to store input and globals.
			build_input();

			//Run the test suites 100 times.
			for (int i = 0; i < runs; i++) {
				for (int j = 0; j < factors*factors * 2; j++)
					plist[j].clear();
				//Run to store all pairs into plist.
				findtriples();
				//Initialize the AETG test suite algorithm.
				//testcases();
				currentrun++;
			}
			olog.close();
		}

		else if (input == "exit" || input == "2")
			break;

		else
			cout << "Error: Invalid command." << endl;
	}

	return 0;
}

//Main AETG function.
void build_input() {
	total_triples = (levels * levels * levels * factors * (factors - 1) * (factors - 2))/6;

	//Store the table of numbers into an input array.
	int count = 0;
	for (int i = 0; i < factors; i++) {
		for (int j = 0; j < levels; j++) {
			inputarray[i][j] = count;
			count++;
		}
	}

/*
	//Print out the input array.
	cout << "Input array:" << endl;
	for (int j = 0; j < levels; j++) {
		for (int i = 0; i < factors; i++) {
			cout << inputarray[i][j] << " ";
		}
		cout << endl << endl;
	}
*/
}

//This function finds every pairwise combination and stores them in an array of linked lists.
void findtriples() {
	tuples.clear(); 
	node tmp;

	//Find all of the triples.
	if (tuples.empty())
		//1st number (at i,j)
		for (int i = 0; i < (factors - 2); i++)
			for (int j = 0; j < levels; j++)
				//2nd number (at x,y)
				for (int x = (i+1); x < (factors - 1); x++)
					for (int y = 0; y < levels; y++)
						//3rd number (at a,b)
						for (int a = (x + 1); a < factors; a++)
							for (int b = 0; b < levels; b++) {
								tmp.x = inputarray[i][j];
								tmp.y = inputarray[x][y];
								tmp.z = inputarray[a][b];
								tuples.push_back(tmp);
							}

	//Print total pair count.
	cout << "Total triples: " << tuples.size() << endl;

	//Print all triples.
//	for (list<node>::iterator i = tuples.begin(); i != tuples.end(); i++) {
//		node n = *i;
//		n.print();
//		cout << ", ";
//	}
}

//Run test cases until all pairs are resolved.
void testcases() {
	int suitearray[400][40];
	clock_t start;
	float timediff = 0;
	int testcase[40];
	int tmpcase[40];
	int randlvl;
	int numpairs = 0;
	int bestpairs;
	int current = 0;
	int suiteSize = 0;
	srand(time(NULL));

	start = clock();

	//for (int i = 0; i < 20; i++)
		//testcase[i] = -1;

	while (current < total_triples) {
		bestpairs = 0;
		numpairs = 0;


		for (int candidate = 0; candidate < 1000; candidate++) {
			numpairs = 0;
			//srand(time(NULL));

			//Choose random level for each factor.
			for (int i = 0; i < factors; i++) {
				randlvl = inputarray[i][(rand()+i) % levels];
				//Add it to current test case.
				tmpcase[i] = randlvl;
			}

			//Test pairs.
			for (int i = 0; i < (factors*levels); i++) {
				for (int x = 0; x < factors; x++) {
					if (i == tmpcase[x]) {
						for (int y = 0; y < factors; y++) {
							if (findp(i, tmpcase[y])) {
								numpairs++;
							}
						}
					}
				}
			}

			if (numpairs > bestpairs) {
				bestpairs = numpairs;
				//Store best case in temp.
				for (int i = 0; i < factors; i++) {
					testcase[i] = tmpcase[i];
				}
			}
		}//temp

		cout << "Covered pairs: " << current << endl;

		//Remove current test case pairs from list
		for (int i = 0; i < (factors*levels); i++) {
			for (int x = 0; x < factors; x++) {
				if (i == testcase[x]) {
					for (int y = 0; y < factors; y++) {
						plist[i].remove(testcase[y]);
					}
				}
			}
		}

		//for (int i = 0; i < factors; i++) {
			//if (testcase[i] == -1)
				//keep = false;
		//}
 
		//Copy to output.
		if (keep == true) {
			current += bestpairs;
			for (int i = 0; i < factors; i++) {
				suitearray[suiteSize][i] = testcase[i];
				//cout << suitearray[suiteSize][i] << " ";
			}
			//cout << endl;
			suiteSize++;
		}
		//for (int i = 0; i < 20; i++)
			//testcase[i] = -1;
		keep = true;
	}

	timediff = (float)(clock() - start) / CLOCKS_PER_SEC;
	timetotal += timediff;

	//Print test suite.
	if (1) { //if (currentrun == (runs-1)) {
		cout << suiteSize << endl << endl;
		olog << suiteSize << endl << endl;
		for (int x = 0; x < suiteSize; x++) {
			for (int y = 0; y < factors; y++) {
				cout << suitearray[x][y] << " ";
				olog << suitearray[x][y] << " ";
			}
			cout << endl;
			olog << endl;
		}
		cout << "Average runtime: " << (timetotal / runs) << "s" << endl;
		olog << "Average runtime: " << (timetotal / runs) << "s" << endl;
	}
}

//Find if value is paired with index.
bool findp(int index, int value) {
	if (index < 0)
		return false;
	for (list<int>::iterator i = plist[index].begin(); i != plist[index].end(); i++) {
		if (*i == value)
			return true;
	}
	return false;
}

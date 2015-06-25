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
#define MAX_SIZE 256

using namespace std;

//Global Variables.
int inputarray[40][10];		//Stores a formatted input array table.
bool bit_tuples[MAX_SIZE];			//Simulates a bit array where the index 'i' represents the presence of that value where 0<='i'<=(n-1)
list<node> tuples;			//Stores the list of tuples.
ofstream olog;				//Output file stream for logging.
int factors, levels;		//Stores the table information.
int runs = 100;				//Number of test suites (100) to generate.
int total_combinations;	//Holds total possible n-tuple combinations.
int total_time;			//Stores the amount of time taken to generate test suites.
int total_runs;			//Stores the current amount of completed runs.

//Function prototypes.
void build_input();
void findtriples();
void testcases();
bool is_match(node);
void remove_tuple(node);

//Main program loop.
int main() {
	string input, output;

	cout << "Options:\n1: 'run'\n2: 'exit'" << endl;

	//Main program loop.
	while (1) {
		cout << "Input:> ";
		cin >> input;

		if (input == "run" || input == "1") {
			factors = 0;
			cout << "Enter the amount of factors: ";
			cin >> factors;
			levels = 0;
			cout << "Enter the amount of levels per factor: ";
			cin >> levels;
			cout << "Enter the name of the output file: ";
			cin >> output;
			olog.open(output.c_str());
			//olog.open("Tests.txt");

			//Run to store input and globals.
			build_input();
			//findtriples();

			//Run the test suites 'runs' times.
			for (int i = 0; i < runs; i++) {
				//Reset tuple list.
				//reset_tuples();
				tuples.clear();
				findtriples();
				//Initialize the AETG test suite algorithm.
				testcases();
				//Increment run count.
				total_runs++;
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

//Reset global variables, calculate combinations count, reset the tuples structure.
void build_input() {
	total_combinations = (levels * levels * levels * factors * (factors - 1) * (factors - 2)) / 6;
	total_time = 0;
	total_runs = 0;
	int count = 0;

	//Clear the tuple list.
	tuples.clear();

	//Store the table of numbers into an input array.
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
/*
	//Print all triples.
	for (list<node>::iterator i = tuples.begin(); i != tuples.end(); i++) {
		i->print();
		cout << endl;
	}
*/
}

//Run test cases until all pairs are resolved.
void testcases() {
	//list<int> suitearray[20];
	int suitearray[2048][40];
	int testcase[20];
	int tmpcase[20];
	node temp;
	int best_count;
	int tuple_count;
	int suiteSize = 0;
	int covered_tuples = 0;
	srand(time(NULL));
	//Initialize the suite-relevant timer.
	clock_t start_time;
	float timediff = 0;
	start_time = clock();

	//This loop runs untill all n-tuples are satisfied
	while (covered_tuples < total_combinations) {
		best_count = 0;
		//srand(time(NULL));
		//This loop saves the best test case generated out of 50 candidates.
		for (int candidates = 0; candidates < 50; candidates++) {
			tuple_count = 0;
			//cout << ".";
			
			//Choose random level for each factor.
			for (int i = 0; i < factors; i++)
				tmpcase[i] = inputarray[i][(rand() + i) % levels];
		
			//Iterate through these loops to cover all pairs.
			for (int i = 0; i < (factors - 2); i++)
				for (int j = (i + 1); j < (factors - 1); j++)
					for (int k = (j + 1); k < (factors); k++) {
						temp.x = tmpcase[i];
						temp.y = tmpcase[j];
						temp.z = tmpcase[k];
						//Check case against the list of tuples.
						if (is_match(temp))
							tuple_count++;
					}

			//If this case is better than the current best, replace it.
			if (tuple_count > best_count) {
				best_count = tuple_count;
				//Store best case in temp.
				for (int i = 0; i < factors; i++) {
					testcase[i] = tmpcase[i];
					tmpcase[i] = -1;
				}
			}
		}//Test case generation loop.

		//cout << "\nTuples covered by current case: " << best_count << endl;

		//Remove covered_tuples test case tuples from list
		for (int i = 0; i < (factors - 2); i++)
			for (int j = (i + 1); j < (factors - 1); j++)
				for (int k = (j + 1); k < (factors); k++) {
					temp.x = testcase[i];
					temp.y = testcase[j];
					temp.z = testcase[k];
					remove_tuple(temp);
				}

		//Copy to output.
		if (true) {
			covered_tuples += best_count;
			for (int i = 0; i < factors; i++)
				suitearray[suiteSize][i] = testcase[i];
			suiteSize++;
		}
	}

	timediff = (float)(clock() - start_time) / CLOCKS_PER_SEC;
	total_time += timediff;

	//Print test suite.
	if (1) { //if (total_runs == (runs-1)) {
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
		cout << "Average runtime: " << (total_time / runs) << "s" << endl;
		//olog << "Average runtime: " << (total_time / runs) << "s" << endl;
	}
}

bool is_match(node n) {
	//Check passed tuple against entire list.
	for (list<node>::iterator i = tuples.begin(); i != tuples.end(); i++) {
		if (i->x == n.x && i->y == n.y && i->z == n.z)
			return true;
	}
	return false;
}

void remove_tuple(node n) {
	//Find tuple and remove it.
	for (list<node>::iterator i = tuples.begin(); i != tuples.end(); i++)
		if (i->x == n.x && i->y == n.y && i->z == n.z) {
			tuples.erase(i);
			break;
		}

}
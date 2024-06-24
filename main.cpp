#include<string>
#include<vector>
#include<iostream>

using namespace std;

void PrintStack();
void Shift();
bool Reduce();

//p188, p159
#define NO_SYMBOLS 9
#define NO_TERMINALS 6
#define NO_NONTERMINALS 3
#define NO_STATES 12
#define ACCEPT_STATE 99


vector<char> terminals = { 'a','+','*','(',')','$' };
vector<char> nonterminals = { 'E','T','F' };
string inputString;
vector<int> stateStack, actions;
vector<char> symbolStack;
int parsingTable[NO_STATES][NO_SYMBOLS] =
{
	//7번 새로생긴 규칙이 accept
	//a + * ( ) $ E T F
	{5,0,0,4,0,0,1,2,3},
	{0,6,0,0,0,ACCEPT_STATE,0,0,0},
	{0,-2,7,0,-2,-2,0,0,0},
	{0,-4,-4,0,-4,-4,0,0,0},
	{5,0,0,4,0,0,8,2,3},
	{0,-6,-6,0,-6,-6,0,0,0 },
	{5,0,0,4,0,0,0,9,3},
	{5,0,0,4,0,0,0,0,10},
	{0,6,0,0,11,0,0,0,0},
	{0,-1,7,0,-1,-1,0,0,0},
	{0,-3,-3,0,-3,-3,0,0,0},
	{0,-5,-5,0,-5,-5,0,0,0}
};




int main()
{

	//스트링 입력
	cout << "입력 : ";
	cin >> inputString;

	cout << "--------------------------------------------------------------------------" << endl;
	cout << "Stack\t\t\t\tInput\t\t\t\tAction" << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	//스택에 0넣고 시작
	stateStack.push_back(0);

	while (true)
	{
		actions.push_back(parsingTable[stateStack.back()][find(terminals.begin(),terminals.end(),inputString[0])-terminals.begin()]);
		PrintStack(); 
		if (actions.back() < 0 || actions.back() == ACCEPT_STATE)
		{
			if (Reduce())
				break;
		}
		else if (actions.back() > 0)
		{
			Shift();
		}
	}

	//ACCEPT_STATE 빼고 출력
	cout << "Right Parse: ";
	for (int i = 0; i < actions.size() - 1; ++i)
	{
		if (actions[i] < 0)
			cout << -actions[i] << " ";
	}

	return 0;
}

void PrintStack()
{
	int i = 0;
	for (i = 0; i < symbolStack.size(); ++i)
	{
		cout << stateStack[i] << symbolStack[i];
	}
	cout << stateStack[i];
	cout<< "\t\t\t\t" << inputString << "\t\t\t\t";
	if (actions.back() == ACCEPT_STATE)
	{
		cout << "Accept" << endl;
	}
	else if (actions.back() > 0)
	{
		cout << "Shift " << actions.back() << endl;
	}
	else if (actions.back() < 0)
	{
		cout << "Reduce " << -actions.back() << endl;
	}
	else
	{
		cout << "Reject" << endl;
		cout << "Wrong Sentence" << endl;
		exit(1);
	}
}	

void Shift()
{
	symbolStack.push_back(inputString[0]);
	stateStack.push_back(parsingTable[stateStack.back()][find(terminals.begin(), terminals.end(), inputString[0]) - terminals.begin()]);
	//actions.push_back(stateStack.back());
	inputString.erase(0, 1);
}

bool Reduce()
{
	//pop 사이즈
	int reduceSize = 0;
	//reduce 받아오기
	int action = parsingTable[stateStack.back()][find(terminals.begin(), terminals.end(), inputString[0]) - terminals.begin()];
	if (action == ACCEPT_STATE)
	{
		return true;
	}
	//actions.push_back(action);
	//pop 사이즈 정하기
	switch (actions.back())
	{
	case -1:
	case -3:
	case -5:
		reduceSize = 3;
		break;	
	case -2:
	case -4:
	case -6:
		reduceSize = 1;
		break;
	}
	//진짜 pop
	for (int i = 0; i < reduceSize; ++i)
	{
		symbolStack.pop_back();
		stateStack.pop_back();
	}
	//스택에 push할거
	switch (actions.back())
	{
	case -1:
	case -2:
		symbolStack.push_back('E');
		stateStack.push_back(parsingTable[stateStack.back()][6]);
		break;
	case -3:
	case -4:
		symbolStack.push_back('T');
		stateStack.push_back(parsingTable[stateStack.back()][7]);
		break;
	case -5:
	case -6:
		symbolStack.push_back('F');
		stateStack.push_back(parsingTable[stateStack.back()][8]);
		break;
	}
	return false;
}
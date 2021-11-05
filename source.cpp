
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

#define INF 0x7ffffff

struct Node
{
	int c[40]{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0
	};     // frequency table
	int delta;      // delta between total
	vector<int> s;  // indices of selected sentences

	Node()
	{
		//memset(c, 0, sizeof(c));
		delta = INF;
	}
};

void AddNode(Node* first, Node* second)
{
	for (int i = 0; i < sizeof first->c / 4; i++)
	{
		first->c[i] += second->c[i];
	}
}

int GetDelta(Node* total, Node* cur)
{
	int tot = 0;
	for (int i = 0; i < sizeof total->c / 4; i++)
	{
		tot += abs(total->c[i] - cur->c[i]);
	}

	return tot;
}

void Extract(string str, Node* node)
{
	for (int i = 0; i < str.length(); i++)
	{
		char ch = str[i];

		if (islower(ch))
		{
			node->c[ch - 'a']++;
		}
	}
}

void PrintCount(Node* node)
{
	cout << "Count Array" << endl;

	for (int i = 0; i < sizeof node->c / 4; i++)
	{
		if (node->c[i])
		{
			cout << "(" << (char)(i + 'a') << "," << node->c[i] << ") ";
		}
	}

	cout << endl;
}

void PrintTotal(Node* node)
{
	cout << "Target Array" << endl;

	for (int i = 0; i < sizeof node->c / 4; i++)
	{
		if (node->c[i])
		{
			cout << "(" << (char)(i + 'a') << "," << node->c[i] << ") ";
		}
	}

	cout << endl;
}

void PrintOptimizedNode(Node* node, vector<string>& sentences)
{
	cout << "Solution Array" << endl;

	for (int i = 0; i < sizeof node->c / 4; i++)
	{
		if (node->c[i])
		{
			cout << "(" << (char)(i + 'a') << "," << node->c[i] << ") ";
		}
	}

	cout << endl;
	cout << "Selected sentences: " << endl;

	for (int i = 0; i < node->s.size(); i++)
	{
		cout << sentences[node->s[i]] << endl;
	}

	cout << endl;
}

/*
*
a 6
b 7
c 3
d 5
e 3
*/

void MakeTarget(Node* nTarget, int *target)
{
	for (int i = 0; i < 40; i++)
	{
		nTarget->c[i] = target[i];
	}
}

Node* CalcSolution(Node** dp, Node* total, Node* vecSentence, int sCount)
{
	// init dp
	for (int i = 0; i <= sCount; i++)
	{
		dp[0][i].delta = GetDelta(total, &dp[0][i]);
		dp[i][0].delta = GetDelta(total, &dp[i][0]);
	}

	for (int i = 1; i <= sCount; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			int delta1 = dp[i - 1][j].delta;
			memcpy(dp[i][j].c, dp[i - 1][j - 1].c, sizeof(dp[i][j].c));
			AddNode(&dp[i][j], &vecSentence[i - 1]);
			int delta2 = GetDelta(total, &dp[i][j]);

			if (delta1 < delta2)
			{
				dp[i][j] = dp[i - 1][j];
			}
			else
			{
				dp[i][j].delta = delta2;
				dp[i][j].s = dp[i - 1][j - 1].s;
				dp[i][j].s.push_back(i - 1);
			}
		}
	}

	int min = INF;
	Node* minNode = NULL;

	for (int i = 1; i <= sCount; i++)
	{
		if (dp[sCount][i].delta < min)
		{
			min = dp[sCount][i].delta;
			minNode = &dp[sCount][i];
		}
	}

	return minNode;
}

int main()
{
	vector<string> sentences;
	sentences.push_back("aaa bbb ccc ddd eee");
	sentences.push_back("bbb rrr bbb ddd aaa");
	sentences.push_back("ggg bbb ddd aaa bbb");
	sentences.push_back("fff bbb ddd aaa bbb");
	sentences.push_back("hhh bbb ddd aaa bbb");
	sentences.push_back("rrr bbb ddd aaa bbb");
	sentences.push_back("rrr bbb ddd aaa bbb");
	sentences.push_back("ppp bbb ddd aaa bbb");
	sentences.push_back("rrr bbb ddd aaa bbb");
	sentences.push_back("rrr bbb ddd aaa bbb");
	sentences.push_back("eee ggg ddd aaa bbb");
	sentences.push_back("rrr ggg ddd aaa bbb");
	sentences.push_back("zzz bbb ddd aaa bbb");


	// You can change target by editing this variable;
	int target[40]
	{
		3,3,3,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0
	};

	int sCount = sentences.size();
	Node* vecSentence = new Node[sCount];
	Node count;
	Node nTarget;
	Node** dp = new Node * [sCount + 5];

	for (int i = 0; i < sCount + 5; i++)
	{
		dp[i] = new Node[sCount + 5];
	}

	MakeTarget(&nTarget, target);

	for (int i = 0; i < sentences.size(); i++)
	{
		Extract(sentences[i], &vecSentence[i]);
		AddNode(&count, &vecSentence[i]);
	}

	PrintCount(&count);
	PrintTotal(&nTarget);

	Node* sol = CalcSolution(dp, &nTarget, vecSentence, sCount);

	PrintOptimizedNode(sol, sentences);

	return 0;
}

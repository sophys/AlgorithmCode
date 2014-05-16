#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace std;

int minDistance(string word1, string word2) {

	 int rows = word1.size() + 1;
	 int cols = word2.size() + 1;
	 int ** d = (int**)malloc(rows * sizeof(int*));
	 for(int i = 0; i < rows; ++i){
		 d[i] = (int*)malloc(cols * sizeof(int));
		 d[i][0] = i;//sub string in word1 range [0,i) edit to ""
	 }
	 for(int j = 0; j < cols; ++j)
	   d[0][j] = j;//sub string in word2 range [0,j) edit to ""
	 for(int i = 1; i < rows; ++i) {
		 char ci = word1[i-1];
		 for(int j = 1; j < cols; ++j) {
			 char cj = word2[j-1];
			 //we will edit str1:word1[0,i) to str2:word2[0,j)
			 if (ci == cj) {
				 //if ci equal to cj, then the edit ditance of word1[0,i) to word2[0,j)
				 // is the same as word1[0,i-1) to word2[0,j-1)
				 d[i][j] = d[i-1][j-1];
			 } else {
				 //if we modify letter ci to cj, there will be 1 operation
				 int dEdit = d[i-1][j-1] + 1;
				 //if we add cj to the end of word1[0,i), then from the edit distance of 
				 // word1[0,i) to word2[0, j -1), we can conclude follow dist
				 int dAdd = d[i][j-1] + 1;
				 //if we delete ci from word1[0,i), and we know the dist
				 // from word1[0,i-1) to word2[0,j), the things done:
				 int dDel = d[i-1][j] + 1;
				 //the minimum one will be the final distance for str1 to str2
				 int min = dEdit < dAdd ? dEdit : dAdd;
				 min = min < dDel ? min : dDel;
				 d[i][j] = min;
			 }
		 }
	 }
	 int result = d[rows - 1][cols - 1];
	 //delete dist;
	 return result;
 }


int main()
{
	vector<string> input[257];
	string str;
	int n = 0,p;

	while(cin >> str)
	{
		p = 0;
		int len = str.length();
		for(int i = len -5 ; i < len + 5; i++)
		{
			if(input[i].size() > 0)
			{
				for(vector<string>::iterator it = input[i].begin();it != input[i].end(); it++)
				  if(minDistance(str,*it) <= 5)
					p++;
			}
		}
		if(p > n)
		  n = p;
		input[len].push_back(str);
	}
	cout << p << endl;
}


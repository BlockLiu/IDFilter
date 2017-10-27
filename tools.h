#ifndef TOOLS_H
#define TOOLS_H

#include "MurmurHash3.h"

class ELEMENT{
public:
	char *val;
	unsigned int category;

	ELEMENT(){}
	ELEMENT(char *v, unsigned int c);
	~ELEMENT();
};
ELEMENT::ELEMENT(char *v, unsigned int c){
	if(v != NULL){
		val = (char*)malloc(strlen(v));
		memcpy(val, v, strlen(v));
	}
	else val = NULL;
	category = c;
}
ELEMENT::~ELEMENT(){
	if(val != NULL)	free(val);
}



int NearestLarger2Power(int val){
	int ans = 1;
	while((1 << ans) <= val)
		ans++;
	return ans;
}

int CombinationNum(int n, int m){
	unsigned long ans = 1;
	for(int i = n; i > n - m; --i)
		ans *= i;
	for(int i = 2; i <= m; ++i)
		ans /= i;
	return ans;
}



unsigned int hash_k(char *elem, int k){
	unsigned int ans = 0;
	MurmurHash3_x86_32((void*)elem, strlen(elem), k, &ans);
	return ans;
}

#define MACHINE_WORD_SIZE 64
int numOfMemoryAccess(int pos, int len){
	int firstMachineWord = pos / MACHINE_WORD_SIZE;
	int lastMachineWord = (pos + len - 1) / MACHINE_WORD_SIZE;
	return (lastMachineWord - firstMachineWord + 1);
}



int SeperateLine(char *strLine, int &setID)
{
	strLine[strlen(strLine) - 1] = '\0';
	int i;
	while(strLine[i] != ' ')
		i++;
	strLine[i] = '\0';

	setID = atoi(strLine);
	return i + 1;
}


#endif
#ifndef COMB_H
#define COMB_H

#include "BloomFilter.h"
#include "tools.h"
#include <map>

class COMB{
	BloomFilter *bf;
	int m;
	int k;
	int setNum;
	int codeLen;
	int weight;
	unsigned int *codesOfSetID;
	std::map<unsigned int, int> setIDOfCodes;
public:
	int mc_ins, mc_que;
	COMB();
	COMB(int _m, int _k, int s);
	~COMB();

	void insert(ELEMENT *elem);
	int query(char *elem);
private:
	void setCodelenWithCombinationNum(int setNum);
	void setCodes(int n, int m, unsigned int ans);
};

COMB::COMB(){
	mc_ins = 0;
	mc_que = 0;
}
COMB::COMB(int _m, int _k, int s){
	mc_ins = 0;
	mc_que = 0;
	m = _m;
	k = _k;
	setNum = s;
	bf = new BloomFilter(m);
	setCodelenWithCombinationNum(setNum);
	setIDOfCodes.clear();
	codesOfSetID = new unsigned int[setNum + 1];
	memset(codesOfSetID, 0, (setNum + 1) * sizeof(unsigned int));
	setCodes(codeLen, weight, 0);
}
COMB::~COMB(){
	delete bf;
	delete codesOfSetID;
}
void COMB::setCodelenWithCombinationNum(int setNum){
	for(int i = 2; i > 0; ++i)
		if(CombinationNum(i, i/2) >= setNum){
			codeLen = i;
			weight = i/2;
			printf("setNum=%d, codeLen=%d, weight=%d\n", setNum, codeLen, weight);
			return;
		}
}
void COMB::setCodes(int n, int m, unsigned int ans){
	static int codeCnt = 0;
	if(codeCnt == setNum)	return;
	if(m == 0){
		//printf("codes of set-%d is %x\n", codeCnt+1, ans);
		codesOfSetID[++codeCnt] = ans;
		setIDOfCodes.insert(std::make_pair(ans, codeCnt));
		return;
	}
	for(int i = n - 1; i >= m - 1; --i){
		ans += (unsigned int)1 << i;
		setCodes(i, m - 1, ans);
		ans -= (unsigned int)1 << i;
	}
}

void COMB::insert(ELEMENT *elem){
	unsigned int setID = elem->category;
	unsigned int val = codesOfSetID[setID];
	for(int i = 0; i < codeLen; ++i)
		if((val & ((unsigned int)1 << (codeLen - 1 - i))) != 0)
			for(int j = i * k + 1; j <= (i + 1) * k; ++j)
			{
				int pos = hash_k(elem->val, j) % m;
				bf->set_1(pos);
				mc_ins++;
			}
}

int COMB::query(char *elem){
	unsigned int val = 0;
	int cnt = 0;
	for(int i = 0; i < codeLen; ++i)
	{
		int flag = 1;
		for(int j = i * k + 1; j <= (i + 1) * k; ++j){
			int pos = hash_k(elem, j) % m;
			mc_que++;
			if(bf->query(pos) != 1){
				flag = 0;
				break;
			}
		}
		if(flag == 1){
			if(++cnt > weight)
				return -1;
			val += (unsigned int)1 << (codeLen - 1 - i);
		}
	}
	if(cnt > weight)
		return -1;
	else if(cnt < weight)
		return 0;
	return setIDOfCodes[val];
}







#endif

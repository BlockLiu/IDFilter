#ifndef CODEDBF_H
#define CODEDBF_H

#include "BloomFilter.h"
#include "tools.h"

class CodedBF{
	BloomFilter *bf;
	int m;
	int k;
	int setNum;
	int codeLen;
public:
	int mc_ins, mc_que;
	CodedBF();
	CodedBF(int _m, int _k, int s);
	~CodedBF();

	void insert(ELEMENT *elem);
	int query(char *elem);
};

CodedBF::CodedBF(){
	mc_ins = 0; 
	mc_que = 0;
}
CodedBF::CodedBF(int _m, int _k, int s){
	mc_ins = 0; 
	mc_que = 0;
	m = _m;
	k = _k;
	setNum = s;
	codeLen = NearestLarger2Power(setNum);
	bf = new BloomFilter[codeLen];
	for(int i = 0; i < codeLen; ++i)
		bf[i].create_ary(_m / codeLen);
}
CodedBF::~CodedBF(){
	delete[] bf;
}

void CodedBF::insert(ELEMENT *elem){
	unsigned int val = (unsigned int)elem->category;
	for(int i = 0; i < codeLen; ++i)
		if((val & ((unsigned int)1 << (codeLen - 1 - i))) != 0){
			for(int j = i * k + 1; j <= (i + 1) * k; ++j)
			{
				int pos = hash_k(elem->val, j) % (m / codeLen);
				bf[i].set_1(pos);
				mc_ins++;
			}
		}
}

int CodedBF::query(char *elem){
	unsigned int val = 0;
	for(int i = 0; i < codeLen; ++i)
	{
		int flag = 1;
		for(int j = i * k + 1; j <= (i + 1) * k; ++j){
			int pos = hash_k(elem, j) % (m / codeLen);
			mc_que++;
			if(bf[i].query(pos) != 1){
				flag = 0;
				break;
			}
		}
		if(flag == 1)
			val += (unsigned int)1 << (codeLen - 1 - i);
	}
	return val;
}

#endif
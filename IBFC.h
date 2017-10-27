#ifndef IBFC_H
#define IBFC_H

#include "BloomFilter.h"
#include "tools.h"

class IBFC{
	BloomFilter *bf;
	int m;
	int k;
	int setNum;
	int codeLen;
public:
	int mc_ins, mc_que;
	IBFC();
	IBFC(int _m, int _k, int s);
	~IBFC();

	void insert(ELEMENT *elem);
	int query(char *elem);
private:
	unsigned long aux_val, aux_val_2;
};

IBFC::IBFC(){
	mc_ins = 0; 
	mc_que = 0;
}
IBFC::IBFC(int _m, int _k, int s){
	mc_ins = 0; 
	mc_que = 0;
	m = _m;
	k = _k;
	setNum = s;
	codeLen = NearestLarger2Power(setNum) * 2;
	bf = new BloomFilter(m);
	
	aux_val = 0;
	for(int i = 0; i < codeLen / 2; ++i)
		aux_val += (1 << i);
	aux_val_2 = 0;
	for(int i = 0; i < codeLen; ++i)
		aux_val_2 += (unsigned long)1 << i;
}
IBFC::~IBFC(){
	delete bf;
}

void IBFC::insert(ELEMENT *elem){
	unsigned long val = (unsigned long)(elem->category);
	val = val ^ aux_val;
	val += (unsigned long)(elem->category) << (codeLen / 2);

	for(int i = 0; i < k; ++i)
	{
		int pos = hash_k(elem->val, i) % m;
		unsigned char *tmpStr = bf->query(pos, codeLen);

		for(int j = 0; j < codeLen; ++j)
			if((val & ((unsigned long)1 << (codeLen - 1 - j))) != 0)
				tmpStr[j/8] |= (unsigned char)1 << (7 - (j % 8));

		bf->set(pos, codeLen, tmpStr);
		free(tmpStr);

		mc_ins += numOfMemoryAccess(pos, codeLen);
	}
}

int IBFC::query(char *elem){
	unsigned long ans = aux_val_2;

	for(int i = 0; i < k; ++i)
	{
		int pos = hash_k(elem, i) % m;
		unsigned char* tmpStr = bf->query(pos, codeLen);

		for(int j = 0; j < codeLen; ++j)
			if((tmpStr[j/8] & (1 << (7 - (j % 8)))) == 0)
				ans &= ~((unsigned long)1 << (codeLen - 1 - j));
		free(tmpStr);
		mc_que += numOfMemoryAccess(pos, codeLen);
	}

	unsigned long res = ans >> (codeLen / 2);
	//printf("%x\t", (int)res);
	if(((ans | res) & aux_val) != aux_val)
		return 0;
	if(((ans ^ res) & aux_val) != aux_val)
		return -1;
	return (int)res;
}



#endif
#ifndef IBF_H
#define IBF_H

#include "BloomFilter.h"
#include "tools.h"


class IBF{
	BloomFilter *bf;
	int m;				// number of bits in bf
	int k;				// number of hash functions
	int setNum;			// number of sets	
	int codeLen;		// number of bits in a code
public:
	int mc_ins, mc_que;
	IBF();
	IBF(int _m, int _k, int s);
	~IBF();

	void insert(ELEMENT *elem);
	int query(char *elem);
};

IBF::IBF(){
	mc_ins = 0; 
	mc_que = 0;
}
IBF::IBF(int _m, int _k, int s){
	mc_ins = 0; 
	mc_que = 0;
	m = _m;
	k = _k;
	setNum = s;
	codeLen = NearestLarger2Power(setNum);
	bf = new BloomFilter(m);
}
IBF::~IBF(){
	delete bf;
}

void IBF::insert(ELEMENT *elem){
	unsigned int val = elem->category;
	for(int i = 0; i < k; ++i)
	{
		int pos = hash_k(elem->val, i) % m;
		unsigned char* tmpStr = bf->query(pos, codeLen);

		for(int j = 0; j < codeLen; ++j)
			if((val & ((unsigned int)1 << (codeLen - 1 - j))) != 0)
				tmpStr[j / 8] |= 1 << (7 - (j % 8));

		bf->set(pos, codeLen, tmpStr);
		free(tmpStr);

		mc_ins += numOfMemoryAccess(pos, codeLen);
	}
}
int IBF::query(char *elem){
	unsigned int ans = 0;
	for(int i = 0; i < codeLen; ++i)
		ans |= (unsigned int)1 << i;
	for(int i = 0; i < k; ++i)
	{
		int pos = hash_k(elem, i) % m;
		unsigned char* tmpStr = bf->query(pos, codeLen);

		for(int j = 0; j < codeLen; ++j)
			if((tmpStr[j / 8] & (1 << (7 - (j % 8)))) == 0)
				ans &= ~((unsigned int)1 << (codeLen - 1 - j));
		free(tmpStr);
		mc_que += numOfMemoryAccess(pos, codeLen);
	}
	return (int)ans;
}





#endif
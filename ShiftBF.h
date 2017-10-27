#ifndef SHIFTBF_H
#define SHIFTBF_H

#include "BloomFilter.h"
#include "tools.h"

class ShiftBF{
	BloomFilter *bf;
	int m;
	int k;
	int setNum;
public:
	int mc_ins, mc_que;
	ShiftBF();
	ShiftBF(int _m, int _k, int s);
	~ShiftBF();

	void insert(ELEMENT *elem);
	int query(char *elem);
};

ShiftBF::ShiftBF(){
	mc_ins = 0;
	mc_que = 0;
}
ShiftBF::ShiftBF(int _m, int _k, int s){
	mc_ins = 0;
	mc_que = 0;
	m = _m;
	k = _k;
	setNum = s;
	bf = new BloomFilter(m);
}
ShiftBF::~ShiftBF(){
	delete bf;
}

void ShiftBF::insert(ELEMENT *elem){
	unsigned int val = elem->category;
	for(int i = 0; i < k; ++i)
	{
		int pos = hash_k(elem->val, i) % m;
		bf->set_1(pos);
		bf->set_1(pos + val);
		mc_ins++;
		if(numOfMemoryAccess(pos, val + 1) > 1)
			mc_ins++;
	}
}

int ShiftBF::query(char *elem){
	unsigned char *ans = (unsigned char*)malloc(setNum / 8 + 1);
	memset(ans, 0, setNum / 8 + 1);
	for(int i = 0; i <= setNum; ++i) 
		ans[i/8] |= (unsigned char)1 << (7 - (i%8));
	for(int i = 0; i < k; ++i)
	{
		int pos = hash_k(elem, i) % m;
		unsigned char *tmpStr = bf->query(pos, setNum + 1);

		for(int j = 0; j <= setNum; ++j)
			if((tmpStr[j / 8] & (1 << (7 - (j % 8)))) == 0)
				ans[j / 8] &= ~((unsigned char)1 << (7 - (j % 8)));

		mc_que += numOfMemoryAccess(pos, setNum + 1);
		free(tmpStr);
	}

	int setID = -1;
	if((ans[0] & 0x80) == 0)
		setID = 0;
	else{
		for(int i = 1; i <= setNum; ++i)
			if((ans[i/8] & (1 << (7 - (i % 8)))) != 0){
				if(setID > 0){
					setID = -1;
					break;
				}
				setID = i;
			}
	}
	free(ans);
	return setID;
}


#endif
#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <stdlib.h>
#include <string.h>

class BloomFilter{
	unsigned char *ary;
	int size;
public:
	BloomFilter(){}
	BloomFilter(int s);
	~BloomFilter();
	void create_ary(int s);
	void set_0(int pos);
	void set_1(int pos);
	void set(int pos, int length, unsigned char* val);
	int query(int pos);
	unsigned char* query(int pos, int length);
};

BloomFilter::BloomFilter(int s){
	create_ary(s);
}
void BloomFilter::create_ary(int s){
	size = s;
	ary = (unsigned char*)malloc((s - 1) / 8 + 1);
	memset(ary, 0, (s - 1) / 8 + 1);
}
BloomFilter::~BloomFilter(){
	free(ary);
}
void BloomFilter::set_0(int pos){
	pos = pos % size;
	ary[pos/8] &= ~((unsigned char)1 << (7 - (pos % 8)));
}
void BloomFilter::set_1(int pos){
	pos = pos % size;
	ary[pos/8] |= (unsigned char)1 << (7 - (pos % 8));
}
void BloomFilter::set(int pos, int length, unsigned char* val){
	for(int i = 0; i < length; ++i){
		int bit = (int)((val[i/8] & ((unsigned char)1 << (7 - (i %8)))) != 0);
		if(bit == 1)
			set_1(pos + i);
		else
			set_0(pos + i);
	}
}
int BloomFilter::query(int pos){
	pos = pos % size;
	return (int)((ary[pos/8] & ((unsigned char)1 << (7 - (pos % 8)))) != 0);
}
// remember to free str
unsigned char* BloomFilter::query(int pos, int length){
	unsigned char *str = (unsigned char*)malloc((length - 1) / 8 + 1);
	memset(str, 0, (length - 1) / 8 + 1);
	for(int i = 0; i < length; ++i){
		int offset = (pos + i) % size;
		int bit = (int)((ary[offset/8] & ((unsigned char)1 << (7 - (offset % 8)))) != 0);
		if(bit == 1)
			str[i / 8] |= (unsigned char)1 << (7 - (i % 8));
	}
	return str;
}





#endif
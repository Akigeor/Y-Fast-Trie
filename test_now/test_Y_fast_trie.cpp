#include <cstdio>
#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
#include "y-faststdmap.hpp"

int n;
int a[10005], c[10005];
char b[10005], d[10005];

int main() {
	/*
		find(key) = pair<bool, value>; // 
		erase(key) = bool;
		insert(key, value) = bool; // 
		findpre(key) = pair<bool, pair<key, value> >;
		findsuc(key) = pair<bool, pair<key, value> >;
	*/
	srand(time(0));	
	n = 40;
	for (int i = 1; i <= n; i ++) a[i] = rand() % 10000;
	for (int i = 1; i <= n; i ++) b[i] = (char)(rand() % 20 + 90);
	
	// Insert
	sjtu::xfast_trie<char> my_map(10000);
	std::map<int, char> std_map;
	for (int i = 1; i <= n; i ++) {
		my_map.insert(a[i], b[i]);
		if (!std_map.count(a[i])) std_map[a[i]] = b[i];
	}
	bool Right;
	int times = 100000;
	
	// Find Test
	Right = 1;
	for (int i = 1; i <= times; i ++) {
		int x = rand() % 10000;
		if (std_map.count(x)) {
			if (!(my_map.find(x).first)) Right = 0;
			if (my_map.find(x).second != std_map[x]) Right = 0;
		} else {
			if (my_map.find(x).first) Right = 0;
		}
	}
	printf("Find Test : ");
	if (Right) puts("Right!\n");
	else puts("Wrong Answer!\n");
	
	//Erase Test
	times = 1000;
	Right = 1;
	for (int i = 1; i <= times; i ++) {
		int x = rand() % 10000;
		if (std_map.count(x)) {
			std_map.erase(x);
			if (!my_map.find(x).first) Right = 0;
			my_map.erase(x);
		}
	}
	for (int i = 1; i <= n; i ++) {
		if ((int)my_map.find(a[i]).first != std_map.count(a[i])) Right = 0;
		if (std_map.count(a[i])) {
			if (my_map.find(a[i]).second != std_map[a[i]]) Right = 0;
		}
	}
	printf("Erase Test : ");
	if (Right) puts("Right!\n");
	else puts("Wrong Answer!\n");
	
	// pre suc
	Right = 1;
	int sum = 0;
	for (std::map<int, char>::iterator it = std_map.begin(); it != std_map.end(); it ++) {
		++ sum;
		c[sum] = it->first;
		d[sum] = it->second;
	}
	/*
		findpre(key) = pair<bool, pair<key, value> >;
		findsuc(key) = pair<bool, pair<key, value> >;
	*/
	for (int i = 1; i <= sum; i ++) {
		if (i > 1) {
			if (my_map.findpre(c[i]).first == 0) Right = 0;
			if (my_map.findpre(c[i]).second.first != c[i - 1]) Right = 0;
			if (my_map.findpre(c[i]).second.second != d[i - 1]) Right = 0;
		} else {
			if (my_map.findpre(c[i]).first == 1) Right = 0;
		}
		if (i < sum) {
			if (my_map.findsuc(c[i]).first == 0) Right = 0;
			if (my_map.findsuc(c[i]).second.first != c[i + 1]) Right = 0;
			if (my_map.findsuc(c[i]).second.second != d[i + 1]) Right = 0;
		} else {
			if (my_map.findsuc(c[i]).first == 1) Right = 0;
		}
	}
	printf("Pre-Suc Test : ");
	if (Right) puts("Right!\n");
	else puts("Wrong Answer!\n");
}

#include <cstdio>
#include <cstdlib>
#include <map>
#include <iostream>
#include <string>
#include "splay.hpp"
using namespace std;
int main()
{

	////////// 以下是如何使用

	auto t = new sjtu::Splay<int, string>();
	////////// Insert
	t -> insert(233, "abc");
	t -> insert(666, "efg");
	t -> insert(450, "hij");
	t -> insert(-1, "***");
	t -> insert(233, "xyz");  //会覆盖 <233, xyz>
	////////// Erase
	t -> erase(233);  //233消失了
	t -> erase(233);  //什么都不会发生
	auto t2 = new sjtu::Splay<int, string>();
	////////// Split
	auto t1t2 = sjtu::split(t);  //不要 调用(->)/delete t after doing this! 不过可以给t重新赋值
	t = t1t2.first, t2 = t1t2.second;
	////////// Size
	cout << t -> size() << " " << t2 -> size() << endl;  //cout 2 1
	////////// Min & Max
	cout << t -> min() << " " << t2 -> max() << endl;  //cout -1 666
	// t : {-1, 450} t2 : {666}
	////////// Merge
	auto newt = sjtu::merge(t, t2);  //不要 调用(->)/delete t, t2 after doing this! 不过可以给t, t2重新赋值
	////////// Find
	auto it = newt -> find(233);  //it == nullptr
	it = t -> find(666);  //it -> key == 666
	////////// <key, data>
	cout << "<" << it -> key << ", " << it -> data << ">" << endl;  //cout 666 efg
	////////// Previous & Next
	it = newt -> previous(it);  //it -> key == 450
	it = newt -> next(it);  //it -> key == 666
	it = newt -> next(it);  //it == nullptr
	////////// First one
	it = newt -> begin();  //it -> key == -1
	////////// Last one
	it = newt -> rbegin();  //it -> key == 666
	////////// first >=
	it = newt -> geq(449);  //it -> key == 450
	it = newt -> geq(450);  //it -> key == 450
	it = newt -> geq(667);  //it -> key == 666 (the last one)
	////////// first <=
	it = newt -> leq(449);  //it -> key == -1
	it = newt -> leq(450);  //it -> key == 450
	it = newt -> leq(-2);  //it -> key == -1 (the first one)

	////////// 以下是测试用代码
	
	/*int n = 1e4;
	sjtu::Splay<int, int> tree11, tree2, tree3;
	std::map<int, int> t1, t2;
	for (int i = 0; i < n * 2; ++ i)
	{
		int x = rand() % 200, y = rand() % 200;
		tree11.insert(x, y);
		t1[x] = y;
	}
	for (int i = 0; i < n; ++ i)
	{
		int x = rand() + rand();
		tree2.insert(x, x);
		t2[x] = x;
	}
	for (int i = 0; i <= n; ++ i)
	{
		int x = rand() % 1000, y = rand() % 1000;
		//int x = (rand() % 2 * 2 - 1) * (rand() << 15) + rand();
		tree3.insert(x, y);
		t1[x] = y;
	}*/
	/*tree1.merge(tree2);
	tree1.merge(tree3);
	tree1.split(tree3);*/
	/*auto tree1 = sjtu::merge(&tree11, &tree2);
	tree1 = sjtu::merge(tree1, &tree3);
	auto tmtmp = sjtu::split(tree1);
	auto tr1 = tmtmp.first, tr2 = tmtmp.second;
	printf("%d %d\n", tr1 ->size(), tr2 -> size());
	tree1 = sjtu::merge(tr1, tr2);
	t1.insert(t2.begin(), t2.end());
	printf("%d %d\n", tree1 -> size(), t1.size());
	printf("%d %d\n", tree1 -> max(), t1.rbegin() -> first);
	assert(tree1 -> max() == t1.rbegin() -> first);*/
	/*
	//auto tm = tree1.begin();
	//for (; tm; tm = tree1.next(tm)) printf("%d ", tm -> data);
	//puts("");
	//tm = tree3.begin();
	//for (; tm; tm = tree3.next(tm)) printf("%d ", tm -> data);
	//puts("");
	tree1.merge(tree3);
	t1.insert(t2.begin(), t2.end());
	for (int i = 0; i < n; ++ i)
	{
		int x = rand(); //, y = rand();
		//x = n - i + 1;
		tree1.erase(x);
		t1.erase(x);
		assert(t1.size() == tree1.size());
	}
	auto tmp = tree1.begin();
	auto tt = t1.begin();
	for (; tmp; tmp = tree1.next(tmp), ++ tt)
	{
		assert(tt -> first == tmp -> key);
		//cout << tt -> second << " " << tmp -> data << endl;
		assert(tt -> second == tmp -> data);
	}
	assert(tt == t1.end());
	for (int i = 0; i < n; ++ i)
	{
		int x = rand();
		x = i;
		auto a = tree1.leq(x);
		auto b = t1.upper_bound(x);
		if (b == t1.begin())
		{
			b = t1.end();
		}
		else
		{
			b --;
		}
		if (a)
		{
			assert(b != t1.end());
			assert(b -> first == a -> key);
			assert(b -> second == a -> data);
		}
		else
		{
			assert(b == t1.end());
		}
	}
	puts("correct");*/
}


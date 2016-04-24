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
	sjtu::Splay<int, string> t;
	t.insert(233, "abc");
	t.insert(666, "efg");
	t.insert(450, "hij");
	t.insert(-1, "***");
	t.insert(233, "xyz");  //会覆盖 <233, xyz>
	t.erase(233);  //233消失了
	t.erase(233);  //什么都不会发生
	sjtu::Splay<int, string> t2;
	t2.insert(555, "###");
	t.split(t2);  //等分 t2之前的东西会没了 t{-1, 450} t2{666}
	cout << t.size() << " " << t2.size() << endl;  //cout 2 1
	t.merge(t2);  //合并 t2会变为空
	auto it = t.find(234);  //it == nullptr
	it = t.find(666);  //it -> key == 666
	cout << "<" << it -> key << ", " << it -> data << ">" << endl;  //cout 666 efg
	it = t.previous(it);  //it -> key == 450
	it = t.next(it);  //it -> key == 666
	it = t.next(it);  //it == nullptr
	it = t.begin();  //it -> key == -1

	////////// 以下是测试用代码
	
	/*int n = 1e3;
	sjtu::Splay<int, int> tree1, tree2, tree3;
	std::map<int, int> t1, t2;
	for (int i = 0; i < n * 2; ++ i)
	{
		int x = rand() % 200, y = rand() % 200;
		tree1.insert(x, y);
		t1[x] = y;
	}
	for (int i = 0; i < n; ++ i)
	{
		int x = (rand() << 15) + rand();
		tree2.insert(x, x);
		t2[x] = x;
	}
	for (int i = 0; i <= n; ++ i)
	{
		int x = rand() % 1000, y = rand() % 1000;
		//int x = (rand() % 2 * 2 - 1) * (rand() << 15) + rand();
		tree3.insert(x, y);
		t1[x] = y;
	}
	tree1.merge(tree2);
	tree1.merge(tree3);
	tree1.split(tree3);
	printf("%d %d\n", tree1.size(), tree3.size());
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
	puts("correct");*/
}


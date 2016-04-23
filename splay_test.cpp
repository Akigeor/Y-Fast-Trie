#include <cstdio>
#include <cstdlib>
#include <set>
#include "splay.hpp"
int main()
{
	int n = 1e4;
	sjtu::Splay<int> tree1, tree2, tree3;
	std::set<int> t1, t2;
	for (int i = 0; i < n * 2; ++ i)
	{
		int x = rand();
		x = i;
		tree1.insert(x);
		t1.insert(x);
	}
	for (int i = 0; i < n; ++ i)
	{
		int x = (rand() << 15) + rand();
		tree2.insert(x);
		t2.insert(x);
	}
	for (int i = 0; i < n; ++ i)
	{
		int x = (rand() % 2 * 2 - 1) * (rand() << 15) + rand();
		tree3.insert(x);
		t1.insert(x);
	}
	tree1.merge(tree2);
	tree1.merge(tree3);
	tree1.split(tree3);
	//printf("%d %d\n", tree1.size(), tree3.size());
	/*auto tm = tree1.begin();
	for (; tm; tm = tree1.next(tm)) printf("%d ", tm -> data);
	puts("");
	tm = tree3.begin();
	for (; tm; tm = tree3.next(tm)) printf("%d ", tm -> data);
	puts("");*/
	tree1.merge(tree3);
	t1.insert(t2.begin(), t2.end());
	for (int i = 0; i < n; ++ i)
	{
		int x = rand();
		x = n - i + 1;
		tree1.erase(x);
		t1.erase(x);
		assert(t1.size() == tree1.size());
	}
	auto tmp = tree1.begin();
	auto tt = t1.begin();
	for (; tmp; tmp = tree1.next(tmp), ++ tt)
	{
		assert(*tt == tmp -> data);
	}
	assert(tt == t1.end());
	puts("correct");
}


#include<cstdio>
#include<memory>
#include "y-faststdmap.hpp"

int main(){
	freopen("asd.txt","r",stdin);
	sjtu :: xfast_trie<int>s(100);
	for (int i = 0;i < 18;i ++){
		int x;
		scanf("%d",&x);
		printf("i = %d\n",i);
		s.insert(x,x);
	}
	return 0;
}

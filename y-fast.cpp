#include<cstdio>
#include<utility>
#include <cstddef>
#include<algorithm>
#include<map>
#include <functional>
#include "splay.hpp"

template<class T>
class xfast_trie;
template<class T>
class xfast_node;
template<class T>
class xfast_node{
	friend xfast_trie<T>;
	private:
		xfast_node *son[2],*pre,*nxt,*des[2];
		int *key;
		sjtu :: Splay<int,T>* bstroot;
	public:
		xfast_node(){
			son[0] = son[1] = pre = nxt = des[0] = des[1] = nullptr;
			key = nullptr;
			bstroot = nullptr;
		}
		xfast_node(const int &_key){
			key = new int(_key);
			bstroot = new sjtu :: Splay<int,T>();
			son[0] = son[1] = pre = nxt = des[0] = des[1] = nullptr;
		}
};

template <class T>
class xfast_trie{
	friend xfast_node<T>;
	friend sjtu :: Splay<int,T>;
	private:
		int logM;
		xfast_node<T>* root;
		std :: map<int,xfast_node<T>*>* LSS;
	public:
		xfast_trie(int M){
			for (logM = 1;(1 << logM) <= M;logM ++);
			LSS = new std :: map<int,xfast_node<T>*> [logM + 1];
			for (int i = 0;i <= logM;i ++) LSS[i].clear();
			root = nullptr;
		}
		void dfs(xfast_node<T>* &u,int dep){
			if (u == nullptr) return;
			if (u->son[0] != nullptr) dfs(u->son[0],dep - 1);
			if (u->son[1] != nullptr) dfs(u->son[1],dep - 1);
			LSS[dep].erase(*(u->key));
			delete u->key;
			if (dep == 0) delete u->bstroot;
			u = nullptr;
		}
		~xfast_trie(){
			dfs(root,logM);
		}
		xfast_node<T>* successor(const int &key){
			if (root == nullptr) return nullptr;
			int l = -1,r = logM;
			while (l + 1 < r){
				int mid = l + r >> 1;
				if (LSS[mid].find(key >> mid) == LSS[mid].end()) l = mid;
				else r = mid;
			}
			xfast_node<T>* u = LSS[r][key >> r];
			if (r){
				if (u->son[1] == nullptr) return u->des[1]->nxt;
				else return u->des[0];
			}else return u;
		}
		std :: pair<int,T> findsuc(const int &key){
			xfast_node<T>* u = predecessor(key);
			xfast_node<T>* v = successor(key);
			if (u != nullptr) v = u->nxt;
			std :: pair<int,T> a,b;
			bool ha = false;
			bool hb = false;
			if (u != nullptr){
				a = u->bstroot->geq(key);
				ha = true;
			}
			if (v != nullptr){
				b = v->bstroot->geq(key);
				hb = true;
			}
			if (ha && a.first >= key)  return a;
			else return b;
		}
		xfast_node<T>* predecessor(const int &key){
			if (root == nullptr) return nullptr;
			int l = -1,r = logM;
			while (l + 1 < r){
				int mid = l + r >> 1;
				if (LSS[mid].find(key >> mid) == LSS[mid].end()) l = mid;
				else r = mid;
			}
			xfast_node<T>* u = LSS[r][key >> r];
			if (r){
				if (u->son[1] == nullptr) return u->des[1];
				else return u->des[0]->pre;
			}else return u;
		}
		std :: pair<int,T> findpre(const int &key){
			xfast_node<T>* v = predecessor(key);
			xfast_node<T>* u = nullptr;
			if (v != nullptr) u = v->pre;
			std :: pair<int,T> a,b;
			bool ha = false,hb = false;
			if (u != nullptr){
				a = u->bstroot->leq(key);
				ha = true;
			}
			if (v != nullptr) {
				b = v->bstroot->leq(key);
				hb = true;
			}
			if (hb && b.first <= key)  return b;
			else return a;
		}
		T find(const int &key){
			xfast_node<T> *u = predecessor(key);
			return u->bstroot->find(key);
		}
		xfast_node<T>* now;
		void updatedes(xfast_node<T>* u,int dep){
			if (u->son[0] != nullptr){
				if (dep > 1) u->des[0] = u->son[0]->des[0];
				else u->des[0] = u->son[0];
			}else{
				if (dep > 1) u->des[0] = u->son[1]->des[0];
				else u->des[0] = u->son[1];
			}
			if (u->son[1] != nullptr){
				if (dep > 1) u->des[1] = u->son[1]->des[1];
				else u->des[1] = u->son[1];
			}else{
				if (dep > 1) u->des[1] = u->son[0]->des[1];
				else u->des[1] = u->son[0];
			}
		}
		void walk(xfast_node<T>* &u,const int &key,int dep){
			if (dep == 0){
				u = now;
				LSS[dep][key] = u;
				return;
			}
			if (u == nullptr){
				u = new xfast_node<T>(key >> dep);
				LSS[dep][key >> dep] = u;
			}
			if ((key >> dep - 1) & 1) walk(u->son[1],key,dep - 1);
			else walk(u->son[0],key,dep - 1);
			updatedes(u,dep);
		}
		void delete_xfast_node(xfast_node<T>* &u,const int &key,int dep){
			if (dep == 0){
				LSS[dep].erase(key);
				delete u->key;
				u = nullptr;
				return;
			}
			if ((key >> dep - 1) & 1) delete_xfast_node(u->son[1],key,dep - 1);
			else delete_xfast_node(u->son[0],key,dep - 1);
			if (u->son[0] == nullptr && u->son[1] == nullptr){
				LSS[dep].erase(key >> dep);
				delete u->key;
				u = nullptr;
				return;
			}
			updatedes(u,dep);
		}
		void dealwithsize(xfast_node<T> *u){
			xfast_node<T> *tmp;
			if (u->bstroot->size() < logM / 4){
				if (u->pre == nullptr && u->nxt == nullptr) return;
				if (u->pre != nullptr){
					u->pre->bstroot = merge(u->pre->bstroot,u->bstroot);
					tmp = u->pre;
					xfast_node<T> *v = u->nxt;
					tmp->nxt = v;
					if (v != nullptr) v->pre = tmp;
					delete_xfast_node(root,*(u->key),logM);
					u = tmp;
				}else{
					xfast_node<T> *v = u->nxt->nxt;
					u->bstroot = merge(u->bstroot,u->nxt->bstroot);
					u->nxt = v;
					if (v != nullptr) v->pre = u;
					delete_xfast_node(root,*(u->key),logM);
				}
			}
			std :: pair<sjtu :: Splay<int,T>*,sjtu :: Splay<int,T>* > change;
			if (u->bstroot->size() > 2 * logM){
				change = sjtu :: split(u->bstroot);
				delete_xfast_node(root,*(u->key),logM);
				now = new xfast_node<T>((change.first)->min());
				now->bstroot = change.first;
				walk(root,(change.first)->min(),logM);
				now = new xfast_node<T>((change.second)->min());
				now->bstroot = change.second;
				walk(root,(change.second)->min(),logM);
			}
		}
		void insert(const int &key,const T &value){
			xfast_node<T>* u = predecessor(key);
			if (u != nullptr){
				u->bstroot->insert(key,value);
			}else{
				xfast_node<T>*v = root;
				for (int dep = logM;dep;dep --){
					if (v->son[0] != nullptr) v = v->son[0];
					else v = v->son[1];
				}
				now = u = new xfast_node<T>(key);
				v->pre = u;
				u->bstroot = new sjtu :: Splay<int,T>();
				u->bstroot->insert(key,value);
				walk(root,key,logM);
			}
			dealwithsize(u);
		}
		void strikeout(xfast_node<T>* &u,const int &key,int dep){
			if (dep == 0){
				LSS[dep].erase(key);
				delete u->key;
				u->bstroot->erase(key);
				if (u->bstroot->size() == 0){
					now = u = nullptr;return;
				}else now = u;
				return;
			}
			if ((key >> dep - 1) & 1) strikeout(u->son[1],key,dep - 1);
			else strikeout(u->son[0],key,dep - 1);
			if (u->son[0] == nullptr && u->son[1] == nullptr){
				LSS[dep].erase(key >> dep);
				delete u->key;
				u = nullptr;
				return;
			}
			updatedes(u,dep);
		}
		void erase(const int &key){
			xfast_node<T>* u = predecessor(key);
			printf("start strikeout\n");
			sjtu :: Splay<int,T>* tmp = u->bstroot;
			tmp->erase(key);
			u->bstroot = nullptr;
			xfast_node<T>* pp = u->pre;
			xfast_node<T>* ss = u->nxt;
			if (tmp->size() == 0){
				if (pp != nullptr) pp->nxt = ss;
				if (ss != nullptr) ss->pre = pp;
				delete_xfast_node(root,*(u->key),logM);
			}else{
				if (tmp->min() != (*(u->key))){
					if (pp != nullptr) pp->nxt = ss;
					if (ss != nullptr) ss->pre = pp;					
					delete_xfast_node(root,*(u->key),logM);
					now = new xfast_node<T>(tmp->min());
					now->bstroot = tmp;
					walk(root,tmp->min(),logM);
					dealwithsize(now);
				}
			}
			printf("finish strikeout\n");
		}
};
void test(){
	xfast_trie<int> set(1000000);
	printf("finish constructor\n");
	for (int i = 0;i < 13;i ++){
		set.insert(i * 2,i * 2);
	}
	for (int i = 0;i < 13;i ++){
		printf("%d %d\n",set.find(i * 2),set.findsuc(i).second);
	}
	printf("finish find and findsuc\n");
	for (int i = 5;i < 6;i ++)
		set.erase(i * 2);
	printf("finish erase\n");
	for (int i = 0;i < 5;i ++){
		printf("%d\n",set.findsuc(i * 2 + 4).second);
	}
}
int main(){
	test();
	return 0;
}

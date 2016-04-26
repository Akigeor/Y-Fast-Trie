#include<cstdio>
#include<cassert>
#include<utility>
#include<cstddef>
#include<algorithm>
#include<functional>
#include<map>
#include "splay.hpp"

namespace sjtu{

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
		xfast_node(int _key){
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
		int logM,B;
		xfast_node<T>* root;
		std :: map<int,xfast_node<T>*>* LSS;
	public:
		xfast_trie(int M){
			B = M;
			for (logM = 1;(1 << logM) <= M;logM ++);
			printf("logM = %d\n",logM);
			LSS = new std :: map<int,xfast_node<T>*> [logM + 1];
			root = nullptr;
		}
		void dfs(xfast_node<T>* &u,int dep){
			//printf("%d \n",dep);
			if (u == nullptr) return;
			if (u->son[0] != nullptr) dfs(u->son[0],dep - 1);
			if (u->son[1] != nullptr) dfs(u->son[1],dep - 1);
			printf("key = %d\n",*(u->key));
			int waste = *(u->key);
			LSS[dep].erase(waste);
			printf("start %d\n",dep);
			if (u->key == nullptr) printf("play game ganasdansdi a\n");
			if ((*(u->key)) >= 100) 
				printf("caonima\n");
			//delete u->key;
			printf("finsih deleda key\n");
			if (dep == 0 && u->bstroot != nullptr) delete u->bstroot;
			u = nullptr;
			printf("finish dep\n");
		}
		~xfast_trie(){
			printf("start fuck\n");
			//printf("asdasdad                                      %d\n",check(root));
			//dfs(root,logM);
		}
		xfast_node<T>* successor(int key){
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
		std :: pair<bool,std :: pair<int,T> >findsuc(int key){
			key ++;
			if (key < 0) key = 0;
			if (key >= B) return std :: make_pair(false,std :: make_pair(0,T()));
			xfast_node<T>* u = predecessor(key);
			xfast_node<T>* v;
			if (u != nullptr) v = u->nxt;
			else v = successor(key);
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
			if (ha && a.first >= key)  return std :: make_pair(true,a);
			else{
				if (hb && b.first >= key) return std :: make_pair(true,b);
				else return std :: make_pair(false,std :: make_pair(0,T()));
			}
		}
		xfast_node<T>* predecessor(int key){
			if (root == nullptr) return nullptr;
			//printf("startfind\n");
			int l = -1,r = logM;
			while (l + 1 < r){
				int mid = l + r >> 1;
				if (LSS[mid].find(key >> mid) == LSS[mid].end()) l = mid;
				else r = mid;
			}
			xfast_node<T>* u = LSS[r][key >> r];
			if (r){
				if (u->son[1] == nullptr){
					//printf("finish find\n");
					return u->des[1];
				}
				else{
					//printf("finish find\n");
					return u->des[0]->pre;
				}
			}else{
				//printf("finish find\n");
				return u;
			}
		}
		std :: pair<bool,std :: pair<int,T> >findpre(int key){
			key --;
			if (key >= B) key = B - 1;
			if (key < 0) return std :: make_pair(false,std :: make_pair(0,T()));
			xfast_node<T>* v = predecessor(key);
			std :: pair<int,T> before;
			bool have = false;
			if (v != nullptr) {
				before = v->bstroot->leq(key);
				have = true;
			}else before = std :: make_pair(0,T());
			return std :: make_pair(have,before);
		}
		std :: pair<bool,T> find(const int &key){
			if (count(key) == 0){
				return std :: make_pair(false,T());
			}
			xfast_node<T> *u = predecessor(key);
			return std :: make_pair(true,u->bstroot->find(key));
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
		void walk(xfast_node<T>* &u,int key,int dep){
			if (dep == 0){
				u = now;
				LSS[dep][key] = u;
				return;
			}
			if (u == nullptr){
				u = new xfast_node<T>(key >> dep);
				assert((*(u->key)) < 100);
				LSS[dep][key >> dep] = u;
			}
			if ((key >> dep - 1) % 2 == 1) walk(u->son[1],key,dep - 1);
			else walk(u->son[0],key,dep - 1);
			updatedes(u,dep);
		}
		void delete_xfast_node(xfast_node<T>* &u,int key,int dep){
			if (dep == 0){
				LSS[dep].erase(key);
				delete u->key;
				u = nullptr;
				return;
			}
			if ((key >> dep - 1) % 2 == 1) delete_xfast_node(u->son[1],key,dep - 1);
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
					//printf("start merge %d\n",u->bstroot->size());
					//printf("%d %d\n",u->pre->bstroot->size(),u->bstroot->size());
					u->pre->bstroot = sjtu :: merge(u->pre->bstroot,u->bstroot);
					u->bstroot = nullptr;
					//printf("finish merge\n");
					tmp = u->pre;
					xfast_node<T> *v = u->nxt;
					tmp->nxt = v;
					if (v != nullptr) v->pre = tmp;
					int waste = *(u->key);
					//printf("start delete\n");
					delete_xfast_node(root,waste,logM);
					//if (count(waste) != 0) printf("....................\n");
					//printf("finish delete\n");
					u = tmp;
					//if (u -> bstroot == nullptr) printf("caonima *****************\n");
				}else{
					xfast_node<T> *v = u->nxt->nxt;
					tmp = u->nxt;
					u->bstroot = sjtu :: merge(u->bstroot,u->nxt->bstroot);
					u->nxt->bstroot = nullptr;
					u->nxt = v;
					if (v != nullptr) v->pre = u;
					int waste = *(tmp->key);
					//printf("start delete\n");
					delete_xfast_node(root,waste,logM);
					//if (count(waste) != 0) printf(".................................\n");
					//if (u -> bstroot == nullptr) printf("caonima **************\n");
					//printf("finish delete\n");
				}
			}
			std :: pair<sjtu :: Splay<int,T>*,sjtu :: Splay<int,T>* > change;
			//printf("start split\n");
			if (u->bstroot->size() > 2 * logM){
				xfast_node<T>* pp = u->pre;
				xfast_node<T>* ss = u->nxt;
				xfast_node<T>* now2;
				if (ss != nullptr) ss->pre = pp;
				if (pp != nullptr) pp->nxt = ss;
				change = sjtu :: split(u->bstroot);
				int waste = *(u->key);
				u->bstroot = nullptr;
				delete_xfast_node(root,waste,logM);
				//if (count(waste) != 0) printf("................................\n");
				now = new xfast_node<T>((change.first)->min());
				now->bstroot = change.first;
				walk(root,*(now->key),logM);
				if (pp != nullptr) pp->nxt = now;
				now->pre = pp;
				now2 = new xfast_node<T>((change.second)->min());
				now2->bstroot = change.second;
				now->nxt = now2;
				now2->pre = now;
				now2->nxt = ss;
				if (ss != nullptr) ss->pre = now2;
				walk(root,*(now2->key),logM);
				//if (now -> bstroot == nullptr || now2->bstroot == nullptr) printf("caonima 6786788*********\n");
			}
			/*
			printf("start for\n");
			for (int i = 0;i < 100;i ++){
				xfast_node<T>*u = predecessor(i);
				//printf("%d\n",i);
				if (u != nullptr && u->bstroot == nullptr){
					printf("%d lisheng \n",i);
					break;
				}
			}
			*/
		}
		int check(xfast_node<T>*u){
			if (u == nullptr) return 1;
			int ret = 1;
			ret &= check(u->son[0]);
			ret &= check(u->son[1]);
			if ((*(u->key)) >= 100) ret = 0,printf("asdsadasdasdasdasdasdasdasdsadasdasdasdsadasdsad\n");
			return ret;
		}
		bool insert(int key,T value){
			if (count(key) == 1){
				return false;
			}
			//printf("start pre\n");
			xfast_node<T>* u = predecessor(key);
			//printf("finish pre\n");
			if (u != nullptr){
				u->bstroot->insert(key,value);
			}else{
				xfast_node<T>*v = root;
				for (int dep = logM;v != nullptr && dep;dep --){
					if (v->son[0] != nullptr) v = v->son[0];
					else v = v->son[1];
				}
				now = u = new xfast_node<T>(key);
				if (v != nullptr) v->pre = u;
				//printf("%d\n",v == nullptr);
				u->nxt = v;
				u->bstroot->insert(key,value);
				walk(root,key,logM);
			}
			//printf("start dealwithsize\n");
			dealwithsize(u);
			//printf("finish dealwithsize\n");
			//printf("start for\n");
			int past = -1;
			for (int i = 0;i < 100;i ++){
				xfast_node<T>*u = predecessor(i);
				//printf("%d\n",i);
				/*
				if (u != nullptr){
					if (u->bstroot->max() >= past) past = u->bstroot->max();
						else {
							printf("invalid\n");
							break;
						}
				}
				*/
				if (u != nullptr && u->bstroot == nullptr){
					
					printf("%d lisheng \n",i);
					break;
				}/*else if (u != nullptr) printf("%d\n",u->bstroot->min());*/
			}
			printf("special judge = %d\n",check(root));
			//printf("finish for\n");
			return true;
		}
		bool erase(const int &key){
			if (count(key) == 0){
				return false;
			}
			xfast_node<T>* u = predecessor(key);
			sjtu :: Splay<int,T>* tmp = u->bstroot;
			tmp->erase(key);
			u->bstroot = nullptr;
			xfast_node<T>* pp = u->pre;
			xfast_node<T>* ss = u->nxt;
			if (tmp->size() == 0){
				if (pp != nullptr) pp->nxt = ss;
				if (ss != nullptr) ss->pre = pp;
				int waste = *(u->key);
				delete_xfast_node(root,waste,logM);
			}else{
				if (tmp->min() != (*(u->key))){
					if (pp != nullptr) pp->nxt = ss;
					if (ss != nullptr) ss->pre = pp;
					int waste = *(u->key);
					delete_xfast_node(root,waste,logM);
					now = new xfast_node<T>(tmp->min());
					now->bstroot = tmp;
					walk(root,*(now->key),logM);
					dealwithsize(now);
				}else u->bstroot = tmp;
			}
			return true;
		}
		int count(int key){
			if (root == nullptr) return 0;
			//printf("start pre\n");
			xfast_node<T>* u = predecessor(key);
			//printf("end pre\n");
			if (u == nullptr) return 0;
			//printf("start geq\n");
			//printf("%d %d\n",u->bstroot != nullptr,*(u->key));
			int store = (u->bstroot->geq(key)).first;
			return store == key;
		}
};

}

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <memory>
#define SZ(x) ((x) ? ((x) -> sz) : 0)
namespace sjtu
{
	template<class T, class T2, class Compare = std::less<T>>
	class Splay
	{
		class Node
		{
		public:
			T key;
			Node *ch[2], *p;
			T2 data;
			int sz;
			Node(const T &key = T(), Node *p = nullptr, const T2 &data = T2()) : key(key), p(p), data(data)
			{
				ch[0] = ch[1] = nullptr;
				sz = 1;
			}
			inline bool isr() {return this == p -> ch[1];}
			inline void update() {sz = 1 + SZ(ch[0]) + SZ(ch[1]);}
		};
		Node* root;
		void rotate(Node *x)
		{
			assert(x -> p);
			bool s = x -> isr();
			Node *P = x -> p;
			P -> ch[s] = x -> ch[!s];
			if (x -> ch[!s]) x -> ch[!s] -> p = P;
			x -> ch[!s] = P;
			x -> p = P -> p;
			if (P -> p) P -> p -> ch[P -> isr()] = x;
			P -> p = x;
			P -> update();
			x -> update();
		}
		void splay(Node *x, Node *up = nullptr)
		{
			if (!up) root = x;
			while (x -> p != up)
			{
				if (x -> p -> p == up) rotate(x);
				else if (x -> isr() == x -> p -> isr())
				{
					rotate(x -> p), rotate(x);
				}
				else
				{
					rotate(x), rotate(x);
				}
			}
			x -> update();
		}
		std::pair<Node**, Node*> find(const T &x, Node *root)
		{
			if (!root) return std::make_pair(&root, nullptr);
			Node **cur = &root;
			Node *las = root -> p;
			while (*cur)
			{
				bool s = Compare()((*cur) -> key, x);
				bool t = Compare()(x, (*cur) -> key);
				if (!s && !t) return std::make_pair(cur, las);
				las = *cur;
				cur = &((*cur) -> ch[s]);
			}
			return std::make_pair(cur, las);
		}
		void insert_dfs(Node *x)
		{
			if (!x) return;
			insert(x -> key, x -> data);
			insert_dfs(x -> ch[0]);
			insert_dfs(x -> ch[1]);
		}
		void merge(Node *y)
		{
			if (!y) return;
			if (!root)
			{
				root = y;
				return;
			}
			if (root -> sz < y -> sz) std::swap(root, y);
			insert_dfs(y);
		}
		Node *rank(Node *x, int cnt)
		{
			if (!x) return x;
			int t = SZ(x -> ch[0]) + 1;
			if (t == cnt) return x;
			if (t > cnt) return rank(x -> ch[0], cnt);
			return rank(x -> ch[1], cnt - t);
		}
		void reset() {root = nullptr;}
		int cnt_max;
		void dfs(Node *x, int cnt = 0)
		{
			if (!x) return;
			cnt_max = std::max(cnt_max, cnt);
			if (x -> ch[0]) dfs(x -> ch[0], cnt + 1);
			if (x -> ch[1]) dfs(x -> ch[1], cnt + 1);
			delete x;
		}
		void destory()
		{
			cnt_max = 0;
			dfs(root);
			root = nullptr;
		}
	public:
		Splay() {root = nullptr;}
		~Splay() {destory();}
		Node *geq(const T &x)
		{
			if (!root) return nullptr;
			Node *cur = root;
			Node *ans = nullptr;
			while (cur)
			{
				bool s = Compare()(cur -> key, x);
				bool t = Compare()(x, cur -> key);
				if (!s && !t) return cur;
				if (!s) ans = cur;
				cur = cur -> ch[s];
			}
			if (ans) splay(ans); else ans = rbegin();
			return ans;
		}
		Node *leq(const T &x)
		{
			if (!root) return nullptr;
			Node *cur = root;
			Node *ans = nullptr;
			while (cur)
			{
				bool s = Compare()(cur -> key, x);
				bool t = Compare()(x, cur -> key);
				if (!s && !t) return cur;
				if (s) ans = cur;
				cur = cur -> ch[s];
			}
			if (ans) splay(ans); else ans = begin();
			return ans;
		}
		void insert(const T &x, const T2 &val)
		{
			auto p = find(x, root);
			if (!*p.first)
			{
				*p.first = new Node(x, p.second, val);
			}
			else
			{
				(*p.first) -> data = val;
			}
			splay(*p.first);
		}
		T min()
		{
			return begin() -> key;
		}
		T max()
		{
			return rbegin() -> key;
		}
		void erase(const T &x)
		{
			auto p = *find(x, root).first;
			if (!p) return;
			auto pp = previous(p);
			splay(p);
			if (!pp)
			{
				if (p -> ch[1]) p -> ch[1] -> p = nullptr;
				root = p -> ch[1];
			}
			else
			{
				splay(pp, p);
				pp -> ch[1] = p -> ch[1];
				if (p -> ch[1]) p -> ch[1] -> p = pp;
				root = pp;
				pp -> p = 0;
				pp -> update();
			}
			delete p;
		}
		void merge(Splay &other)
		{
			merge(other.root);
			other.reset();
		}
		void split(Splay &other)
		{
			other.destory();
			auto t = rank(root, (SZ(root) + 1) / 2);
			if (!t) return;
			splay(t);
			assert(root == t);
			other.root = root -> ch[1];
			if (other.root) (other.root) -> p = root -> ch[1] = nullptr;
			root -> update();
		}
		Node *find(const T &x)
		{
			return *find(x, root).first;
		}
		size_t size()
		{
			return (size_t) SZ(root);
		}
		bool empty()
		{
			return !root;
		}
		Node *next(Node *x)
		{
			assert(x);
			splay(x);
			Node *t = x -> ch[1];
			while (t && t -> ch[0]) t = t -> ch[0];
			return t;
		}
		Node *previous(Node *x)
		{
			assert(x);
			splay(x);
			Node *t = x -> ch[0];
			while (t && t -> ch[1]) t = t -> ch[1];
			return t;
		}
		Node *begin()
		{
			if (!root) return nullptr;
			Node* tmp;
			for (tmp = root; tmp -> ch[0]; tmp = tmp -> ch[0]);
			return tmp;
		}
		Node *end()
		{
			return nullptr;
		}
		Node *rbegin()
		{
			if (!root) return nullptr;
			Node* tmp;
			for (tmp = root; tmp -> ch[1]; tmp = tmp -> ch[1]);
			return tmp;
		}
	};
	template<class T, class T2, class Compare = std::less<T>>
	Splay<T, T2, Compare>* merge(Splay<T, T2, Compare> *a, Splay<T, T2, Compare> *b)
	{
		a -> merge(*b);
		delete b;
		return a;
	}
	template<class T, class T2, class Compare = std::less<T>>
	std::pair<Splay<T, T2, Compare>*, Splay<T, T2, Compare>*> split(Splay<T, T2, Compare> *a)
	{
		auto b = new Splay<T, T2, Compare>();
		a -> split(*b);
		return std::make_pair(a, b);
	}
};


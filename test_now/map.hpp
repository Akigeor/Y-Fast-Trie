#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map> // for test

namespace sjtu {

template<class Temp>
class unordered_map{
public:
	std::vector<std::vector<int> > T; // 0/1 represents "if a number exists"
	std::vector<std::vector<unsigned long long> > T2; // the key of stored things
	std::vector<std::vector<Temp> > value; // the key of stored things
	std::vector<std::pair<unsigned long long, Temp> > L; // a link for some elements
	std::vector<std::vector<std::pair<unsigned long long, Temp> > > LL; // a link for some elements
	std::vector<unsigned long long> m1, m2; // random prime for second-level bucket
	std::vector<int> s; // size of each second-level bucket
	std::vector<int> b; // number of elements
	std::vector<int> m; // a bound about T[j]
	int count; // counter
	int M; // a bound for all elements
	int c = 5; // a constant
	unsigned long long magic1, magic2; // for the top-level hashing
	int sum_sj; // sum of all s[j]
	int sM; // number of buckets now
	
	static int prime[80005];
	static bool flag[1000001];
	static int total_prime;
	
	void prepare() {
		for (int i = 2; i <= 1000000; i ++) {
			if (!flag[i]) {
				prime[total_prime ++] = i;
			}
			for (int j = 0; j < total_prime; j ++) {
				if (i * prime[j] > 1000000) break;
				flag[i * prime[j]] = 1;
				if (i % prime[j] == 0) break;
			}
		}
	}
	
	unordered_map() {
		if (!total_prime)
		{
			srand(time(0));
			prepare();
		}
		magic1 = prime[rand() % total_prime];
		magic2 = prime[rand() % total_prime];
		while (magic2 == magic1) {
			magic2 = prime[rand() % total_prime];
		}
		count = 0;
		M = 0;
		T.resize(0);
		T2.resize(0);
		value.resize(0);
		L.resize(0);
		b.resize(0);
		s.resize(0);
		m.resize(0);
		m1.resize(0);
		m2.resize(0);
		sum_sj = 0;
		sM = 0;
	}
	
	int hash(int x) {
		unsigned long long result = x;
		result = (result * magic1 + magic2) % M;
		return result;
	}
	
	int hash_j(int x, int j) {
		if (T.size() == 0 || s[j] == 0) return -1;
		unsigned long long result = x;
		result = (result * m1[j] + m2[j]) % s[j];
		return result;
	}
	
	bool is_injective(int j, std::vector<std::pair<unsigned long long, Temp> > &vec) {
		std::vector<int> tmp(s[j]);
		for (int i = 0; i < (int)vec.size(); i ++) {
			int HASH = hash_j(vec[i].first, j);
			if (tmp[HASH] == 0) 
				tmp[HASH] = 1;
			else 
				return 0;
		}
		return 1;
	}
	
	void full_re_hash(int x, Temp y) {
		L.clear();
		for (int i = 0; i < M; i ++) 
			if ((int)T[i].size() > 0) 
				for (int j = 0; j < (int)T[i].size(); j ++)
					if (T[i][j] != 0) L.push_back(std::make_pair(T2[i][j], value[i][j]));
		if (x >= 0) L.push_back(std::make_pair(x, y));
		count = (int)L.size();
		M = (1 + c) * std::max(count, 4);
		b.resize(M);
		m.resize(M);
		s.resize(M);
		m1.resize(M);
		m2.resize(M);
		T.clear();
		T.resize(M);
		T2.resize(M);
		value.resize(M);
		std::vector<int> tmp(M);
		do {
			tmp.clear();
			tmp.resize(M);
			magic1 = prime[rand() % total_prime];
			magic2 = prime[rand() % total_prime];
			while (magic2 == magic1) {
				magic2 = prime[rand() % total_prime];
			}
			for (int i = 0; i < count; i ++) {
				tmp[hash(L[i].first)] ++;
			}
			sum_sj = 0;
			sM = 0;
			for (int j = 0; j < M; j ++) {
				b[j] = tmp[j];
				m[j] = 2 * b[j];
				s[j] = 2 * m[j] * (m[j] - 1);
				if (tmp[j] == 0) continue;
				sum_sj += s[j];
				sM ++;
			}
		} while (sum_sj > 32 * M * M / sM + 4 * M);
		LL.clear();
		LL.resize(M);
		for (int i = 0; i < (int)L.size(); i ++) {
			LL[hash(L[i].first)].push_back(L[i]);
		}
		for (int j = 0; j < M; j ++) {
			if (tmp[j] == 0) continue;
			T[j].clear();
			T[j].resize(s[j]);
			do {
				m1[j] = prime[rand() % total_prime];
				m2[j] = prime[rand() % total_prime];
				while (m1[j] == m2[j]) {
					m2[j] = prime[rand() % total_prime];
				}
			} while (!is_injective(j, LL[j]));
			T[j].clear();
			T[j].resize(s[j]);
			T2[j].resize(s[j]);
			value[j].resize(s[j]);
			for (int i = 0; i < (int)LL[j].size(); i ++) {
				T[j][hash_j(LL[j][i].first, j)] = 1;
				T2[j][hash_j(LL[j][i].first, j)] = LL[j][i].first;
				value[j][hash_j(LL[j][i].first, j)] = LL[j][i].second;
			}
		}
	}
	
	int find_second_address(int j, int x) {
		if (T.size() == 0 || s[j] == 0) return -1;
		if (T[j][x] != 0) return T2[j][x];
		return -1;
	}
	
	bool find(int x) {
		if (M == 0) return 0;
		int j = hash(x); // position in first-level
		int k = hash_j(x, j);
		if (k == -1) return 0;
		if (find_second_address(j, k) == x) return 1;
		return 0;
	}
	
	Temp locate(int x) {
		int j = hash(x); // position in first-level
		int k = hash_j(x, j);
		return value[j][k];
	}
	
	void insert(std::pair<int, Temp> x) {
		count ++;
		if (count > M) {
			full_re_hash(x.first, x.second);
		} else {
			int j = hash(x.first);
			int k = find_second_address(j, hash_j(x.first, j));
			if (k != x.first) {
				b[j] ++;
				L.clear();
				for (int i = 0; i < (int)T[j].size(); i ++) {
					if (T[j][i] != 0) L.push_back(std::make_pair(T2[j][i], value[j][i]));
				}
				if (b[j] <= m[j]) {
					if (k == -1) {
						T[j][hash_j(x.first, j)] = x.first;
						L.push_back(std::make_pair(x.first, x.second));
					} else {
						L.push_back(std::make_pair(x.first, x.second));
						b[j] = L.size();
						do {
							m1[j] = prime[rand() % total_prime];
							m2[j] = prime[rand() % total_prime];
							while (m1[j] == m2[j]) {
								m2[j] = prime[rand() % total_prime];
							}
						} while (!is_injective(j, L));
						T[j].clear();
						T[j].resize(s[j]);
						T2[j].resize(s[j]);
						value[j].resize(s[j]);
						for (int i = 0; i < (int)L.size(); i ++) {
							T[j][hash_j(L[i].first, j)] = 1;
							T2[j][hash_j(L[i].first, j)] = L[i].first;
							value[j][hash_j(L[i].first, j)] = L[i].second;
						}
					}
				} else {
					m[j] = 2 * std::max(1, m[j]);
					sum_sj -= s[j];
					s[j] = 2 * m[j] * (m[j] - 1);
					sum_sj += s[j];
					if (sum_sj <= 32 * M * M / sM + 4 * M) {
						L.push_back(x);
						b[j] = L.size();						
						do {
							m1[j] = prime[rand() % total_prime];
							m2[j] = prime[rand() % total_prime];
							while (m1[j] == m2[j]) {
								m2[j] = prime[rand() % total_prime];
							}
						} while (!is_injective(j, L));
						T[j].clear();
						T[j].resize(s[j]);
						T2[j].resize(s[j]);
						value[j].resize(s[j]);
						for (int i = 0; i < (int)L.size(); i ++) {
							T[j][hash_j(L[i].first, j)] = 1;
							T2[j][hash_j(L[i].first, j)] = L[i].first;
							value[j][hash_j(L[i].first, j)] = L[i].second;
						}
					} else {
						full_re_hash(x.first, x.second);
					}
				}
			}
		}
	}
	
	Temp & operator[](const int &x) {		
		if (find(x)) {
			int j = hash(x); // position in first-level
			int k = hash_j(x, j);
			return value[j][k];
		}
		insert(std::make_pair(x, Temp()));
		int j = hash(x); // position in first-level
		int k = hash_j(x, j);
		return value[j][k];
	}
	
	void erase(int x) {
		count ++;
		int j = hash(x);
		if (find_second_address(j, hash_j(x, j)) == x) {
			T[j][hash_j(x, j)] = 0;
		} else return;
		if (count >= M) {
			full_re_hash(-1, Temp());
		}
	}
};

template<class Temp>
int unordered_map<Temp>::total_prime = 0;
template<class Temp>
int unordered_map<Temp>::prime[] = {0};
template<class Temp>
bool unordered_map<Temp>::flag[] = {0};

};

/*
int a[100005];

int main() {
	freopen("test.out", "w", stdout);
	for (int n = 25; n <= 3000; n ++) {
		printf("n = %d\n", n);
		unordered_map<int> map;
		std::map<int, int> M;
		for (int i = 1; i <= n; i ++)
			a[i] = rand();
		printf("FUZHI complete.\n");
		for (int i = 1; i <= n; i ++) {
			map[i] = a[i];
			M[i] = a[i];
		}
		printf("Insert complete.\n");
		for (int i = 1; i <= n * 10; i ++) {
			//printf("%d\n", i);
			int x = rand() % n + 1;
			if (map[x] != M[x]) puts("Wrong Answer(exist).\n");
		}
		printf("Ask1 complete.\n");
		for (int i = 1; i <= n * 10; i ++) {
			//printf("%d\n", i);
			int x = rand();
			if (map.find(x) != M.count(x)) {
				printf("%d %d\n", map.find(x), M.count(x));
				int k = 1;
				map.find(x);
				puts("Wrong Answer(random number).\n");
			}
		}
		printf("Ask2 complete.\n");
	}
	return 0;
}
*/

#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

namespace BigNum
{
	const int WS = 10000;

	class BigNum
	{
	private:
		vector<int> _num;
		int _len;
	public:
		BigNum()
		{
			_num.clear();
			_len = 1;
		}
		~BigNum() {}
		BigNum(const int num);
		BigNum(const char* Str);

		friend istream& operator >> (istream& in, BigNum& Num);
		friend ostream& operator << (ostream& out, const BigNum& Num);

		friend bool operator < (const BigNum& A, const BigNum& B);
		friend bool operator > (const BigNum& A, const BigNum& B);
		friend bool operator == (const BigNum& A, const BigNum& B);
		friend bool operator >= (const BigNum& A, const BigNum& B);
		friend bool operator <= (const BigNum& A, const BigNum& B);
		friend bool operator != (const BigNum& A, const BigNum& B);

		friend BigNum operator + (const BigNum& A, const BigNum& B);
		friend BigNum operator - (const BigNum& A, const BigNum& B);
		friend BigNum operator * (const BigNum& A, const BigNum& B);
		friend BigNum operator / (const BigNum& A, const BigNum& B);
		friend BigNum operator ^ (const BigNum& A, const int p);
	};

	BigNum::BigNum(int num)
	{
		_len = 0;
		_num.resize(20);			// int转不会太大
		while (true)
		{
			_num[++_len] = num % WS;
			num /= WS;
			if (num == 0)
				break;
		}
	}

	BigNum::BigNum(const char* Str)
	{
		int len = strlen(Str + 1), i;
		_len = 0;

		for (i = len; i >= 4; i -= 4)
		{
			int t = 1;
			_len++;
			for (int j = 0; j < 4; j++, t = t * 10)
				_num[_len] += (Str[i - j] - '0') * t;
		}
		if (i)
		{
			int t = 1;
			_len++;
			for (int j = 0; j < i; j++, t = t * 10)
				_num[_len] += (Str[i - j] - '0') * t;
		}
	}

	istream& operator>>(istream& in, BigNum& Num)
	{
		string Str;
		in >> Str;
		Num = Str.c_str();
		return in;
	}

	ostream& operator<<(ostream& out, const BigNum& Num)
	{
		cout << Num._num[Num._len];
		for (int i = Num._len - 1; i >= 1; i--)
			cout << setw(4) << setfill('0') << Num._num[i];
		return out;
	}

	bool operator<(const BigNum& A, const BigNum& B)
	{
		if (A._len < B._len) return true;
		if (A._len > B._len) return false;
		for (int i = A._len; i >= 1; i--)
		{
			if (A._num[i] < B._num[i]) return true;
			if (A._num[i] > B._num[i]) return false;
		}
		return false;
	}

	bool operator>(const BigNum& A, const BigNum& B)
	{
		if (A._len < B._len) return false;
		if (A._len > B._len) return true;
		for (int i = A._len; i >= 1; i--)
		{
			if (A._num[i] < B._num[i]) return false;
			if (A._num[i] > B._num[i]) return true;
		}
		return false;
	}

	bool operator==(const BigNum& A, const BigNum& B)
	{
		if (A._len != B._len) return false;
		for (int i = 1; i <= A._len; i++) if (A._num[i] != B._num[i]) return false;
		return true;
	}

	bool operator>=(const BigNum& A, const BigNum& B)
	{
		return A > B || A == B;
	}

	bool operator<=(const BigNum& A, const BigNum& B)
	{
		return A < B || A == B;
	}

	bool operator!=(const BigNum& A, const BigNum& B)
	{
		return !(A == B);
	}

	BigNum operator+(const BigNum& A, const BigNum& B)
	{
		BigNum C;
		C._len = A._len > B._len ? A._len : B._len;	//Get Max
		C._num.resize(C._len << 1);
		for (int i = 1; i <= C._len; i++)
		{
			C._num[i] += A._num[i] + B._num[i];
			C._num[i + 1] = C._num[i] / WS;
			C._num[i] %= WS;
		}
		if (C._num[C._len + 1])
			C._len++;
		return C;
	}

	BigNum operator-(const BigNum& A, const BigNum& B)
	{
		BigNum C;
		C._len = A._len > B._len ? A._len : B._len;
		C._num.resize(C._len << 1);
		for (int i = 1; i <= C._len; i++)
		{
			C._num[i] += A._num[i] - B._num[i];
			if (C._num[i] < 0)
			{
				C._num[i] += WS;
				C._num[i + 1]--;
			}
		}
		while (!C._num[C._len])
			C._len--;
		return C;
	}

	BigNum operator*(const BigNum& A, const BigNum& B)
	{
		BigNum C;
		C._len = A._len + B._len + 1;
		C._num.resize(C._len << 1);
		for (int i = 1; i <= A._len; i++)
			for (int j = 1; j <= B._len; j++)
			{
				C._num[i + j - 1] += A._num[i] * B._num[j];
				C._num[i + j] += C._num[i + j - 1] / WS;
				C._num[i + j - 1] %= WS;
			}
		while (!C._num[C._len])
			C._len--;
		return C;
	}

	BigNum operator/(const BigNum& A, const BigNum& B)
	{
		BigNum C;
		C._num.resize(A._len << 1);
		BigNum T;
		T._num.resize(A._len << 1);
		for (int i = A._len; i >= 1; i--)
		{
			T = T * WS; T._num[1] = A._num[i];
			while ((T > B) || (T == B))
			{
				T = T - B;
				C._num[i]++;
			}
		}
		C._len = A._len;

		while (!C._num[C._len] && C._len >= 2)
			C._len--;
		return C;
	}
	BigNum operator^(const BigNum& A, const int p)
	{
		BigNum C = 1;
		BigNum Base = A;
		int exp = p;
		while (true)
		{
			if (exp & 1) C = C * Base;
			Base = Base * Base;
			exp >>= 1;
			if (!exp)
				return C;
		}
	}
}


int main()
{
	BigNum::BigNum A = 2;
	A = A ^ 1000;
	cout << A << endl;
	return 0;
}


#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <cmath>

typedef unsigned long long int ull;
using namespace std;
const int MAX_PRIME = (int)1E5;

void generate_primes(vector <unsigned int>& primes)
{
	primes.push_back(2);

	for (unsigned int i = 3; i < MAX_PRIME; i+=2) 
	{
		bool is_prime = true;
		for (unsigned int j = 0; j < primes.size() && primes[j] * primes[j] <= i; j++)
		{
			if (i % primes[j] == 0)
			{
				is_prime = false;
				break;
			}
		}
		if (is_prime) primes.push_back(i);
	}

	ofstream out("./primes.txt");
	for (vector <unsigned int> ::const_iterator i = primes.begin(); i != primes.end(); ++i)
	{
		out << *i << endl;
	}
	out.close();
}

void get_primes(string file, vector <unsigned int>& primes)
{
	ifstream in;
	in.open("primes.txt");

	// list of primes will be generated only once
	if (!in.is_open())
	{
		generate_primes(primes);
		return;
	}

	string s;
	while (getline(in, s))
	{
		if (s.size() > 0)
		{
			primes.push_back(stoi(s));
		}
	}

	in.close();
}

ull inv(ull a, ull b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}

int main()
{
	vector <unsigned int> primes;
	get_primes("./primes.txt", primes);

	// Pick two different random primes (p, q), p * q = n
	// Note: (p, q) > sqrt(65537) to avoid possible problem with generating open key in future
	srand(time(NULL));
	unsigned int p = rand() % (primes.size() - 55) + 55;
	unsigned int q = rand() % (primes.size() - 55) + 55;
	// Algorithm will work slower with code like this, but I think it isn't good to use IF statements in encryption algorithms.
	q += (q == p) * ((q < primes.size() - 1) - (q == primes.size() - 1));

	/* Debug numbers:
	primes[p] = 3557;
	primes[q] = 2579;*/

	ull n = (ull)primes[p] * (ull)primes[q];

	// Calculate Euler's function f = (p - 1) * (q - 1)
	ull f = (ull)(primes[p] - 1) * (ull)(primes[q] - 1);

	// Generate open exp
	vector <unsigned int> ferma_numbers { 17, 257, 65537 };
	unsigned int r = rand() % 3;
	unsigned int e = ferma_numbers[r] * (ferma_numbers[r] % f != 0) + ferma_numbers[(r + 1) % 3] * (ferma_numbers[r] % f == 0);
	
	/* Debug numbers: 
	e = 3;*/

	// Generate close exp
	ull d = inv((ull)e, f);

	// Interface
	pair <unsigned int, ull> public_key(e, n);
	pair <ull, ull> private_key(d, n);
	ull msg;

	cout << "Public key: {" << e << ", " << n << "}\n";
	cout << "Private key; {" << d << ", " << n << "}\n\n";

	// There should be a 'BigInt.cpp' solution...
	cout << "Enter numeric message to encrypt: ";
	cin >> msg;
	ull encrypted = inv(pow(msg, e), n);
	cout << "Encrypted: " << encrypted << endl;

	cout << "Enter numeric message to decrypt: ";
	cin >> msg;
	ull decrypted = inv(pow(encrypted, d), n);
	cout << "Decrypted: " << decrypted;
}
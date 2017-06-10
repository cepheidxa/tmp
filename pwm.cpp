#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct freq {
	double f;
	double source;
	int pwm_size;
	int m;
	int p;
};

bool operator<(const freq & lhs, const freq & rhs)
{
	return lhs.f < rhs.f;
}

int calculate_peroid(double source, vector < freq > &freqs)
{
	int available_p[] = { 1, 3, 5, 6 };
	int available_pwm_size[] = { 6, 9 };

	for (int i = 0;
	     i < sizeof(available_pwm_size) / sizeof(available_pwm_size[0]);
	     i++) {
		int pwm_size = available_pwm_size[i];
		for (int m = 0; m < 8; m++) {
			for (int j = 0;
			     j < sizeof(available_p) / sizeof(available_p[0]);
			     j++) {
				int p = available_p[j];
				freq finfo;
				finfo.f =
				    (source * (1 << m)) / ((1 << pwm_size) * p *
							   1.0);
				finfo.source = source;
				finfo.pwm_size = pwm_size;
				finfo.m = m;
				finfo.p = p;
				freqs.push_back(finfo);
			}
		}
	}
}

int main()
{
	vector < freq > freqs;
	calculate_peroid(1.024, freqs);
	calculate_peroid(32.768, freqs);
	calculate_peroid(19200, freqs);
	stable_sort(freqs.begin(), freqs.end());
 for (const freq & e:freqs)
		cout << "f = " << e.f << ", source = " << e.
		    source << ", pwm_size = " << e.pwm_size << ", m = " << e.
		    m << ", p = " << e.p << endl;
}

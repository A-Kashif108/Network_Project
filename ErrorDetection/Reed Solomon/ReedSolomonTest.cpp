#include "ReedSolomon.h"
#include <time.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
using namespace std;
const double prob = 0.25;
int successes1 = 0,successes2 = 0,successes3 = 0,successes4 = 0 ,trials1 = 0,trials2 = 0,trials3 = 0,trials4 = 0,detection1=0,detection2=0,detection3=0,detection4=0;
bool fp1=0,fp2=0,fp3=0,fp4=0;
int nfp1=0,nfp2=0,nfp3=0,nfp4=0;

// Function to generate a random value between 0 and 1
double generateRandomValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

//Function to introduce error with probability p
void data_error(vector<unsigned int> &encoded, int sz){
    double bit_prob = 0.25;
    for(int i=0; i<=sz-1; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
           encoded.push_back(i);
            bit_prob /= 2;
        }
    }
}

//Function to introduce error with probability p
void singlebit_error(vector<unsigned int> &encoded, int sz){
    double bit_prob = 0.25;
    for(int i=0; i<=sz-1; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
           encoded.push_back(i);
            bit_prob /= 50;
        }
    }
}

//Function to introduce error with probability p
void multiplebit_error(vector<unsigned int>&encoded, int sz){
    double bit_prob = 0.25;
    for(int i=0; i<=sz-1; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
           encoded.push_back(i);
        }
    }
}

//Function to introduce error with probability p
void burst_error(vector<unsigned int>&encoded, int sz){
    double bit_prob = 0.1;
    for(int i=0; i<=sz-1; i++){
        double rand_value = generateRandomValue();
        int burst_len[] = {2, 3, 4};
        if (rand_value <= bit_prob) {
            int len = rand()%3+2;
            for(int j=i; j<=min(sz, i+len); j++)
                encoded.push_back(i);
        }
    }
}

bool test_data_error(int bits, int k, int nsym, int ncorr, bool print, bool* unmatch, int erasures)
{
	RS_WORD *originalData = (RS_WORD*)malloc(sizeof(RS_WORD) * (k + nsym));
	for (int i = 0; i < k; i++)
	{
		originalData[i] = rand() % (1 << bits);
	}
	Poly msg(k, originalData);
	Poly a(k + nsym, originalData);
	ReedSolomon rs(bits);
    auto t1 = chrono::high_resolution_clock::now();
	rs.encode(a.coef, originalData, k, nsym);
     auto t2 = chrono::high_resolution_clock::now();
	vector<unsigned int> possPos, corrPos, erasePos;
    data_error(corrPos,k+nsym);

    if(!corrPos.empty())trials1++;
    else return false;
//    singlebit_error(corrPos,k+nsym);
//    multiplebit_error(corrPos,k+nsym);
//    burst_error(corrPos,k+nsym);

	for (unsigned char i : corrPos)
	{
		a.coef[i] = rand() % (1 << bits);
		if (print) cout << (int)i << " ";
	}
     auto t3 = chrono::high_resolution_clock::now();
	bool success = rs.decode(a.coef, msg.coef, a.coef, k, nsym, erasures ? &erasePos : nullptr, print,detection1);
     auto t4 = chrono::high_resolution_clock::now();

//    chrono::duration<double, std::milli> t11;t11+=(t2-t1);
//    chrono::duration<double, std::milli> t13;t13+=(t4-t3);
//    std::cout << t11.count() << "ms\n";
//    std::cout << t13.count() << "ms\n";
	for (int i = 0; i < k; i++)
	{
		if (msg.coef[i] != originalData[i])
		{
			if (unmatch)
			{
				*unmatch = true;
			}
			break;
		}
	}
	return success;
}
bool test_singlebit_error(int bits, int k, int nsym, int ncorr, bool print, bool* unmatch, int erasures)
{
    RS_WORD *originalData = (RS_WORD*)malloc(sizeof(RS_WORD) * (k + nsym));
    for (int i = 0; i < k; i++)
    {
        originalData[i] = rand() % (1 << bits);
    }
    Poly msg(k, originalData);
    Poly a(k + nsym, originalData);
    ReedSolomon rs(bits);
    rs.encode(a.coef, originalData, k, nsym);
    vector<unsigned int> possPos, corrPos, erasePos;
//    data_error(corrPos,k+nsym);
    singlebit_error(corrPos,k+nsym);
    if(!corrPos.empty())trials2++;
    else return false;

//    multiplebit_error(corrPos,k+nsym);
//    burst_error(corrPos,k+nsym);

    for (unsigned char i : corrPos)
    {
        a.coef[i] = rand() % (1 << bits);
        if (print) cout << (int)i << " ";
    }

    bool success = rs.decode(a.coef, msg.coef, a.coef, k, nsym, erasures ? &erasePos : nullptr, print,detection2);

    for (int i = 0; i < k; i++)
    {
        if (msg.coef[i] != originalData[i])
        {
            if (unmatch)
            {
                *unmatch = true;
            }
            break;
        }
    }
    return success;
}
bool test_multiplebit_error(int bits, int k, int nsym, int ncorr, bool print, bool* unmatch, int erasures)
{
    RS_WORD *originalData = (RS_WORD*)malloc(sizeof(RS_WORD) * (k + nsym));
    for (int i = 0; i < k; i++)
    {
        originalData[i] = rand() % (1 << bits);
    }
    Poly msg(k, originalData);
    Poly a(k + nsym, originalData);
    ReedSolomon rs(bits);
    rs.encode(a.coef, originalData, k, nsym);
    vector<unsigned int> possPos, corrPos, erasePos;
//    data_error(corrPos,k+nsym);
//    singlebit_error(corrPos,k+nsym);
    multiplebit_error(corrPos,k+nsym);
    if(!corrPos.empty())trials3++;
    else return false;

//    burst_error(corrPos,k+nsym);

    for (unsigned char i : corrPos)
    {
        a.coef[i] = rand() % (1 << bits);
        if (print) cout << (int)i << " ";
    }

    bool success = rs.decode(a.coef, msg.coef, a.coef, k, nsym, erasures ? &erasePos : nullptr, print,detection3);

    for (int i = 0; i < k; i++)
    {
        if (msg.coef[i] != originalData[i])
        {
            if (unmatch)
            {
                *unmatch = true;
            }
            break;
        }
    }
    return success;
}
bool test_burst_error(int bits, int k, int nsym, int ncorr, bool print, bool* unmatch, int erasures)
{
    RS_WORD *originalData = (RS_WORD*)malloc(sizeof(RS_WORD) * (k + nsym));
    for (int i = 0; i < k; i++)
    {
        originalData[i] = rand() % (1 << bits);
    }
    Poly msg(k, originalData);
    Poly a(k + nsym, originalData);
    ReedSolomon rs(bits);
    rs.encode(a.coef, originalData, k, nsym);
    vector<unsigned int> possPos, corrPos, erasePos;
//    data_error(corrPos,k+nsym);
//    singlebit_error(corrPos,k+nsym);
//    multiplebit_error(corrPos,k+nsym);
    burst_error(corrPos,k+nsym);

    if(!corrPos.empty())trials4++;
    else return false;

    for (unsigned char i : corrPos)
    {
        a.coef[i] = rand() % (1 << bits);
        if (print) cout << (int)i << " ";
    }

    bool success = rs.decode(a.coef, msg.coef, a.coef, k, nsym, erasures ? &erasePos : nullptr, print,detection4);

    for (int i = 0; i < k; i++)
    {
        if (msg.coef[i] != originalData[i])
        {
            if (unmatch)
            {
                *unmatch = true;
            }
            break;
        }
    }
    return success;
}
void testConfig(int k, int nsym, int bits)
{
	srand(time(0));
//    chrono::duration<double, std::milli> t11;
//    chrono::duration<double, std::milli> t13;

        for(int i=0;i<10000;i++){
			bool success1 = test_data_error(bits, k, nsym, 3, false, &fp1, 0);
            bool success2 = test_singlebit_error(bits, k, nsym, 3, false, &fp2, 0);
            bool success3 = test_multiplebit_error(bits, k, nsym, 3, false, &fp3, 0);
            bool success4 = test_burst_error(bits, k, nsym, 3, false, &fp4, 0);
			if (success1)
			{
				successes1++;
                if(fp1)nfp1++;
			}
            if (success2)
            {
                successes2++;
                if(fp2)nfp2++;

            }
            if (success3)
            {
                successes3++;
                if(fp3)nfp3++;

            }
            if (success4)
            {
                if(fp4)nfp4++;
                successes4++;
            }}

//         detection1+=successes1;
//         detection2+=successes2;
//         detection3+=successes3;
//         detection4+=successes4;
         cout<<"data_error_detection: "<<(double)detection1/(double)trials1<<"         data_error_correction: "<<(double)successes1/(double)detection1<<"        data_error_true_positive: "<<(double)(successes1-nfp1)/(double)detection1<<endl;
         cout<<"singlebit_error_detection: "<<(double)detection2/(double)trials2<<"         singlebit_error_correction: "<<(double)successes2/(double)detection2<<"        singlebit_error_true_positive: "<<(double)(successes2-nfp2)/(double)detection2<<endl;
         cout<<"multiplebit_error_detection: "<<(double)detection3/(double)trials3<<"        multiplebit_error_correction: "<<(double)successes3/(double)detection3<<"       multiplebit_error_true_positive: "<<(double)(successes3-nfp3)/(double)detection3<<endl;
         cout<<"burst_error_detection: "<<(double)detection4/(double)trials4<<"        burst_error_correction: "<<(double)successes4/(double)detection4<<"       bust_error_true_positive: "<<(double)(successes4-nfp4)/(double)detection4<<endl;
	}


int main()
{
	srand(time(NULL));
//    using std::chrono::high_resolution_clock;
//    using std::chrono::duration_cast;
//    using std::chrono::duration;
//    using std::chrono::milliseconds;
    testConfig(4, 6, 4);
}
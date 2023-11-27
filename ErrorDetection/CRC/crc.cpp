#include <bits/stdc++.h>
using namespace std;
const double prob = 0.25;


// Function to generate a random value between 0 and 1
double generateRandomValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

//Function to introduce error with probability p
void data_error(int encoded[], int sz){
    double bit_prob = 0.1;
    for(int i=1; i<=sz; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
            encoded[i] = 1 - encoded[i];
            bit_prob /= 2;
        }
    }
}

//Function to introduce error with probability p
void singlebit_error(int encoded[], int sz){
    double bit_prob = 0.25;
    for(int i=1; i<=sz; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
            encoded[i] = 1 - encoded[i];
            bit_prob /= 10;
        }
    }
}

//Function to introduce error with probability p
void multiplebit_error(int encoded[], int sz){
    double bit_prob = 0.1;
    for(int i=1; i<=sz; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
            encoded[i] = 1 - encoded[i];
        }
    }
}

//Function to introduce error with probability p
void burst_error(int encoded[], int sz){
    double bit_prob = 0.1;
    for(int i=1; i<=sz; i++){
        double rand_value = generateRandomValue();
        int burst_len[] = {2, 3, 4};
        if (rand_value <= bit_prob) {
            int len = rand()%3+2;
            for(int j=i; j<=min(sz, i+len); j++)
                encoded[j] = 1 - encoded[j];
        }
    }
}

string xor1(string a, string b)
{

	// Initialize result
	string result = "";

	int n = b.length();

	// Traverse all bits, if bits are
	// same, then XOR is 0, else 1
	for (int i = 1; i < n; i++) {
		if (a[i] == b[i])
			result += "0";
		else
			result += "1";
	}
	return result;
}

string mod2div(string dividend, string divisor)
{
	int pick = divisor.length();
	string tmp = dividend.substr(0, pick);

	int n = dividend.length();

	while (pick < n) {
		if (tmp[0] == '1')
			tmp = xor1(divisor, tmp) + dividend[pick];
		else
			tmp = xor1(std::string(pick, '0'), tmp)
				+ dividend[pick];
		pick += 1;
	}

	if (tmp[0] == '1')
		tmp = xor1(divisor, tmp);
	else
		tmp = xor1(std::string(pick, '0'), tmp);

	return tmp;
}



void encode(int code[], int data[],int num, string key)
{
    string data1="";
    for(int i=1;i<=num;i++){
        data1+=to_string(data[i]);
    }
    int l_key = key.length();
    string appended_data
            = (data1 + std::string(l_key - 1, '0'));

    string remainder = mod2div(appended_data, key);
    string codeword = data1 + remainder;
    for(int i=1;i<=codeword.size();i++){
        code[i]=int(codeword[i-1])-48;
    }
}

bool decode(int received[], int code[], int num,string key){
    string codeword="";
    for(int i=1;i<num+key.size();i++){
        codeword+=to_string(code[i]);
    }
    // cout<<codeword<<endl;
    string remainder=mod2div(codeword,key);
    int rem=stoi(remainder);
    if(rem==0)return false;
    else return true;

}
int main() {
    srand(time(NULL));
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    int trial;
    // int trial=1000;
    cout<<"Enter no of trials: ";
    cin>>trial;
    int ct1=0, ct2=0, ct3=0, ct4=0;
    int num;
    // int num=32;
    cout<<"Enter size of Data(in Binary): ";
    cin>>num;
    int s1=0, s2=0, s3=0, s4=0;
    string key;
    cout<<"Enter Polynomial: ";
    cin>>key;
    // key="1100000101";
    duration<double, std::milli> t11;
    duration<double, std::milli> t13;
    auto a =trial;

    while(trial--){
        int data[num+1];
        int r =key.size();
        int n = num+r;
        int code[n];
        int received[num+1];
        for(int i=1; i<=num; i++){
            double temp_r = generateRandomValue();
            if(temp_r<=0.5)
                data[i]=0;
            else data[i]=1;
        }
        auto t1 = high_resolution_clock::now();
        encode(code, data, num,key);
        auto t2 = high_resolution_clock::now();
        int copy[n];
        for(int i=1; i<n; i++){
            copy[i]=code[i];
        }

        //Data error
        data_error(code, n);
        bool is_error = decode(received, code, num,key);
        bool c=0;
        for(int i=1; i<n; i++){
            if(code[i]!=copy[i]){
                c=1;
                ct1++;
                break;
            }
        }
        if(is_error==c && c==1) s1++;

        //single bit error
        for(int i=1; i<n; i++){
            code[i]=copy[i];
        }
        singlebit_error(code, n);
        auto t3 = high_resolution_clock::now();
        is_error = decode(received, code, num,key);
        auto t4 = high_resolution_clock::now();
        c=0;
        for(int i=1; i<n; i++){
             if(code[i]!=copy[i]){
                c=1;
                ct2++;
                break;
            }
        }
        if(is_error==c && c==1) s2++;

        //multiple bit error
        for(int i=1; i<n; i++){
            code[i]=copy[i];
        }
        multiplebit_error(code, n);
        is_error = decode(received, code, num,key);
        c=0;
        for(int i=1; i<n; i++){
             if(code[i]!=copy[i]){
                c=1;
                ct3++;
                break;
            }
        }
        if(is_error==c && c==1) s3++;

        //burst error
        for(int i=1; i<n; i++){
            code[i]=copy[i];
        }
        burst_error(code, n);
        is_error = decode(received, code, num,key);
        c=0;
        for(int i=1; i<num+r; i++){
             if(code[i]!=copy[i]){
                c=1;
                ct4++;
                break;
            }
        }
        if(is_error==c && c==1) s4++;
        t11+=(t2-t1);
        t13+=(t4-t3);
    
        


    }
    t11/=a;
    t13/=a;
    cout<<"Encoding Time: ";
    std::cout << t11.count() << "ms\n";
    cout<<"Decoding Time: ";
    std::cout << t13.count() << "ms\n";
    cout<<"Data error efficiency: "<<(double)s1/ct1<<endl;
    cout<<"Single bit error efficiency: "<<(double)s2/ct2<<endl;
    cout<<"Multibit error efficiency: "<<(double)s3/ct3<<endl;
    cout<<"Burst error efficiency: "<<(double)s4/ct4<<endl;

    return 0;
}
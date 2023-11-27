#include <iostream>
#include <random>
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
void data_error(vector<vector<int>> &encoded, int sz, int k){
    double bit_prob = 0.25;
    for(int i=1; i<=sz; i++){
        for(int j=1; j<=k; j++){
            double rand_value = generateRandomValue();
            if (rand_value <= bit_prob) {
                encoded[i][j] = 1 - encoded[i][j];
                bit_prob /= 2;
            }
        }
    }
}

//Function to introduce error with probability p
void singlebit_error(vector<vector<int>> &encoded, int sz, int k){
    double bit_prob = 0.25;
    for(int i=1; i<=sz; i++){
        for(int j=1; j<=k; j++){
            double rand_value = generateRandomValue();
            if (rand_value <= bit_prob) {
                encoded[i][j] = 1 - encoded[i][j];
                bit_prob /= 10;
            }
        }
    }
}

//Function to introduce error with probability p
void multiplebit_error(vector<vector<int>> &encoded, int sz, int k){
    double bit_prob = 0.25;
    for(int i=1; i<=sz; i++){
        for(int j=1; j<=k; j++){
            double rand_value = generateRandomValue();
            if (rand_value <= bit_prob) {
                encoded[i][j] = 1 - encoded[i][j];
            }
        }
    }
}

//Function to introduce error with probability p
void burst_error(vector<vector<int>> &encoded, int sz, int k){
    //int a = (rand()%(sz))+1;
    double bit_prob = 0.1;
    for(int l=1; l<=sz; l++){
        for (int i = 1; i <= k; i++) {
            double rand_value = generateRandomValue();
            //int burst_len[] = {2, 3, 4};
            if (rand_value <= bit_prob) {
                int len = rand() % 3 + 2;
                for (int j = i; j <= min(k, j + len); j++)
                    encoded[l][j] = 1 - encoded[l][j];

            }
        }
    }
}

int check_parity(int n,int i,vector<int> &code)
{
    int p=0,k;
    for(int j=i;j<=n;j=k+i)
    {
        for(k=j;k<j+i && k<=n;k++)
        {
            if(code[k]==1)
                p++;
        }
    }
    if(p%2==0)
        return 0;
    else
        return 1;
}

int redundant_bits(int num){
    int r=0;
    while((r+num+1)>(pow(2,r)))
        r++;
    return r;
}

void encode(vector<int> &code, vector<int> &data, int num){
    int r = redundant_bits(num);
    int m=0,n,j=1,c;
    n=num+r;
    for(int i=1;i<=n;i++)
    {
        if(i==pow(2,m) && m<=r)
        {
            code[i]=0;
            m++;
        }
        else
        {
            code[i]=data[j];
            j++;
        }
    }

    m=0;
    for(int i=1;i<=n;i++)
    {
        if(i==pow(2,m) && m<=r)
        {
            c=check_parity(n,i,code);
            code[i]=c;
            m++;
        }
    }
}

void encode_all(vector<vector<int>> &code, vector<vector<int>> &data, int num, int k){
    int r = redundant_bits(num);
    for(int i=1; i<=k; i++){
        vector<int> tmp_data;
        vector<int> tmp_code(num+r+1);
        encode(tmp_code, data[i], num);
        for(int j=1; j<=num+r; j++){
            code[j][i]=tmp_code[j];
        }
    }
}

bool decode(vector<int> &received, vector<int> &code, int num){
    int m=0,n,j=0,c=0;
    int r = redundant_bits(num);
    n=num+r;
    for(int i=1;i<=n;i++)
    {
        if(i==pow(2,m) && m<=r)
        {
            c=c+(pow(2,j)*check_parity(n,i,code));
            j++;
            m++;
        }
    }
    for(int i=1;i<=n;i++)
    {
        if(i==pow(2,m) && m<=r)
        {
            //received[i]=0;
            m++;
        }
        else
        {
            j++;
        }
    }
//    cout<<"c: "<<c<<endl;
    if(code[c]==1)
        code[c]=0;
    else
        code[c]=1;
    m=0,j=1;
    for(int i=1;i<=n;i++)
    {
        if(i==pow(2,m) && m<=r)
        {
            //received[i]=0;
            m++;
        }
        else
        {
            received[j]=code[i];
            j++;
        }
    }
    if(c==0) return 0;
    return 1;


}

bool decode_all(vector<vector<int>> &received, vector<vector<int>> &code, int num, int k){
    int r = redundant_bits(num);
    bool ans=0;
    for(int i=1; i<=k; i++){
        vector<int> tmp_received(num+1);
        vector<int> tmp_code(num+r+1);
        for(int j=1; j<=num+r; j++){
            tmp_code[j]=code[j][i];
        }
        ans |= decode(tmp_received, tmp_code, num);
        for(int j=1; j<=num; j++){
            received[i][j]=tmp_received[j];
        }
    }
    return ans;
}





int main() {
    int trial;
    cin>>trial;
    //int ct=0;
    int num;
    cin>>num;
    //int success=0;
    int ct1=0, ct2=0, ct3=0, ct4=0;
    int s1=0, s2=0, s3=0, s4=0;
    while(trial--){
        int k=4;
        vector<vector<int>> data(k+1, vector<int> (num+1));
        int r = redundant_bits(num);
        int n = num+r;
        vector<vector<int>> code(num+r+1, vector<int> (k+1));
        vector<vector<int>> received(k+1, vector<int> (num+1));
        for(int i=1; i<=k; i++){
            for(int j=1; j<=num; j++){
                double temp_r = generateRandomValue();
                if (temp_r <= 0.5)
                    data[i][j] = 0;
                else data[i][j] = 1;
            }
        }
        encode_all(code, data, num, k);
        vector<vector<int>> copy(num+r+1, vector<int> (k+1));
        for(int i=1; i<=num+r; i++){
            for(int j=1; j<=k; j++){
                copy[i][j]=code[i][j];
            }
        }

        data_error(code, n, k);
        bool c=decode_all(received, code, num, k);

        if(c==1){
            ct1++;
            bool is_error=0;
            for(int i=1; i<=k; i++){
                for(int j=1; j<=num; j++){
                    if(data[i][j]!=received[i][j]){
                        is_error=1;
                        break;
                    }
                }
            }
            if(is_error==0) s1++;
        }


        for(int i=1; i<=num+r; i++){
            for(int j=1; j<=k; j++){
                code[i][j]=copy[i][j];
            }
        }

        singlebit_error(code, n, k);
        c=decode_all(received, code, num, k);

        if(c==1){
            ct2++;
            bool is_error=0;
            for(int i=1; i<=k; i++){
                for(int j=1; j<=num; j++){
                    if(data[i][j]!=received[i][j]){
                        is_error=1;
                        break;
                    }
                }
            }
            if(is_error==0) s2++;
        }

        for(int i=1; i<=num+r; i++){
            for(int j=1; j<=k; j++){
                code[i][j]=copy[i][j];
            }
        }

        multiplebit_error(code, n, k);
        c=decode_all(received, code, num, k);

        if(c==1){
            ct3++;
            bool is_error=0;
            for(int i=1; i<=k; i++){
                for(int j=1; j<=num; j++){
                    if(data[i][j]!=received[i][j]){
                        is_error=1;
                        break;
                    }
                }
            }
            if(is_error==0) s3++;
        }

        for(int i=1; i<=num+r; i++){
            for(int j=1; j<=k; j++){
                code[i][j]=copy[i][j];
            }
        }

        burst_error(code, n, k);
        c=decode_all(received, code, num, k);

        if(c==1){
            ct4++;
            bool is_error=0;
            for(int i=1; i<=k; i++){
                for(int j=1; j<=num; j++){
                    if(data[i][j]!=received[i][j]){
                        is_error=1;
                        break;
                    }
                }
            }
            if(is_error==0) s4++;
        }

    }
    cout<<(double)s1/ct1<<endl;
    cout<<(double)s2/ct2<<endl;
    cout<<(double)s3/ct3<<endl;
    cout<<(double)s4/ct4<<endl;
    return 0;
}
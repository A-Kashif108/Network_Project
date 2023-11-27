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
    double bit_prob = 0.25;
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
            bit_prob /= 1000;
        }
    }
}

//Function to introduce error with probability p
void multiplebit_error(int encoded[], int sz){
    double bit_prob = 0.25;
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

int check_parity(int n,int i,int code[])
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

void encode(int code[], int data[], int num){
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

bool decode(int received[], int code[], int num){
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
    else return 1;

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
        int data[num+1];
        int r = redundant_bits(num);
        int n = num+r;
        int code[num+r+1];
        int received[num+1];
        int received_without_corr[num+1];
        for(int i=1; i<=num; i++){
            double temp_r = generateRandomValue();
            if(temp_r<=0.5)
                data[i]=0;
            else data[i]=1;
        }
        encode(code, data, num);
        int copy[num+r+1];
        for(int i=1; i<=num+r; i++){
            copy[i]=code[i];
        }

        data_error(code, n);
        bool c = decode(received, code, num);

        if(c==1){
            ct1++;
            bool is_error=0;
            for(int i=1; i<=num; i++){
                if(data[i]!=received[i]){
                    is_error=1;
                    break;
                }
            }
            if(is_error==0) s1++;
        }


        for(int i=1; i<=num+r; i++){
            code[i]=copy[i];
        }

        singlebit_error(code, n);
        c=decode(received, code, num);
        if(c==1){
            ct2++;
            bool is_error=0;
            for(int i=1; i<=num; i++){
                if(data[i]!=received[i]){
                    is_error=1;
                    break;
                }
            }
            if(is_error==0) s2++;
        }

        for(int i=1; i<=num+r; i++){
            code[i]=copy[i];
        }

        multiplebit_error(code, n);
        c=decode(received, code, num);

        if(c==1){
            ct3++;
            bool is_error=0;
            for(int i=1; i<=num; i++){
                if(data[i]!=received[i]){
                    is_error=1;
                    break;
                }
            }
            if(is_error==0) s3++;
        }

        for(int i=1; i<=num+r; i++){
            code[i]=copy[i];
        }

        burst_error(code, n);
        c=decode(received, code, num);

        if(c==1){
            ct4++;
            bool is_error=0;
            for(int i=1; i<=num; i++){
                if(data[i]!=received[i]){
                    is_error=1;
                    break;
                }
            }
            if(is_error==0) s4++;
        }

    }
    cout<<"Data error efficiency: "<<(double)s1/ct1<<endl;
    cout<<"Single bit error efficiency: "<<(double)s2/ct2<<endl;
    cout<<"Multibit error efficiency: "<<(double)s3/ct3<<endl;
    cout<<"Burst error efficiency: "<<(double)s4/ct4<<endl;
    return 0;
}
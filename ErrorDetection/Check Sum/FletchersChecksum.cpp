#include <bits/stdc++.h>
using namespace std;
const double prob = 0.25;



double generateRandomValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}


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
            bit_prob /= 50;
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

string decimalTo16BitBinary(int decimalNum) {

    
    std::bitset<16> binaryRepresentation(decimalNum);
    string binaryString = binaryRepresentation.to_string();

    return binaryString;
}
void encode(int code[], int data[], int num){
    int sum1=0;
    int sum2=0;
    for (int i=1;i<=num;) {
        int bytedata=0;
        int x=128;
        for(int j=0;j<8;j++){
            bytedata +=data[i++]*x;
            x/=2;
        }
        // cout<<bytedata<<endl;
        sum1 = (sum1 + bytedata) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    // cout<<sum1<<" "<<sum2<<" ";
    int checksum=(sum2 << 8) | sum1;
    // cout<<checksum<<endl;
    string checksum16bit=decimalTo16BitBinary(checksum);
    // cout<<checksum16bit<<endl;
    for(int i=0;i<=num;i++){
        code[i]=data[i];
    }
    for(int i=0;i<16;i++){
        if(checksum16bit[i]=='0')code[num+i+1]=0;
        else code[num+i+1]=1;
    }
}


bool decode(int received[], int code[], int num){
    int sum1=0;
    int sum2=0;
    for (int i=1;i<=num;) {
        int bytedata=0;
        int x=128;
        for(int j=0;j<8;j++){
            bytedata+=code[i++]*x;
            x/=2;
        }
        sum1 = (sum1 + bytedata) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    // cout<<sum1<<" "<<sum2<<" ";
    int checksum=(sum2 << 8) | sum1;
    // cout<<checksum<<endl;
    string checksum16bit=decimalTo16BitBinary(checksum);
    // cout<<checksum16bit<<endl;
    for(int i=0;i<16;i++){
        if((checksum16bit[i]=='0' && code[i+num+1]!=0) || 
        (checksum16bit[i]=='1' && code[i+num+1]!=1))return true;
    }
    return false;
}



int main() {
    int trial=10000;
    // cin>>trial;
    int ct1=0, ct2=0, ct3=0, ct4=0;
    int num=80;
    cin>>num;
    int s1=0, s2=0, s3=0, s4=0;


    while(trial--){
        int data[num+1];
        int r =16;
        int n = num+r;
        int code[num+r+1];
        int received[num+1];
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

        //Data error
        data_error(code, n);
        bool is_error = decode(received, code, num);
        bool c=0;
        for(int i=1; i<=n; i++){
            if(copy[i]!=code[i]){
                c=1;
                ct1++;
                break;
            }
        }
        if(is_error==c && c==1) s1++;

        //single bit error
        for(int i=1; i<=num+r; i++){
            code[i]=copy[i];
        }
        singlebit_error(code, n);
        is_error = decode(received, code, num);
        c=0;
        for(int i=1; i<=num; i++){
            if(copy[i]!=code[i]){
                c=1;
                ct2++;
                break;
            }
        }
        if(is_error==c && c==1) s2++;

        //multiple bit error
        for(int i=1; i<=num+r; i++){
            code[i]=copy[i];
        }
        multiplebit_error(code, n);
        is_error = decode(received, code, num);
        c=0;
        for(int i=1; i<=num; i++){
            if(copy[i]!=code[i]){
                c=1;
                ct3++;
                break;
            }
        }
        if(is_error==c && c==1) s3++;

        //burst error
        for(int i=1; i<=num+r; i++){
            code[i]=copy[i];
        }
        burst_error(code, n);
        is_error = decode(received, code, num);
        c=0;
        for(int i=1; i<=num; i++){
            if(copy[i]!=code[i]){
                c=1;
                ct4++;
                break;
            }
        }
        if(is_error==c && c==1) s4++;


    }
//    cout<<s1<<" "<<ct1<<endl;
    cout<<(double)s1/ct1<<endl;
    cout<<(double)s2/ct2<<endl;
    cout<<(double)s3/ct3<<endl;
    cout<<(double)s4/ct4<<endl;

    return 0;
}
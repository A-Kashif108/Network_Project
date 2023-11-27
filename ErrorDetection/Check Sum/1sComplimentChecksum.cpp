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





void encode(int code[], int data[], int num){
    for(int i=1;i<=num;i++){
        code[i]=data[i];
    }
    int block_size=8;
    int result[]={0,0,0,0,0,0,0,0};
    for (int i = 1; i <= num; i += block_size) {

        // Stores the data of the next block
        int next_block[8];
        int ct=0;
        for (int j = i; j < i + block_size; j++) {
            next_block[ct++] = data[j];
        }


        string additions = "";
        int sum = 0, carry = 0;

        for (int k = block_size - 1; k >= 0; k--) {
            sum += (next_block[k])
                   + (result[k]);
            carry = sum / 2;
            if (sum == 0) {
                additions = '0' + additions;
                sum = carry;
            }
            else if (sum == 1) {
                additions = '1' + additions;
                sum = carry;
            }
            else if (sum == 2) {
                additions = '0' + additions;
                sum = carry;
            }
            else {
                additions = '1' + additions;
                sum = carry;
            }
        }

        // After binary add of two blocks with carry,
        // if carry is 1 then apply binary addition
        string final = "";

        if (carry == 1) {
            for (int l = additions.length() - 1; l >= 0;
                 l--) {
                if (carry == 0) {
                    final = additions[l] + final;
                }
                else if (((additions[l] - '0') + carry) % 2
                         == 0) {
                    final = "0" + final;
                    carry = 1;
                }
                else {
                    final = "1" + final;
                    carry = 0;
                }
            }


            for(int i=0;i<8;i++){
                result[i]=final[i]-'0';
            }
        }
        else {
            for(int i=0;i<8;i++){
                result[i]=additions[i]-'0';
            }
        }
    }

    for(int i=0;i<8;i++) {
        code[num+i+1]=1-result[i];
    }
}

bool decode(int received[], int code[], int num){
    int block_size=8;
    int result[]={0,0,0,0,0,0,0,0};
    for (int i = 1; i <= num+block_size; i += block_size) {


        int next_block[8];
        int ct=0;
        for (int j = i; j < i + block_size; j++) {
            next_block[ct++] = code[j];
        }

        string additions = "";
        int sum = 0, carry = 0;

        for (int k = block_size - 1; k >= 0; k--) {
            sum += (next_block[k])
                   + (result[k]);
            carry = sum / 2;
            if (sum == 0) {
                additions = '0' + additions;
                sum = carry;
            }
            else if (sum == 1) {
                additions = '1' + additions;
                sum = carry;
            }
            else if (sum == 2) {
                additions = '0' + additions;
                sum = carry;
            }
            else {
                additions = '1' + additions;
                sum = carry;
            }
        }

        // After binary add of two blocks with carry,
        // if carry is 1 then apply binary addition
        string final = "";

        if (carry == 1) {
            for (int l = additions.length() - 1; l >= 0;
                 l--) {
                if (carry == 0) {
                    final = additions[l] + final;
                }
                else if (((additions[l] - '0') + carry) % 2
                         == 0) {
                    final = "0" + final;
                    carry = 1;
                }
                else {
                    final = "1" + final;
                    carry = 0;
                }
            }


            for(int i=0;i<8;i++){
                result[i]=final[i]-'0';
            }
        }
        else {
            for(int i=0;i<8;i++){
                result[i]=additions[i]-'0';
            }
        }
    }

    for(int i=1; i<=num; i++){
        received[i]=code[i];
    }
    for(int i=0;i<8;i++){
        if(result[i]==0)return true;
    }
    return false;
}
int main() {
   
    int trial=1000;
    // cin>>trial;
    int ct1=0, ct2=0, ct3=0, ct4=0;
    int num=80;
    cin>>num;
    int s1=0, s2=0, s3=0, s4=0;


    while(trial--){
        int data[num+1];
        int r =8;
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
        for(int i=1; i<=num+8; i++){
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
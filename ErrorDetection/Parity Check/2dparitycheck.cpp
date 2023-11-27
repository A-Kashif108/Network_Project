#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>
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
void data_error(vector<int>&encoded, int sz){
    double bit_prob = 0.5;
    for(int i=0; i<sz; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
            encoded[i] = 1 - encoded[i];
            bit_prob /= 2;
        }
    }
}

//Function to introduce error with probability p
void singlebit_error(vector<int>&encoded, int sz){
    double bit_prob = 0.25;
    for(int i=0; i<sz; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
            encoded[i] = 1 - encoded[i];
            bit_prob /= 1000.0;
          // return;
        }
    }
}

//Function to introduce error with probability p
void multiplebit_error(vector<int>&encoded, int sz){
    double bit_prob = 0.25;
    for(int i=0; i<sz; i++){
        double rand_value = generateRandomValue();
        if (rand_value <= bit_prob) {
            encoded[i] = 1 - encoded[i];
        }
    }
}

//Function to introduce error with probability p
void burst_error(vector<int>&encoded, int sz){
    double bit_prob = 0.05;
    for(int i=0; i<sz; i++){
        double rand_value = generateRandomValue();
        int burst_len[] = {2, 3, 4};
        if (rand_value <= bit_prob) {
            int len = rand()%3+2;
            for(int j=i; j<min(sz, i+len); j++)
                encoded[j] = 1 - encoded[j];
        }
    }
}

void ENCODE_DATA(vector<vector<int>>&data) {
    int n_col = data[0].size();
    int n_row=data.size();
    //col
    vector<int>temp;

    for (size_t i = 0; i < n_col; i ++) {
        int parityBit =0;

        for (int j = 0; j < n_row; ++j)
        {
            parityBit+=data[j][i];
        }
        temp.push_back(parityBit%2);
    }
    data.push_back(temp);
    //row
    n_row=data.size();
    for (size_t i = 0; i < n_row; i ++) {
        int parityBit =0;
        for(auto bit:data[i]){
            parityBit+=bit;
        }
        data[i].push_back(parityBit%2);
    }


    return;
}

vector<int> DecodeRow(vector<vector<int>>&data){
    int n_row=data.size();
    int n_col = data[0].size();
    vector<int>row;
    for (size_t i = 0; i < n_row; i ++) {
        int parityBit =0;
        for(auto bit:data[i]){
            parityBit+=bit;
        }
        if(parityBit%2){
            row.push_back(i+1);
        }
    }
    return row;
}
vector<int> DecodeCol(vector<vector<int>>&data){
    int n_row=data.size();
    int n_col = data[0].size();
    vector<int>col;
    for (size_t i = 0; i < n_col-1; i ++) {
        int parityBit =0;

        for (int j = 0; j < n_row; ++j)
        {
            parityBit+=data[j][i];
        }
        if(parityBit%2){
            col.push_back(i+1);
        }
    }
    return col;
}

void print(vector<vector<int>>&v){
    for(auto row:v){
        for(auto col:row){
            cout<<col;
        }
        cout<<endl;
    }
    cout<<endl;
}
vector<int> change_vv_v(vector<vector<int>>&v){
    vector<int>temp;
    for (int i = 0; i < v.size(); ++i)
    {
        for (int j = 0; j < v[0].size(); ++j)
        {
            temp.push_back(v[i][j]);
        }
    }
    return temp;
}
vector<vector<int>> change_v_vv(vector<int>&v,int k){
    vector<vector<int>>temp;
    for (int i = 0; i < v.size(); i+=k)
    {
        vector<int>tt;
        for (int j = 0; j < k; ++j)
        {
            tt.push_back(v[i+j]);
        }
        temp.push_back(tt);
    }
    return temp;
}

bool DECODE_DATA(vector<vector<int>>&v){
    vector<int>row_error=DecodeRow(v);
    vector<int>col_error=DecodeCol(v);

    for(auto i : row_error){
        for(auto j : col_error){
            v[i-1][j-1]^=1;
        }
    }

    if(row_error.size()>0||col_error.size()>0){
        return true;
    }
    return false;
}

bool is_error_corrected(vector<vector<int>>&v1,vector<vector<int>>&v2){
    for (int i = 0; i < v1.size(); ++i)
    {
        for (int j = 0; j < v1[0].size(); ++j)
        {
            if(v1[i][j]!=v2[i][j])return false;
        }
    }
    return true;
}
int main() {
    srand(time(NULL));
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    srand(static_cast<unsigned int>(time(0)));
    int test;cin>>test;
    int dataerror=0,singleerror=0,multipleerror=0,bursterror=0;
    int detecteddataerror=0,detectedsingleerror=0,
            detectedmultipleerror=0,detectedbursterror=0;
    int correcteddataerror=0,correctedsingleerror=0,
            correctedmultipleerror=0,correctedbursterror=0;
    int n, k;
    cin>>n>>k;

    duration<double, std::milli> t11;
    duration<double, std::milli> t13;
    auto a =test;
    while(test--){

        vector<int>data;
        for(int i=0; i< n; i++){
            double temp_r = generateRandomValue();
            if(temp_r<=0.5)
                data.push_back(0);
            else data.push_back(1);
        }
        vector<vector<int>>v,original;
        for (int i = 0; i < data.size(); i+=k)
        {
            vector<int>temp;
            for (int j = 0; j < k; ++j)
            {
                temp.push_back(data[i+j]);
            }
            v.push_back(temp);
            original.push_back(temp);
        }
        auto t1 = high_resolution_clock::now();
        ENCODE_DATA(v);
        auto t2= high_resolution_clock::now();
        vector<int>v1,v2,v3,v4;
        vector<int>initial=change_vv_v(v);
        v1=v2=v3=v4=initial;

        int n = initial.size();
        data_error(v1,n);
        singlebit_error(v2,n);
        multiplebit_error(v3,n);
        burst_error(v4,n);

        bool is_error,is_corrected;
        k++;

        v = change_v_vv(v1,k);
        is_error = DECODE_DATA(v);
        is_corrected=is_error_corrected(original,v);
        if(v1!=initial){
            dataerror++;
            if(is_error){
                detecteddataerror++;
                if(is_corrected)correcteddataerror++;
            }
        }

        v = change_v_vv(v2,k);
        auto t3 = high_resolution_clock::now();
        is_error = DECODE_DATA(v);
        auto t4 = high_resolution_clock::now();
        is_corrected=is_error_corrected(original,v);
        if(v2!=initial){
            singleerror++;
            if(is_error){
                detectedsingleerror++;
                if(is_corrected)correctedsingleerror++;
            }
        }

        v = change_v_vv(v3,k);
        is_error = DECODE_DATA(v);
        is_corrected=is_error_corrected(original,v);
        if(v3!=initial){
            multipleerror++;
            if(is_error){
                detectedmultipleerror++;
                if(is_corrected)correctedmultipleerror++;
            }
        }

        v = change_v_vv(v4,k);
        is_error = DECODE_DATA(v);
        is_corrected=is_error_corrected(original,v);
        if(v4!=initial){
            bursterror++;
            if(is_error){
                detectedbursterror++;
                if(is_corrected)correctedbursterror++;
            }
        }
        k--;
        t11+=(t2-t1);
        t13+=(t4-t3);

    }
    t11/=a;
    t13/=a;
    cout<<"Encoding Time: ";
    std::cout << t11.count() << "ms\n";
    cout<<"Decoding Time: ";
    std::cout << t13.count() << "ms\n";
    if(dataerror&&detecteddataerror)
        cout<<(double)detecteddataerror/dataerror<<" "<<(double)correcteddataerror/detecteddataerror<<endl;
    if(singleerror&&detectedsingleerror)
        cout<<(double)detectedsingleerror/singleerror<<" "<<(double)correctedsingleerror/detectedsingleerror<<endl;
    if(multipleerror&&detectedmultipleerror)
        cout<<(double)detectedmultipleerror/multipleerror<<" "<<(double)correctedmultipleerror/detectedmultipleerror<<endl;
    if(bursterror&&detectedbursterror)
        cout<<(double)detectedbursterror/bursterror<<" "<<(double)correctedbursterror/detectedbursterror<<endl;

}
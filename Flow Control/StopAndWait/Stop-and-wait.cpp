#include <bits/stdc++.h>
#include <windows.h>
#include <time.h>
#include <cstdlib>
using namespace std;
typedef long long int ll;



ll t=0;
bool cansend=true;
bool reqtosend=false;
bool ackrec=false;
bool datarec=false;
bool nakrec=false;

struct frame{
    string data;
    ll sequence_number;
    bool corrupt=false;
    void turn(){
        corrupt=!corrupt;
    }
};

struct ack{
    ll ack_number;
    bool corrupt=false;
    void turn(){
        corrupt=!corrupt;
    }
};

struct nack{
    ll ack_number;
    bool corrupt=false;
    void turn(){
        corrupt=!corrupt;
    }
};

ll rew=0;
ll rn=0;
ll sn=0;
ll visi[100000]={0};
ll visit[100000]={0};
ll e=0;
nack naks[100000];
ll recieved_times[100000];
ll push[100001]={0};



ll func_rand(){
    
    return (rand()%rew);
}

void sender(vector<bool> &vis,vector<frame> &copy,vector<ack> &acks,vector<ll> &times,vector<ll> time){ 
    if(cansend==true){
        string dat;
        cin>>dat;
        frame fm;
        fm.sequence_number=sn;
        fm.data=dat;
        cout<<"data with seqno: "<<sn<<" is sent at "<<t<<endl;
        copy[sn]=fm;
        push[sn]++;
        sn++;
        cansend=false;
        datarec=true;
        times[sn-1]++;
        t++;
        if(e==0){
            cout<<"Data sent with seqno: "<<sn-1<<" is crashed"<<endl;
            datarec=false;
            ackrec=false;
        }
        e=1-e;
    }
    else if(ackrec==true){
        cout<<"data with ackno: "<<acks[rn].ack_number<<" is recieved at "<<t<<endl;
        
        ll o=func_rand();
        if(o!=0){
            acks[rn].turn();
        }
        if(acks[rn].corrupt==false && acks[rn].ack_number==sn){
            cout<<"data with ackno: "<<acks[rn].ack_number<<" is recieved properly "<<t<<endl;
            rn=rn+1;
            cansend=true;
            vis[sn-1]=true;
            ackrec=false;
            recieved_times[sn-1]=t;
        }
        else{
            cout<<"data with ackno: "<<acks[rn].ack_number<<" recieved is corrupted "<<t<<endl;
            ackrec=false;
            times[sn-1]++;
        }
        t++;
    }
    else if(nakrec==true){
        cout<<"data with ackno: "<<acks[rn].ack_number<<" is recieved at "<<t<<endl;
        ll o=func_rand();
        if(o!=0){
            naks[rn].turn();
        }
        if(naks[rn].corrupt==false && naks[rn].ack_number==(sn-1)){
            cout<<"data with nackno: "<<naks[rn].ack_number<<" is recieved properly "<<t<<endl;
            vis[sn-1]=false;
            nakrec=false;
            times[sn-1]=0;
            datarec=true;
            cout<<"frame with seq no: "<<sn-1<<" is again sent at "<<t<<endl;
            push[sn-1]++;
            times[sn-1]++;
        }
        else{
            cout<<"data with ackno: "<<naks[rn].ack_number<<" recieved is corrupted "<<t<<endl;
            nakrec=false;
            times[sn-1]++;
        }
        t++;
    }
    else if(times[sn-1]>time[sn-1]){
        times[sn-1]=0;
        datarec=true;
        cout<<"frame with seq no: "<<sn-1<<" is again sent at "<<t<<endl;
        push[sn-1]++;
        times[sn-1]++;
        t++;
    }
    else{
        cout<<"at t="<<t<<" ........................."<<endl;
        t++;
        times[sn-1]++;
    }
    
}
ll y=0;
ll y2=0;
void reciever(vector<frame> &recieved_frames,vector<ack> &acks,frame &fm,vector<ll> &times,vector<ll> time){
    if(datarec==true){
        cout<<"data with seqno: "<<fm.sequence_number<<" is recieved at "<<t<<endl;

        ll o=func_rand();
        if(o!=0){
            fm.turn();
        }
        if(fm.corrupt==false){
            cout<<"data with seqno: "<<fm.sequence_number<<" is not corrupted at "<<t<<endl;
            if(fm.sequence_number==rn){
                recieved_frames[rn]=fm;
            }
            if(visi[rn]==0){
            ack ak;
            ak.ack_number=rn+1;
            acks[rn]=ak;
            ackrec=true;
            visi[rn]=1;
            }
            datarec=false;
            times[fm.sequence_number]++;
            cout<<"data with ackno: "<<rn+1<<" is sent at "<<t<<endl;
            ackrec=true;
            if(y==0){
                cout<<"Data sent with ackno: "<<rn+1<<" is crashed"<<endl;
                ackrec=false; 
            }
            else{
                ackrec=true;
            }
            y=1-y;
        }
        else{ 
            cout<<"data with seqno: "<<fm.sequence_number<<" is corrupted at "<<t<<endl;
            if(visit[rn]==0){
            nack ak;
            ak.ack_number=rn;
            naks[rn]=ak;
            nakrec=true;
            visit[rn]=1;
            }
            ackrec=false;
            datarec=false;
            times[fm.sequence_number]++;
            cout<<"Nak with seqno: "<<fm.sequence_number<<" is sent at "<<t<<endl;
            y2=func_rand();
            if(y2!=0){
                cout<<"Data sent with nackno: "<<rn<<" is crashed"<<endl;
                nakrec=false; 
            }
            else{
                nakrec=true;
            }
        }
        t++;
    }
}

int main(){
    srand((unsigned)time(NULL));
    ll n;
    cin>>n;
    cin>>rew;
    vector<frame> copy(n);
    vector<frame> recieved_frames(n);
    vector<ack> acks(n);
    vector<ll> time(n);
    vector<bool> vis(n);
    for(ll i=0;i<n;i++){
        cin>>time[i];
        vis[i]=false;
    }
    vector<ll> times(n,0);
    ll ede=8;
    while(vis[n-1]==false){
        sender(vis,copy,acks,times,time);
        reciever(recieved_frames,acks,copy[sn-1],times,time);
    }
    // cout<<"recieved time of zero is "<<recieved_times[0]<<endl; 
    //Lets fix tp=1;
    // double efficency=(double)(0.1)*(double)push[0]/(double)(recieved_times[0]);
    // cout<<"efficency for first packet is "<<efficency<<endl;
    ll pushing=0;
    for(ll i=0;i<n;i++){
        pushing+=push[i];
    }
    // cout<<t<<" "<<pushing<<endl;
    cout<<"Total effiency is "<<(double)(((double)pushing*(0.10))/((double)((double)pushing*0.10+t)))<<endl;
    return 0;
}
#include <iostream>
#include <bits/stdc++.h>
#include <random>
using namespace std;
#define max(a, b) (a < b ? b : a)
#define min(a, b) (a < b ? a : b)
#define abs(x) (x < 0 ? (-x) : x)
#define IN(i, l, r) (l < i && i < r)
#define FOR(i, L, R) for (int i = L; i < R; i++)
#define FORE(i, L, R) for (int i = L; i <= R; i++)
#define FFOR(i, L, R) for (int i = L; i > R; i--)
#define FFORE(i, L, R) for (int i = L; i >= R; i--)
#define ll long long

double trans_time;
double prop_time;
double timeOut_time;
int w;
double total_time;
int no_of_packet;
int th;
int pktLoss;
int ackLoss;
int lostPkt=0;
vector<double> sentTime, recvTime;

bool checktr()
{
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(1, 100);

    int r = dis(gen);
    r = r % 100;
    cout << "random " << r << " ";
    if (r >= pktLoss)
        return true;
    return false;
}

void send_window(int base)
{
    int nxtWin = base;

    for (int i = base + 1; i < base + w && i <= no_of_packet; i++)
    {
        sentTime[i] = sentTime[i - 1] + trans_time;
    }

    for (int i = base; i < base + w && i <= no_of_packet; i++)
    {
        int pr = rand() % 100;
        if (pr >= pktLoss)
        {
            int rr = rand() % 100;
            if (rr >= ackLoss)
            {
                int z = rand() % 20;
                recvTime[i] = sentTime[i] + 2 * prop_time + trans_time ;
            }
            else
            {
                recvTime[i] = -1;
            }
        }
        else
        {
            lostPkt++;
            recvTime[i] = -2;
        }
    }

    for (int i = base; i < base + w && i <= no_of_packet; i++)
    {
        if (recvTime[i] == -2)
        {
            nxtWin = i;
            sentTime[i] = sentTime[i] + timeOut_time;
            double l = sentTime[i] + timeOut_time;
            for (int j = i + 1; j < base + w && j <= no_of_packet; j++)
            {
                if (recvTime[j] >= 0)
                {
                    l = min(l, recvTime[j]);
                }
            }
            sentTime[i] = min(sentTime[i], l);
            break;
        }

        else if (recvTime[i] == -1)
        {
            bool loss = true;
            double l = sentTime[i] + timeOut_time;
            for (int j = i + 1; j < base + w && j <= no_of_packet; j++)
            {
                if (recvTime[j] >= 0)
                {
                    if (recvTime[j] <= l)
                    {
                        loss = false;
                        l = min(recvTime[j], l);
                    }
                }
            }
            if (loss)
            {
                nxtWin = i;
                sentTime[i] = sentTime[i] + timeOut_time;
            }
            else
            {
                nxtWin++;
                if (base + w <= no_of_packet)
                {
                    if (sentTime[base + w] == 0 || sentTime[base + w] > l)
                    {
                        sentTime[base + w] = l;
                    }
                }
            }
        }

        else
        {
            bool loss = false;
            double l = sentTime[i] + timeOut_time;
            if (l < recvTime[i])
            {
                loss = true;
            }

            for (int j = i + 1; j < base + w && j <= no_of_packet; j++)
            {
                if (recvTime[j] >= 0)
                {
                    if (recvTime[j] <= l)
                    {
                        loss = false;
                        l = min(recvTime[j], l);
                    }
                }
            }

            if (loss)
            {
                nxtWin = i;
                sentTime[i] = sentTime[i] + timeOut_time;
            }
            else
            {
                nxtWin++;
                if (base + w <= no_of_packet)
                {
                    if (sentTime[base + w] == 0 || sentTime[base + w] > l)
                    {
                        sentTime[base + w] = l;
                    }
                }
            }
        }
    }

    if (nxtWin <= no_of_packet)
    {
        send_window(nxtWin);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif

    double pktSize, bandwidth;
    cin >> w;
    cin >> no_of_packet;
    cin >> pktSize;
    cin >> bandwidth;
    cin >> prop_time;
    cin >> timeOut_time;
    cin >> pktLoss;
    cin >> ackLoss;

    trans_time = (pktSize*1000 / bandwidth) ;

    sentTime.resize(no_of_packet + 1, 0);
    recvTime.resize(no_of_packet + 1, 0);

    cout << trans_time;
    send_window(1);

    // for (int i = 0; i <= no_of_packet; i++)
    // {
    //     cout << i << " - " << sentTime[i] << " " << recvTime[i] << endl;
    // }

    cout<<trans_time<<"  "<<prop_time<<endl;
    total_time=recvTime[no_of_packet];
    
    double ideal_eff= (w*trans_time)/(trans_time+2*prop_time);

    double band_used=(pktSize*no_of_packet*1000)/(total_time);

    double real_eff=(pktSize*no_of_packet*1000)/(total_time*bandwidth);

    // cout<<"no of lost pkt"<<lostPkt<<endl;

    cout<<" ideal eff " << ideal_eff<<endl;
    cout<<" real eff " << real_eff<<endl;

    return 0;
}
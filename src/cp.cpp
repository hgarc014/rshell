#include <iostream>
using namespace std;

#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
// extern "C" int gettimeofday(timeval *tp, void *tzp);
// // extern "C" int getrusage(int who, struct rusage *rusag);
class Timer
{

    public:
        int start();
        int elapsedWallclockTime (double &);
        int elapsedUserTime (double &);
        int elapsedSystemTime (double &);
        int elapsedTime (double &wc, double &us, double &system);
    private:
        rusage old_us_time;
        rusage new_us_time;
        timeval old_wc_time;
        timeval new_wc_time;
};
    int
Timer::start()
{

    if (gettimeofday (&this->old_wc_time, 0) == -1
            || getrusage (RUSAGE_SELF, &this->old_us_time) == -1)
        return -1;
    else
        return 0;
}
    int
Timer::elapsedWallclockTime (double &wc)
{

    if (gettimeofday (&this->new_wc_time, 0) == -1)
        return -1;
    wc = (this->new_wc_time.tv_sec - this->old_wc_time.tv_sec)
        + (this->new_wc_time.tv_usec - this->old_wc_time.tv_usec) / 1000000.0;
    return 0;
}
    int
Timer::elapsedUserTime (double &ut)
{

    if (getrusage (RUSAGE_SELF, &this->new_us_time) == -1)
        return -1;
    ut = (this->new_us_time.ru_utime.tv_sec - this->old_us_time.ru_utime.tv_sec)
        + ((this->new_us_time.ru_utime.tv_usec
                    - this->old_us_time.ru_utime.tv_usec) / 1000000.0);
    return 0;
}
    int
Timer::elapsedSystemTime (double &st)
{

    if (getrusage (RUSAGE_SELF, &this->new_us_time) == -1)
        return -1;
    st = (this->new_us_time.ru_stime.tv_sec - this->old_us_time.ru_stime.tv_sec)
        + ((this->new_us_time.ru_stime.tv_usec
                    - this->old_us_time.ru_stime.tv_usec) / 1000000.0);
    return 0;
}
    int
Timer::elapsedTime (double &wallclock, double &user_time, double &system_time)
{

    if (this->elapsedWallclockTime (wallclock) == -1)
        return -1;
    else
    {

        if (getrusage (RUSAGE_SELF, &this->new_us_time) == -1)
            return -1;
        user_time = (this->new_us_time.ru_utime.tv_sec
                - this->old_us_time.ru_utime.tv_sec)
            + ((this->new_us_time.ru_utime.tv_usec
                        - this->old_us_time.ru_utime.tv_usec) / 1000000.0);
        system_time = (this->new_us_time.ru_stime.tv_sec
                - this->old_us_time.ru_stime.tv_sec)
            + ((this->new_us_time.ru_stime.tv_usec
                        - this->old_us_time.ru_stime.tv_usec) / 1000000.0);
        return 0;
    }
}

#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void systemCall(char *in, char *out, int sz);
void printTimeStart(Timer &t, const string check);
void checkExist(char* in);

int main(int argc, char *argv[]){


    if(argc < 3){
        cout << "invalid use: cp <FLAG> <inputfile> <outputfile>" << endl;
        exit(1);
    }
    else if (argc > 4)
    {
        cout << "only 4 arguements\ncp <FLAG> <inputfile> <outputfile>" << endl;
        exit(1);
    }
    bool showTime = false,
         first = true;
    char *in;
    char *out;
    if(argc == 3)
    {
        in = argv[1];
        out=argv[2];
        checkExist(out);
        systemCall(in,out,BUFSIZ);
    }
    else
    {
        for(int i =1; i < argc ; ++i)
        {
            if(argv[i][0] == '-' && !showTime)
            {
                showTime = true;
            }
            else if(first)
            {
                first = false;
                in = argv[i];
            }
            else
            {
                out = argv[i];
            }
        }
        if(!showTime)
        {
            cout << "NO FLAG PASSED IN" << endl;
            exit(1);
        }
        checkExist(out);

        Timer t;
        t.start();

        systemCall(in,out,1);
        printTimeStart(t,"read BUF=1");

        ifstream fin(in);
        ofstream fout(out);

        while(fin.good()){
            char c = fin.get();
            if(fin.good())
                fout.put(c);
        }
        fout.close();
        fin.close();
        printTimeStart(t,"fstream");


        systemCall(in,out,BUFSIZ);
        printTimeStart(t,"read BUF=BUFSIZ");
    }
    return 0;
}

void printTimeStart(Timer &t, const string check)
{
    double wTime,
           uTime,
           sTime;
    t.elapsedTime(wTime,uTime,sTime);
    cout << endl << check << endl;
    cout << "UserTime=" << uTime << endl
        << "SystemTime=" << sTime << endl
        << "WallClock=" << wTime << endl;
    t.start();
}


void systemCall(char *in, char *out, int sz){
    int fdi = open(in, O_RDONLY);
    int fdo = open(out, O_WRONLY | O_CREAT);
    char buf[BUFSIZ];
    if(fdi == -1){
        perror("open in");
        exit(1);
    }
    if(fdo == -1){
        perror("open out");
        exit(1);
    }
    int n;
    while((n = read(fdi, buf, sz))){
        if(n == -1){
            perror("read");
            exit(1);
        }
        if(write(fdo, buf, n) == -1){
            perror("write");
            exit(1);
        }
    }
    close(fdi);
    close(fdo);
}

void checkExist(char* out){
    ifstream file(out);
    if(file.good()){
        cout << "file exists" << endl;
        exit(1);
    }
}

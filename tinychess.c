#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int b[120];

const int is[120] = {
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 4, 2, 3, 5, 6, 3, 2, 4, 7,
    7, 1, 1, 1, 1, 1, 1, 1, 1, 7,
    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
    7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
    7,-1,-1,-1,-1,-1,-1,-1,-1, 7,
    7,-4,-2,-3,-5,-6,-3,-2,-4, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

const int v[] = {0, 100, 300, 305, 500, 900, 20000};
int N[]={-21,-19,-12,-8,8,12,19,21}, K[]={-1,1,-10,10,-11,-9,9,11};

int bs, bd;

int e() {
    int s = 0;
    for(int i = 21; i <= 98; i++)
        if (b[i] != 7) s += (b[i] > 0 ? v[b[i]] : -v[-b[i]]);
    return s;
}

int n(int s, int d) {
    if (d==0) return s * e();

    int ms = -20000;
    /* 
    for each move
        make move
        score = -n(-s, d-1)
        undo move
        if score > ms then ms = score
    return ms
    */
}

int S(int s, int d, int a, int be) {
    if (d==0) {
        return s * e();
    }
    for(int src=21; src<=98; src++) {
        int p = b[src];
        if (p == 7 || p==0 || (p>0 && s<0) || (p<0 && s>0)) continue;

        if(abs(p) == 1) {
            int dir = (s == 1)? 10:-10;
            
            int t = src + dir;
            if (b[t] == 0) {
                b[t] = p;
                b[src] = 0;

                int score = -S(-s, d-1, -be, -a);
                b[src] = p;
                b[t] = 0;

                if (score > a) a = score;
                if (a >= be) return be;
            }
            for(int i=-1; i<=1; i+= 2) {
                if (   b[src+i+dir]!=0 &&  b[src+i+dir] != 7 && (b[src+dir+i]>0) != (s>0)) {
                    int t = src + dir + i;
                    int capt = b[t];
                    b[t] = p; 
                    b[src] = 0;

                    int score = -S(-s, d-1, -be, -a);
                    b[src] = p;
                    b[t] = capt;

                    if (score > a) a = score;
                    if (a >= be) return be;
                }
            }
        }
        else {}
    }
    return a;
}
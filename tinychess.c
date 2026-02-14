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

int S(int s, int d, int a, int be) {
    if (d==0) {
        int sc = 0;
        for(int i = 21; i <= 98; i++)
            if (b[i] != 7) sc += (b[i] > 0 ? v[b[i]] : -v[-b[i]]);
        return sc * s;
    }

    for(int src=21; src<=98; src++) {
        int p = b[src];
        if (p == 7 || p==0 || (p>0 && s<0) || (p<0 && s>0)) continue;

        int *vec = (abs(p) == 2)?N:K;
        int len = 8;
        if (abs(p) == 1) len = 0;

        if(abs(p) == 1) {
            int dir = (s == 1)? 10:-10;
            
            if (b[src + dir] == 0) {
                int t = src + dir;
                b[t] = p;
                b[src] = 0;

                int score = -S(-s, d-1, -be, -a);
                b[src] = p;
                b[t] = 0;

                if (score > a) {a = score;
                    if (d==4) {
                        bs = src;
                        bd = t;
                    }}
                if (a >= be) return be;

                if (((s==1 && src<40) || (s==-1 && src>70)) && b[src + 2*dir] == 0) {
                    t = src + 2*dir;
                    b[t] = p;
                    b[src] = 0;

                    score = -S(-s, d-1, -be, -a);
                    b[src] = p;
                    b[t] = 0;

                    if (score > a) {a = score;
                        if (d==4) {
                            bs = src;
                            bd = t;
                        }}
                    if (a >= be) return be;
                }

            }
            for(int i=-1; i<=1; i+= 2) {
                if (b[src+i+dir]!=0 && b[src+i+dir] != 7 && (b[src+dir+i]>0) != (s>0)) {
                    int t = src + dir + i;
                    int capt = b[t];
                    b[t] = p; 
                    b[src] = 0;

                    int score = -S(-s, d-1, -be, -a);
                    b[src] = p;
                    b[t] = capt;

                    if (score > a) {a = score;
                        if (d==4) {
                            bs = src;
                            bd = t;
                        }}
                    if (a >= be) return be;
                }
            }
        }
        else {

            int st = (abs(p) == 4)?0:4;
            int en = ((abs(p)==3)?4:8);

            if (abs(p) == 2 || abs(p) == 5 || abs(p) == 6){
                st = 0; en = 8;
            }

            for(int i = st; i<en; i++) {
                int dir = vec[i];
                int t = src;
                while(1) {
                    t += dir;
                    int target = b[t];
                    if (target == 7) break;
                    if (target!=0 && (target>0) == (s>0)) break;

                    b[t] = p;
                    b[src] = 0;
                    int sc = -S(-s, d-1, -be, -a);
                    b[src] = p;
                    b[t] = target;
                    if (sc > a) {a = sc;
                        if (d==4) {
                            bs = src;
                            bd = t;
                        }}
                    if (a >= be) return be;

                    if (target != 0 || abs(p) == 2 || abs(p) == 6) break;
                }
            }
        }
    }
    return a;
}
#include <string.h>
#define abs(x) ((x)<0?-(x):(x))

int b[120];
const int v[] = {0,1,3,3,5,9,99};
int N[]={-21,-19,-12,-8,8,12,19,21}, K[]={-1,1,-10,10,-11,-9,9,11};

int bs, bd;

void init() {
    for(int i=0; i<120; i++) {
        int r=i/10,c=i%10;
        b[i] = (r<2||r>9||c<1||c>8)?7:(r==3?1:r==8?-1:r==2?"42356324"[c-1]-'0':r==9?-("42356324"[c-1]-'0'):0);
}}

int S(int s, int d, int a, int be);

int E(int s,int d,int a,int be,int src,int t,int p,int capt){
    b[t]=p;b[src]=0;
    int sc=-S(-s,d-1,-be,-a);
    b[src]=p;b[t]=capt;
    if(sc>a){a=sc;if(d==4)bs=src,bd=t;}
    return a>=be?be:a;
}

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

        if(abs(p) == 1) {
            int dir = (s == 1)? 10:-10;
            
            if (b[src + dir] == 0) {
                int t = src + dir;
                a = E(s,d,a,be,src,t,p,0);
                if (a >= be) return be;

                if (((s==1 && src<40) || (s==-1 && src>70)) && b[src + 2*dir] == 0) {
                    t = src + 2*dir;
                    a = E(s,d,a,be,src,t,p,0);
                    if (a >= be) return be;
                }

            }
            for(int i=-1; i<=1; i+= 2) {
                if (b[src+i+dir]!=0 && b[src+i+dir] != 7 && (b[src+dir+i]>0) != (s>0)) {
                    int t = src + dir + i;
                    int capt = b[t];
                    a = E(s,d,a,be,src,t,p,capt);
                    if (a >= be) return be;
                }
            }
        }
        else {
            int *vec = K;
            int st, en;
            if (abs(p) == 2) {
                vec = N;
                st = 0; en = 8;
            } else if(abs(p)==4) {
                st =0; en =4;
            } else if (abs(p)==3) {
                st =4; en = 8;
            } else {
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

                    a = E(s,d,a,be,src,t,p,target);
                    if (a >= be) return be;

                    if (target != 0 || abs(p) == 2 || abs(p) == 6) break;
                }
            }
        }
    }
    return a;
}

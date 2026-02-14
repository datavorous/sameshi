#define abs(x) ((x)<0?-(x):(x))
#define r(i,a,b) for(int i=(a);i<(b);i++)
const int v[]={0,1,3,3,5,9,99};
int N[]={-21,-19,-12,-8,8,12,19,21},K[]={-1,1,-10,10,-11,-9,9,11};
int bs, bd, b[120];
void I(){r(i,0,120){int r=i/10,c=i%10;b[i]=(r<2||r>9||c<1||c>8)?7:(r==3?1:r==8?-1:r==2?"42356324"[c-1]-'0':r==9?-("42356324"[c-1]-'0'):0);}}
int S(int s,int d,int a,int be);
int C(int s){
    int k=0,e=-s;
    r(i,21,99) if(b[i]==6*s){k=i;break;}
    if(!k)return 0;
    if (s==1){if(b[k+9]==-1||b[k+11]==-1)return 1;}
    else{if(b[k-9]==1||b[k-11]==1)return 1;}
    r(i,0,8)if(b[k+N[i]]==2*e)return 1;
    r(i,0,8)if(b[k+K[i]]==6*e)return 1;
    r(i,0,4){int t=k;while(1){t+=K[i];if(b[t]==7)break;if(!b[t])continue;if((b[t]>0)==(e>0)&&(abs(b[t])==4||abs(b[t])==5))return 1;break;}}
    r(i,4,8){int t=k;while(1){t+=K[i];if(b[t]==7)break;if(!b[t])continue;if((b[t]>0)==(e>0)&&(abs(b[t])==3||abs(b[t])==5))return 1;break;}}
    return 0;}
int E(int s,int d,int a,int be,int src,int t,int p,int capt,int *m){
    b[t]=p;b[src]=0;
    if(C(s)){b[src]=p;b[t]=capt;return a;}*m=1;
    int sc=-S(-s,d?d-1:0,-be,-a);
    b[src]=p;b[t]=capt;
    if(sc>a){a=sc;if(d==5)bs=src,bd=t;}
    return a>=be?be:a;}
int S(int s,int d,int a,int be) {
    int q=(d==0);
    int m=0;
    if (q){
        int sc = 0;
        r(i,21,99)if (b[i]!=7)sc+=(b[i]>0?v[b[i]]:-v[-b[i]]);
        sc*= s;
        if(sc>a)a=sc;
        if(a>=be)return be;
    }
    r(pass,0,(q?1:2)){
    r(src,21,99) {
        int p=b[src];
        if(p==7||p==0||(p>0&&s<0)||(p<0&&s>0))continue;
        if(abs(p)==1){
            int dir=(s==1)?10:-10;
            if (pass==0){
                for(int i=-1;i<=1;i+= 2) {
                    int t=src+dir+i;
                    int capt=b[t];
                    if (capt!=0&&capt!=7&&(capt>0)!=(s>0)) {
                        a = E(s,d,a,be,src,t,p,capt,&m);
                        if (a>=be)return be;
                    }
                }
            }else{
                if(b[src+dir]==0) {
                    int t=src+dir;
                    a = E(s,d,a,be,src,t,p,0,&m);
                    if (a>=be)return be;
                    if (((s==1&&src<40)||(s==-1&&src>70))&&b[src+2*dir]==0){
                        t=src+2*dir;
                        a=E(s,d,a,be,src,t,p,0,&m);
                        if(a>=be)return be;
                    }
                }
            }
        }
        else {
            int *vec=K;
            int st,en;
            if (abs(p)==2){vec=N;st=0;en=8;} else if(abs(p)==4){st=0;en=4;}
            else if(abs(p)==3){st=4;en=8;}else{st=0;en=8;}
            r(i,st,en){
                int dir=vec[i];
                int t=src;
                int ray=abs(p)!=2&&abs(p)!=6;
                while(1){
                    t+=dir;
                    int target=b[t];
                    if (target==7) break;
                    if (target!=0&&(target>0)==(s>0))break;
                    if (pass==0) {
                        if (target!=0) {
                            a = E(s,d,a,be,src,t,p,target,&m);
                            if (a >= be) return be;
                            break;
                        }
                    } else {
                        if (target==0) {
                            a = E(s,d,a,be,src,t,p,0,&m);
                            if (a>=be)return be;
                        }else break;
                    }
                    if(!ray)break;
                }
            }
        }
    }
    }
    if(!m && !q) return C(s)?-9999:0;
    return a;
}

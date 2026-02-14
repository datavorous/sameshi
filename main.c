#include <stdio.h>
#include "sameshi.h"

extern int b[120],bs,bd;
void p(){
    printf("\n  a b c d e f g h\n");
    for(int r=9;r>=2;r--){
        printf("%d ",r-1);
        for(int c=1;c<=8;c++){
            int q=b[r*10+c];
            char s=".PNBRQK"[j(q)];
            if(q<0)s+=32;
            printf("%c ",s);
        }
        puts("");
    }
}
int main(){
    I();
    char f[5];
    int t=1;
    while(1){
        p();
        if(t>0){
            printf("> ");
            scanf("%4s",f);
            int s=(f[1]-'0'+1)*10+(f[0]-'a'+1);
            int d=(f[3]-'0'+1)*10+(f[2]-'a'+1);
            b[d]=b[s];b[s]=0;
        }else{
            S(-1,5,-30000,30000);
            b[bd]=b[bs];b[bs]=0;
        }
        t=-t;
    }
}

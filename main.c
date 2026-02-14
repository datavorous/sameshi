#include <stdio.h>
#include "sameshi.h"

extern int b[120], bs, bd;

static char pc(int q){
    int a=j(q);
    if(!a)return '.';
    if(a==1)return q>0?'P':'p';
    if(a==2)return q>0?'N':'n';
    if(a==3)return q>0?'B':'b';
    if(a==4)return q>0?'R':'r';
    if(a==5)return q>0?'Q':'q';
    return q>0?'K':'k';
}

static void board(void){
    puts("");
    puts("  a b c d e f g h");
    for(int r=9;r>=2;r--){
        printf("%d ",r-1);
        for(int c=1;c<=8;c++)printf("%c ",pc(b[r*10+c]));
        printf("%d\n",r-1);
    }
    puts("  a b c d e f g h");
}

static int sq(char f,char r){
    if(f<'a'||f>'h'||r<'1'||r>'8')return -1;
    return (r-'0'+1)*10+(f-'a'+1);
}

int main(void){
    I();
    char m[8];
    while(1){
        board();
        printf("move: ");
        if(scanf("%7s",m)!=1)break;
        if(m[0]=='q')break;
        int s=sq(m[0],m[1]),d=sq(m[2],m[3]);
        if(s<0||d<0)continue;
        b[d]=b[s];
        b[s]=0;
        S(-1,5,-30000,30000);
        printf("ai: %c%c%c%c\n",'a'+bs%10-1,'0'+bs/10-1,'a'+bd%10-1,'0'+bd/10-1);
        b[bd]=b[bs];
        b[bs]=0;
    }
    return 0;
}

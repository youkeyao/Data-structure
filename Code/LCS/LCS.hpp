#ifndef LCS_hpp
#define LCS_hpp

#include <cstring>

int LCS(char *a,char *b){
    int len1=strlen(a),len2=strlen(b),**result,answer;
    result=new int*[len1+1];
    for(int i=0;i<=len1;i++)result[i]=new int[len2+1];
    for(int i=0;i<=len1;i++)
        for(int j=0;j<=len2;j++)
            result[i][j]=0;
    for(int i=0;i<len1;i++){
        for(int j=0;j<len2;j++){
            if(a[i]==b[j]){
                result[i+1][j+1]=result[i][j]+1;
            }
            else{
                result[i+1][j+1]=result[i+1][j]>result[i][j+1] ? result[i+1][j]:result[i][j+1];
            }
        }
    }
    answer=result[len1][len2];
    for(int i=0;i<=len1;i++)delete [] result[i];
    delete [] result;
    return answer;
}

#endif
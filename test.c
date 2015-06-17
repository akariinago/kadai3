
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SIZE1 52973671      //links.txtの総行数
#define SIZE2 1483277       //pages.txtの総行数

#define WORD_LENGTH 1000     //wikipediaの見出し語の長さ

typedef struct data_t1{
    int id;                 //ページのid
    int linked;             //リンクされているページのid
}data_type1;

typedef struct data_t2{
    int id;
    char *word;
}data_type2;

data_type1 table1[SIZE1];     //大きさSIZE1の配列を宣言
data_type2 table2[SIZE2];     //大きさSIZE2の配列を宣言

int num_linked[SIZE2]={0};  //被リンク数を格納する配列を宣言、0で初期化

int n;                      //リンク情報数
int m;                      //ページ情報数


void insert_linkend(int id,int linked){ /*table1の最後にデータを追加する*/
    if(n<SIZE1-1){  /*table1に空きがあるなら*/
        table1[n+1].id=id;
        table1[n+1].linked=linked;
        n++;
    }else{
        printf("テーブルがいっぱいです\n");
        exit(1);
    }
}

void insert_pageend(int id,char *word){ /*table2の最後にデータを追加する*/
    if(m<SIZE2-1){  /*table2に空きがあるなら*/
        table2[m+1].id=id;
        table2[m+1].word=word;
        m++;
    }else{
        printf("テーブルがいっぱいです\n");
        exit(1);
    }
}

void read_linkfile(char *filename){ /*ファイルからリンク情報を読み込む*/
    FILE *file;
    int id,linked;
    int r;
    
    file=fopen(filename,"r");   //ファイルを読み込み専用で開く
    if(file==NULL){ /*ファイルが見つからなかったら*/
        printf("ファイル%sが見つかりません\n",filename);
        exit(1);    //エラーを出して終了
    }
    while(1){
        r=fscanf(file,"%d\t%d\n",&id,&linked);  //1行リンク情報を読み込む
        if(r==EOF){ /*ファイルの終わりなら*/
            fclose(file);   //ファイルを閉じて
            return; //終了
        }else{  /*データがあったなら*/
            insert_linkend(id,linked);  //それを追加して読み込みを続ける
        }
    }
}

void read_pagefile(char *filename){ /*ファイルからページ情報を読み込む*/
    FILE *file;
    int id;
    char *word;
    int r;
    
    file=fopen(filename,"r");   //ファイルを読み込み専用で開く
    if(file==NULL){ /*ファイルが見つからなかったら*/
        printf("ファイル%sが見つかりません\n",filename);
        exit(1);    //エラーを出して終了
    }
    while(1){
        word=(char *)malloc(WORD_LENGTH);   //見出し語を格納するメモリを確保
        r=fscanf(file,"%d\t%s\n",&id,word); //1行、ページ情報を読み込む
        if(r==EOF){ /*ファイルの終わりなら*/
            fclose(file);   //ファイルを閉じて
            return; //終了
        }else{  /*データがあったなら*/
            insert_pageend(id,word);    //それを追加して読み込みを続ける
        }
    }
    
}


int main(int argc, char *argv[]){
    
    int id1=-1; //入力した単語のid
    int x=0; //入力した単語のリンク数
    int y=-1;//入力した単語のリンクの最初の配列数
    int random;//乱数
    int i=0,j=0;
    if(argc!=3){
        printf("単語と数を入力してください\n");
        exit(1);
    }
    n=-1;    //table1[0]からデータを入れるために-1に初期化
    read_linkfile("links.txt"); //リンク情報を読み込む
    m=-1;   //table2[0]からデータを入れるために-1に初期化
    read_pagefile("pages.txt"); //ページ情報を読み込む
    //printf("データ読み込み終わり\n");
 
    for(i=0; i<SIZE2;i++){
        if(strcmp(table2[i].word,argv[1])==0){    //入力された単語のidを見つける
            id1 = table2[i].id;
            break;
        }
    }
    
    if(id1==-1){
        printf("その単語はwikipediaにありません\n");
        exit(1);
    }
    printf("%s → ", table2[id1].word);
    for(i=0; i<atoi(argv[2]); i++){
        j=0;
        x=0;
        y=-1;
        while(table1[j].id<id1+1) {
            if(table1[j].id==id1){
                x++;            //単語のリンク数を求める
                if(y==-1){
                    y=j;        //リンクの最初の行を求める
                }
            }
            j++;
        
        }
        if(x==0){
            printf("%sのリンクはありません\n",table2[id1].word);
            exit(1);
        }
        
        srand((unsigned)time(NULL));    //乱数を発生させる
        random=y+rand()%x;
        j=0;
        while(j<SIZE2){                 //単語のリンクをランダムに出力する
            if(table2[j].id==table1[random].linked){
                if(i!=atoi(argv[2])-1){
                    printf("%s → ", table2[j].word);
                    id1=table2[j].id;
                break;
                }
                else{
                    printf("%s\n", table2[j].word);
                    id1=table2[j].id;
                break;
                }
                
            }
            j++;
            
        }
        
    }

}
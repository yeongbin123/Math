#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  FILE * fp = fopen("formula", "w") ;
  FILE * fpc = fopen("test","r");
  int **a=NULL;
  char buffer[20];

   
  int row=0,col=0;
  //파일 읽어 들여서 row,column 수 알아내고 그 각각에 해당 값을 -1~9까지로
 //받아서 원소로 가지기
  int x, y ;
  while(1){
    fgets(buffer, sizeof(buffer), fpc);    // hello.txt에서 문자열을 읽음
    if(feof(fpc))break;
    row++;
    //printf("%s", buffer);
    col=strlen(buffer)/2;
    //여기서 column이 다르면 return null
   }
  fclose(fpc);

  //row 와 column은 알게됨.
	a=(int**)malloc(row * sizeof (*a)); 
	for (int j = 0; j < row; j++)      
	a[j]=(int*)malloc(col * sizeof(**a));
//할당
FILE * fpd = fopen("test","r");
char c;
int k=0,l=0;
	while(1){
		c = fgetc(fpd);
		if(feof(fpd)) break;
		if(c!='\n'&&c!=' '&&c!='\0'){
			k++;
			if((k%row)!=1){
				if(c=='?'){
					//printf("-1 ");
					a[k-1][l-1]=-1;
				}else{
					//printf("%d ",atoi(&c));	
					a[k-1][l-1]=atoi(&c);
				}
			}
			else if((k%row)==1){
				k=k%row;
				l++;
				if(c=='?'){
					//printf("\n-1 ");
                                        a[k-1][l-1]=-1;
				}else{
					//printf("\n%d ",atoi(&c));
                                        a[k-1][l-1]=atoi(&c);
                                }
			}
		}
	}
	//printf("\n");
//여기서 Bmn행렬 formula 텍스트 파일에 선언
 /* for (i = 1 ; i <= row ; i++)
        for (j = 1 ; j <= col ; j++)
                fprintf(fp, "(declare-const B%d%d Int)\n", i, j) ;*/
  fclose(fpc);
//여기서 부터 명제
  for (int i = 1; i <= row; i++)
      for(int j = 1; j <= col; j++)
  		 fprintf(fp,"(assert(and (<= B%d%d 1) (<= 0 B%d%d)))\n", i, j, i, j);

  for (int i = 1; i <= row; i++) // Aij가 -1이 아닐경우 그 값의 합은 아래로 판단
    for (int j = 1; j <= col; j++)
      if(a[i-1][j-1] != a[0][0] && a[i-1][j-1] != a[0][col-1] && a[i-1][j-1] != a[row-1][0] && a[i-1][j-1] != a[row-1][col-1])
        if(a[i-1][j-1] != -1)
          if(!(i == 1 && (j != 1 && j != col))&&!(j == 1 && (i != 1 && i != row))&&!(j == col && (i != 1 && i != row))&&!(i == row && (j != 1 && j != col)))
      	   fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d) %d))\n",i-1,j-1,i-1,j,i-1,j+1,i,j-1,i,j,i,j+1,i+1,j-1,i+1,j,i+1,j+1,a[i-1][j-1]);

  if(a[0][0] != -1)
    fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d) %d))\n",1,1,1,2,2,1,2,2,a[0][0]);
  if(a[0][col-1] != -1)
    fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d) %d))\n",1,col,1,col-1,2,col,2,col-1,a[0][col-1]);
  if(a[row-1][0] != -1)
    fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d) %d))\n",row,1,row-1,1,row,2,row-1,2,a[row-1][0]);
  if(a[row-1][col-1] != -1)
    fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d) %d))\n",row,col,row,col-1,row-1,col,row-1,col-1,a[row-1][col-1]);

  for (int i = 1; i <= row; i++)
    for (int j = 1; j <= col; j++)
    {
      if(a[i-1][j-1] != -1 && (i == 1 && j != 1 && j != col))
        fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d) %d))\n",i,j,i,j+1,i,j-1,i+1,j,i+1,j+1,i+1,j-1,a[i-1][j-1]);
      if(a[i-1][j-1] != -1 && (j == 1 && i != 1 && i != row))
        fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d) %d))\n",i,j,i,j+1,i-1,j,i-1,j+1,i+1,j,i+1,j+1,a[i-1][j-1]);
      if(a[i-1][j-1] != -1 && (j == col && i != 1 && i != row))
        fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d) %d))\n",i,j,i-1,j,i-1,j-1,i,j-1,i+1,j,i+1,j-1,a[i-1][j-1]);
      if(a[i-1][j-1] != -1 && (i == row && j != 1 && j != col))
        fprintf(fp,"(assert(=(+ B%d%d B%d%d B%d%d B%d%d B%d%d B%d%d) %d))\n",i,j,i,j-1,i,j+1,i-1,j,i-1,j-1,i-1,j+1,a[i-1][j-1]);
    }

  fprintf(fp,"(check-sat)\n(get-model)\n");
  fclose(fp) ;
/*
	FILE * fin = popen("z3 formula", "r") ; //FIXME
	char buf[128] ;
	fscanf(fin, "%s %s", buf, buf) ;
	while (!feof(fin)) {
		fscanf(fin, "%s", buf) ; printf("%s ", buf) ;
		fscanf(fin, "%s", buf) ; printf("%s ", buf) ;
		fscanf(fin, "%s", buf) ; printf("%s ", buf) ;
		fscanf(fin, "%s", buf) ; printf("%s ", buf) ;
		fscanf(fin, "%s", buf) ; printf("%s\n", buf) ;
	}
	pclose(fin) ;*/
}


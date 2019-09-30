#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FILE * fpc =NULL;


int main (int argc, char*argv[])
{
  char str[1024], *token;
  FILE * fp = fopen("formula", "w") ;
  FILE * fpc = NULL;
  int case_count =0;
 if(argc>1)
        //printf("go");
        fpc = fopen(argv[1],"r");
  else{
        printf("not a form\n");
        exit(-1);}

  int **a=NULL;
  char buffer[1024];

   
  int row=0,col=0;
  //파일 읽어 들여서 row,column 수 알아내고 그 각각에 해당 값을 -1~9까지로
 //받아서 원소로 가지기
  int x, y ;
  int count_col=0, count_row=0;
 /* while(1){
    fgets(buffer, sizeof(buffer), fpc);   
    if(feof(fpc))break;
    row++;
    //printf("%s", buffer);
    col=strlen(buffer)/2;
    //여기서 column이 다르면 return null
   }
  fclose(fpc);*/
     while(fgets(str,1024,fpc) != NULL){
    str[strlen(str)-1] = 0; // delete ‘\n’
    row++;
    token = strtok(str, " ");
    while(token) {
      col++;
      token = strtok(NULL, " ");
    }
  }
  col /= row;

  fclose(fpc);

  //row 와 column은 알게됨.
	a=(int**)malloc(row * sizeof (*a)); 
	for (int j = 0; j < row; j++)      
	a[j]=(int*)malloc(col * sizeof(**a));
//할당
//FILE * fpd = fopen(argv[1],"r");
FILE * fpd = fopen("test","r");
char c;
int k=0,l=0;
	while(1){
		c = fgetc(fpd);
		if(feof(fpd)) break;
		if(c!='\n'&&c!=' '&&c!='\0'){
			l++;
			if((l%col)!=1){
				if(c=='?'){
					//printf("-1 ");
					a[k-1][l-1]=-1;
				}else{
					//printf("%d ",atoi(&c));	
					a[k-1][l-1]=atoi(&c);
				}
			}
			else if((l%col)==1){
				l=l%col;
				k++;
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
  fclose(fpc);
	//printf("\n");
//행렬 테스트
	/*for(int i=0;i<row;i++){
		for(int j=0;j<col;j++)
			printf("%d ",a[i][j]);
		printf("\n");}*/
//여기부터 명제시작
//여기서 Bmn행렬 formula 텍스트 파일에 선언
  for (int i = 1 ; i <= row ; i++)
        for (int j = 1 ; j <= col ; j++)
                fprintf(fp, "(declare-const B%d_%d Int)\n", i, j) ;
  for (int i = 1; i <= row; i++)
      for(int j = 1; j <= col; j++)
  		 fprintf(fp,"(assert(and (<= B%d_%d 1) (<= 0 B%d_%d)))\n", i, j, i, j);

  for (int i = 1; i <= row; i++) // Aij가 -1이 아닐경우 그 값의 합은 아래로 판단
    for (int j = 1; j <= col; j++)
      if(a[i-1][j-1] != a[0][0] && a[i-1][j-1] != a[0][col-1] && a[i-1][j-1] != a[row-1][0] && a[i-1][j-1] != a[row-1][col-1])
        if(a[i-1][j-1] != -1)
          if(!(i == 1 && (j != 1 && j != col))&&!(j == 1 && (i != 1 && i != row))&&!(j == col && (i != 1 && i != row))&&!(i == row && (j != 1 && j != col)))
      	   fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",i-1,j-1,i-1,j,i-1,j+1,i,j-1,i,j,i,j+1,i+1,j-1,i+1,j,i+1,j+1,a[i-1][j-1]);

  if(a[0][0] != -1)
    fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",1,1,1,2,2,1,2,2,a[0][0]);
  if(a[0][col-1] != -1)
    fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",1,col,1,col-1,2,col,2,col-1,a[0][col-1]);
  if(a[row-1][0] != -1)
    fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",row,1,row-1,1,row,2,row-1,2,a[row-1][0]);
  if(a[row-1][col-1] != -1)
    fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",row,col,row,col-1,row-1,col,row-1,col-1,a[row-1][col-1]);

  for (int i = 1; i <= row; i++)
    for (int j = 1; j <= col; j++)
    {
      if(a[i-1][j-1] != -1 && (i == 1 && j != 1 && j != col))
        fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",i,j,i,j+1,i,j-1,i+1,j,i+1,j+1,i+1,j-1,a[i-1][j-1]);
      if(a[i-1][j-1] != -1 && (j == 1 && i != 1 && i != row))
        fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",i,j,i,j+1,i-1,j,i-1,j+1,i+1,j,i+1,j+1,a[i-1][j-1]);
      if(a[i-1][j-1] != -1 && (j == col && i != 1 && i != row))
        fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",i,j,i-1,j,i-1,j-1,i,j-1,i+1,j,i+1,j-1,a[i-1][j-1]);
      if(a[i-1][j-1] != -1 && (i == row && j != 1 && j != col))
        fprintf(fp,"(assert(=(+ B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d B%d_%d) %d))\n",i,j,i,j-1,i,j+1,i-1,j,i-1,j-1,i-1,j+1,a[i-1][j-1]);
    }

  fprintf(fp,"(check-sat)\n(get-model)\n");




while(case_count++ < 5)
{
  FILE * fin = popen("z3 formula", "r") ; //FIXME

  char board[row+1][col+1] ;

  int i, j, z;

  int check = 0;



  char b[128] ;

  char s[128] ;

  char n[128] ;

  char q[128] ;
  char o,p;

  fseek(fp, -24, SEEK_END);

  fscanf(fin, "%s %s", b, q) ;

  if(strcmp(b,"unsat")==0){
    printf("No solution!\n");
    exit(-1);
  }

  fprintf(fp,"(assert(not(=(+");

  for (z = 0 ; z < row*col ; z++) {

    fscanf(fin,"%s %s %s %s %s", b, s, b, b, n) ;
    sscanf(s,"%c%d%c%d",&o,&i,&p,&j);




    if (strcmp(n, "0)") != 0) {
            check++;
            board[i][j] = 1 ;
            fprintf(fp," B%d_%d",i,j);
    }
    else board[i][j] =0;
  }

  fprintf(fp,")%d)))\n",check);
  fprintf(fp,"(check-sat)\n(get-model)\n");
  check = 0;

  for (i = 1 ; i <= row ; i++) {
      for (j = 1 ; j <= col ; j++) {
              printf("%d ", board[i][j]) ;
      }
              printf("\n") ;

        }
	printf("\n");
pclose(fin);
}
  fclose(fp);



        /*while (!feof(fin)) {

                fscanf(fin, "%s", buf) ; printf("%s ", buf) ;

                fscanf(fin, "%s", buf) ; printf("%s ", buf) ;

                fscanf(fin, "%s", buf) ; printf("%s ", buf) ;

                fscanf(fin, "%s", buf) ; printf("%s ", buf) ;

                fscanf(fin, "%s", buf) ; printf("%s\n", buf) ;

        }*/


}

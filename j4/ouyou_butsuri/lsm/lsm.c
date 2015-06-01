#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct dataset {
  double x;
  double y;
};

void inputData(struct dataset data[], int dataNumber, char filename[256])
{
  FILE *fp;
  int i;

  fp = fopen(filename, "r");
  if(fp==NULL) {
    printf("Error.\n");
    printf("Can't open file.\n");
    exit(1);
  }
  
  for(i=0; i<dataNumber; i++) {
    fscanf(fp, "%lf", &data[i].x);
    fscanf(fp, "%lf", &data[i].y);
  }
  
  fclose(fp);
}

void dispData(struct dataset data[], int dataNumber)
{
  int i;
  
  for(i=0; i<dataNumber; i++) {
    printf("%lf %lf\n", data[i].x, data[i].y);
  }

}

void lsm(struct dataset data[], int dataNumber)
{
  dispData(data, dataNumber);

  printf("\n");
  printf("わかりやすく出力\n");
  printf("(%lf, %lf)\n", data[0].x, data[0].y);
  printf("(%lf, %lf)\n", data[1].x, data[1].y);
  printf("(%lf, %lf)\n", data[2].x, data[2].y);

  /* ここから最小二乗法のプログラムを書く */

  int sum_xxxx, sum_xxx, sum_xx, sum_x;
  int sum_xxy, sum_xy, sum_y;
  int n = dataNumber;

  sum_xxxx = sum_xxx = sum_xx = sum_x = 0;
  sum_xxy - sum_xy = sum_y = 0;

  for(i = 0; i < dataNumber; i++){
    sum_xxxx += data[i].x * data[i].x * data[i].x * data[i].x;
    sum_xxx += data[i].x * data[i].x * data[i].x;
    sum_xx += data[i].x * data[i].x;
    sum_x += data[i].x;
    sum_xxy += data[i].x * data[i].x * data[i].y;
    sum_xy += data[i].x * data[i].y;
    sum_y += data[i].y;
  }

  int fac[][] = {
    {sum_xxxx, sum_xxx, sum_xx},
    {sum_xxx, sum_xx, sum_x},
    {sum_xx, sum_x, n}
  };
  
  int temp = 1;
  for(i = 0; i < 3; i++){
    temp = fac[i];
}

  int bunshi = ;

}


int main(int argc, char *argv[])
{
  struct dataset *data;
  int dataNumber;
  char filename[256];

  if(argc-1 != 2) {
    printf("Error.\n");
    printf("Usage: ./a.out [Data Number] [Filename]\n");
    exit(1);
  }
  
  dataNumber = atoi(argv[1]);
  strcpy(filename, argv[2]);

  data = (struct dataset *)malloc(sizeof(struct dataset)*dataNumber);

  inputData(data, dataNumber, filename);

  lsm(data, dataNumber);

  return 0;
}

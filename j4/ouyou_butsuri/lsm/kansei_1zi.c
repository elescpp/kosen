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
  printf("$B$o$+$j$d$9$/=PNO(B\n");
  printf("(%lf, %lf)\n", data[0].x, data[0].y);
  printf("(%lf, %lf)\n", data[1].x, data[1].y);
  printf("(%lf, %lf)\n", data[2].x, data[2].y);

  /* $B$3$3$+$i:G>.Fs>hK!$N%W%m%0%i%`$r=q$/(B */
  int sum_x;
  int sum_xs;
  int sum_y;
  int sum_xy;
  int n = dataNumber;  
  sum_x = sum_xs = sum_y = sum_xy = 0;
  
  int i;
  for(i = 0; i < dataNumber; i++){
    sum_x += data[i].x;
    sum_xs += data[i].x * data[i].x;
    sum_y += data[i].y;
    sum_xy += data[i].x * data[i].y;
  }
  
  double bunshi = (sum_xs * n) - (sum_x * sum_x);
  
  double a = (n * sum_xy) + (-sum_x * sum_y);
  double b = (-sum_x * sum_xy) + (sum_xs * sum_y);
  
  a /= bunshi;
  b /= bunshi;
  
  printf("a=%f, b = %f\n", a, b);
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

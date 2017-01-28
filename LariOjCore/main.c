    #include<stdio.h>
    //#include "/dev/zero"
    int main(void)
    {
    double counter;
    double result;
    int temp;
    
    char buf[1024];
    scanf("%s %d",buf,&temp);buf[10265] = 5;
    printf("you said %s , tmp = %c ",buf,buf[1025]);
    printf("result is %lf\n ", result);
    return 0;}
    
#include <stdio.h>
#include <string.h>
#include "laplace.h"
#include "csvpackage.h"
#include <time.h>
extern int rand();
extern void srand(unsigned);
/*
函数功能：	对传入的csv文件进行处理，提取其中数据并生成拉普拉斯分布的噪音进行加噪
输入参数说明：
path		csv文件的存储位置
beta		拉普拉斯分布参数
seed	    长整型指针变量， *seed 为伪随机数的种子
*/
double csv_analysis(char* path, double beta, long int seed)
{
	FILE *original_file = fopen(path,"r+"); //读取指定路径的数据集
	struct Animals * original_data = NULL;
	original_data = csv_parser(original_file);
	int sum=0,i=0;
	double x = 0;
    //直接从原始数据集中读取数据
	while(original_data[i].name)  
	{
		if(original_data[i].carrots + x >=55)
		{
			sum++;
		}
		i++;
	}
    //printf("Animals which carrots cost > 55 (without noise): %d\n",sum); 
    //为查询返回的结果添加噪音
    x = laplace_data(beta, &seed);
    //printf("Animals which carrots cost > 55 (without noise): %f\n",sum + x); 
    return sum + x;
	
}

/*
参数表：
seed	    长整型指针变量， *seed为伪随机数的种子
sen			数据集的敏感度
x			用于储存拉普拉斯分布噪音的临时变量
beta		隐私预算，在输入后根据公式转换为拉普拉斯分布参数
*/
int main()
{
	long int seed;
	int sen = 1;  //对于一个单属性的数据集，其敏感度为1
	double beta;
	srand((unsigned)time( NULL )); //生成基于时间的随机种子（srand方法）
	beta = 0;
	printf("Please input laplace epsilon:");
	scanf("%lf", &beta);
    
    int maxquerytime=20;//定义最大查询次数
    int inputquerytime;//实际查询次数
    printf("Please input query times:");
    scanf("%d",&inputquerytime);


	if(beta<=0 || !beta)//当输入的beta值无效时，默认设定beta值为1
	{
		beta = 1.0;
	} 
    if(inputquerytime > maxquerytime)//防止信息泄露
    {
        inputquerytime = maxquerytime;
    }

	printf("Under privacy budget %f, interactive query result is\n",beta);
	//考虑保护多次查询，为每次查询分配预算为beta/times：
    beta = beta / maxquerytime;
    //拉普拉斯机制下，实际公式的算子beta为敏感度/预算
    beta = sen / beta; 
    //记录每一次查询的结果，后续取平均
    double * queryresults = (double*)malloc(inputquerytime * sizeof(double));
    //每次查询都重新生成随机数种子
    for(int i=0;i<inputquerytime;i++)
    {
        seed = rand()%10000+10000; //随机种子产生
        double temp = csv_analysis("./zoo.csv", beta, seed);
        queryresults[i] = temp;
    }
    double average = 0;
    for(int i=0;i<inputquerytime;i++)
    {
        average +=queryresults[i]/inputquerytime;
    }
    //交互式DP结果
    printf("Interactive average query result is \t%f\n", average);
	printf("==================Using neighbour dataset==================\n");
    double * neighborqueryresults = (double*)malloc(inputquerytime * sizeof(double));
    //记录每一次查询的结果，后续取平均
    double * neighborqueryresults = (double*)malloc(inputquerytime * sizeof(double));
    //每次查询都重新生成随机数种子
    for(int i=0;i<inputquerytime;i++)
    {
        seed = rand()%10000+10000; //随机种子产生
        double temp = csv_analysis("./zoo_nb.csv",beta,seed); //再调用相邻数据集
        neighborqueryresults[i] = temp;
    }
	average = 0;
    for(int i=0;i<inputquerytime;i++)
    {
        average +=queryresults[i]/inputquerytime;
    }
    //交互式DP结果
    printf("NeiborSet Interactive average query result is \t%f\n", average);
	
	return 0;
}

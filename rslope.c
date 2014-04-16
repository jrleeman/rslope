/* rslope does a running average best fit slope for ascii x y pairs output is x dy/dx	*/

#include	<stdio.h>
#include	<math.h>
#define		SIZE 4000			/*max data points*/

main (ac,av)


int	ac;
char	*av[];
{

float   x[SIZE], y[SIZE];        /* declaration of data buffer */
float	*next[2];				/* pointers to columns x and y */
char	intstring[10];
char	outfile[13],skip[3];
char	*strcpy(), *index(), *strcat();
FILE	*inff,*outf, *fopen();
int	i,j,n_data,n;
void 	exit();
double  xyprod,xsum,ysum,xsqr,slope;	

	if(ac < 2)
	{
	puts("usage:	rslope filename\n");
	exit(2);
	}

	fprintf(stderr,"\nhow many data points to skip for each used or 'n' for none?  ");
	scanf("%s", skip);
	fprintf(stderr,"\n");

	getAveragingLength(&n,intstring);

	read_data(av[1], x, y, &n_data, skip, &n);

/*	strcat(intstring, "rs");*/


/*	strcpy(outfile,av[1]);
	if(index(av[1],'.'))	strcat(outfile, intstring);	
	else
	{	strcat(outfile, ".");  strcat(outfile, intstring);  }					
	outf = fopen( outfile, "w");
*/
	next[0] = x;
	next[1] = y;

	for(i=0; i< n_data; ++i){
		xsum=ysum=xyprod=xsqr=0;

		for(j=0;j<n;++j){
			xsum += *next[0];
			xsqr += *next[0] * *next[0];
			xyprod += *next[0] * *next[1];
			ysum += *next[1];
			next[0]++;
			next[1]++;
		}
		
		next[0] -= (n-1);		   /*move pointer back so that */
		next[1] -= (n-1);		   /*beginning of window is at */
						   /*the next data point*/

	slope = ( (double)n*xyprod - xsum*ysum )/( (double)n*xsqr- xsum*xsum );

	printf("%f %f\n",x[i+(int)n/2],slope);       /*output */
	/*fprintf(outf,"%f %f\n",x[i+(int)n/2],slope);      */
	}


	/*fprintf(stderr,"xsum=%f ysum%f xyprod=%f n*xsqr=%f slope= %f\n",xsum,ysum,xyprod,n_data*xsqr,(n_data*xyprod-xsum*ysum)/(n_data*xsqr- xsum*xsum));
	fprintf(outf,"x=%f slope=%f xsum=%f ysum=%f xyprod=%f sxsqr=%f\n",gamma,slope,xsum,ysum,xyprod,xsqr);*/



close(outf);
exit(0);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
getAveragingLength(pn,ints)
int 	*pn;
char 	*ints;
{
int	n,bad;

do
{
	fprintf(stderr,"what window length for the fit?  ");
	scanf("%s",ints);
	fprintf(stderr,"\n");
	n=2*(atoi(ints)/2) + 1;			/* force n odd */
	if(n<3 || n>SIZE)	bad=1;
	else	bad=0;
} while(bad);
*pn = n;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


read_data( file, x, y, pdata, skip, pn)

int	*pn,*pdata;
char	*file,skip[];
float	x[], y[];
{
    FILE	*data, *fopen();
    static int     nn = 0;
	int	   i;
	float 	   garb,garb1;
	void	exit();

    if ( (data = fopen( file, "r")) == NULL ) {
		fprintf( stderr, "can't open data file\n");
		exit( 10);
    }
	
	fscanf(data,"%f %f",&x[nn],&y[nn]);	
	++nn;

	for(i=0;i<(int)*pn/2;++i){
		 x[nn] = x[nn-1];
		 y[nn] = y[nn-1];
		 ++nn;
	}
	
	if (skip[0] != 'n'){
		while ( fscanf( data, "%f", &x[nn] ) != EOF ){
		fscanf( data, "%f", &y[nn]);
		for(i=0; i <atoi(skip); ++i) fscanf(data, "%f %f",&garb,&garb1);
		++nn;
		}
	}

	else{
	   	while ( fscanf( data, "%f", &x[nn] ) != EOF ){
		fscanf( data, "%f", &y[nn]);
		++nn;
		}
	}

	for(i=0;i<(int)*pn/2;++i){
		 x[nn] = x[nn-1];
		 y[nn] = y[nn-1];
		 ++nn;
	}
		
    fclose(data);
    *pdata = nn-*pn+1;
    fprintf( stderr, " # of data points = %d\n",*pdata);
}

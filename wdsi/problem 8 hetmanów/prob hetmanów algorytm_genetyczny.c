#include <stdio.h>
#include <time.h> //rand, srand
#include <stdlib.h>

FILE *srednie;
FILE *najlepsze;
int ile_kolizji(int tab[], int n);
int decyzja(int **populacja, int *the_best, int P, int N);
void sortowanie(int **tab, int P, int N);
int tak_czy_nie(float prawdopodobienstwo);

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int N,P=100; N=atoi(argv[1]);
	//printf("Szachownica ma rozmiar N*N. Ile ma wynosić N?\nN=");
	printf("Szachownica ma rozmiar N*N. N=%d\n", N);
	//scanf("%d",&N);
	//printf("Jaka ma być liczebność populacji(P)?\nP=");
	//scanf("%d",&P);
	float prawdopodobienstwo_krzyzowania=0.8;
	float prawdopodobienstwo_mutacji=0.1;
	int liczba_pokolen, liczba_pokolen_na_pozniej;
	liczba_pokolen=liczba_pokolen_na_pozniej=10000;
	int i,j, suma_rankingow=0, liczba=0, ranking, gdzie_krzyzowac, temp, ktorego_mutowac, nowa_wartosc;
	int ** populacja=NULL;
	int ** dzieci=NULL;
	int * the_best=NULL;
	if((najlepsze=fopen("najlepsze", "w"))==NULL)
    	printf("Nie mogę otworzyć pliku \"najlepsze\"!\n");
	if((srednie=fopen("srednie", "w"))==NULL)
    	printf("Nie mogę otworzyć pliku \"srednie\"!\n");
	///////////////////////////////////////////////////////TWORZENIE PIERWOTNEJ POPULACJI
	populacja=(int**)malloc(P*sizeof(int*));
	for(i=0; i<P; i++)
	{
		populacja[i]=(int*)malloc((N+1)*sizeof(int)); //dodatkowy rząd utworzony dla wartości funkcji ff i-tego wiersza
		for(j=0; j<N;j++)
			populacja[i][j]=rand()%N;
	}
	the_best=(int*)malloc(N*sizeof(int));
	for(i=0; i<N; i++)
		*(the_best+i)=0;
	///////////////////////////////////////////////////////PĘTLA
	while(decyzja(populacja, the_best, P, N) && liczba_pokolen)
	{
		///////////////////////////////////////////////////////RANKING I NOWA POPULACJA
		dzieci=(int**)malloc(P*sizeof(int*));
		for(i=0; i<P; i++)
			dzieci[i]=(int*)malloc((N+1)*sizeof(int));//dodatkowy rząd utworzony dla wartości funkcji ff i-tego wiersza
		suma_rankingow=0;
		liczba=0;
		for(i=1; i<=P; i++)
			suma_rankingow+=i;
		for(i=0; i<P; i++)
		{
			liczba=rand()%suma_rankingow+1;
			ranking=P;
			while((liczba-=ranking)>0) ranking--;//ustalenie który osobnik populacji trafi do nowej do populacji
			for(j=0; j<N; j++)//tworzenie populacji potomnej 
				dzieci[i][j]=populacja[P-ranking][j];
			dzieci[i][N]=0;
		}
		for(i=0; i<P; i++)
			free(populacja[i]);
		free(populacja);
		populacja=NULL;
		///////////////////////////////////////////////////////KRZYŻOWANIE JEDNOPUNKTOWE
		for(i=0; i<P-1; i+=2)
		{
			if(tak_czy_nie(prawdopodobienstwo_krzyzowania))
			{//krzyżowanie conajwyżej N-1 genów. Gdbyby skrzyżować wszystkie to byłaby zwykła zamiana miejscami chromosomów (chromosom zawiera N genów)
				gdzie_krzyzowac=rand()%(N-1)+1;
				for(j=1; j<N; j++)
					if(j>=gdzie_krzyzowac)
					{
						temp=dzieci[i][j];
						dzieci[i][j]=dzieci[i+1][j];
						dzieci[i+1][j]=temp;
					}
			}
		}
		///////////////////////////////////////////////////////MUTACJA JEDNOPUNKTOWA
		for(i=0; i<P; i++)
		{
			if(tak_czy_nie(prawdopodobienstwo_mutacji))
			{
				ktorego_mutowac=rand()%N;
				while((nowa_wartosc=rand()%N)==dzieci[i][ktorego_mutowac]) ;
				dzieci[i][ktorego_mutowac]=nowa_wartosc;
			}
		}
		populacja=dzieci;
		dzieci=NULL;
		liczba_pokolen--; 
	}
	if(ile_kolizji(the_best, N))
	{
		printf("Nie udało się znaleźć rozwiązania bez wzajemnego bicia się hetmanów. Najlepsze znalezione rozwiązanie z %d biciem/ami to:\n", ile_kolizji(the_best, N));
		for(i=0; i<N; i++)
			printf("x=%2d, y=%2d\n", i, the_best[i]);
	}
	else
	{
		printf("Udało się znaleźć ułożenie niebijących się hetmanów w %d pokoleniu:\n", liczba_pokolen_na_pozniej-liczba_pokolen+1);
		for(i=0; i<N; i++)
			printf("x=%2d, y=%2d\n", i, the_best[i]);
	}
	fclose(najlepsze);
	fclose(srednie);
	for(i=0; i<P; i++)
		free(populacja[i]);
	free(populacja);
	populacja=NULL;
	free(the_best);
	the_best=NULL;
}

int decyzja(int **populacja, int *the_best, int P, int N)
{
	float suma=0;
	int i;
	///////////////////////////////////////////////////////OCENA(WYNIK)
	for(i=0; i<P; i++)
	{
		populacja[i][N]= ile_kolizji(populacja[i], N); //wstawianie ilości kolizji do ostatniego rzędu
		suma+=(float)populacja[i][N];
	}
	///////////////////////////////////////////////////////SORTOWANIE I DANE WYJŚCIOWE
	sortowanie(populacja, P, N);
	if(ile_kolizji(populacja[0], N)<ile_kolizji(the_best, N))
		for(i=0; i<N; i++)
			the_best[i]=populacja[0][i];
	fprintf(srednie,"%.4f\n", suma/P);
	fprintf(najlepsze,"%d\n", populacja[0][N]);
	return ile_kolizji(populacja[0], N);
}
		   
int ile_kolizji(int tab[], int n)
{
	int i,j,kolizje=0;
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
		{
			if(i==j) j++;
			if(j==n) break;
			if(tab[i] == *(tab+j)) kolizje++;
			else if(((i-j) == (tab[i]-tab[j])) || ((j-i) == (tab[i]-tab[j]))) kolizje++;
		}
	return kolizje/2;
}

void sortowanie(int **tab, int P, int N)
{
    int i, j, licznik;
	int*tmp;
    for(i=0; i<P-1; i++)
    {
        licznik=0;
        for(j=0; j<P-1-i; j++)
            if(tab[j][N]>tab[j+1][N])
            {
                licznik++;
                tmp=tab[j];
                tab[j]=tab[j+1];
                tab[j+1]=tmp;
            }
        if(licznik==0) break;
    }	
}

int tak_czy_nie(float prawdopodobienstwo) //podaje się liczbę z przedziału od 0 do 1. Zwraca 0 lub 1 w
{
	return (rand()%100)<(int)(prawdopodobienstwo*100);
}


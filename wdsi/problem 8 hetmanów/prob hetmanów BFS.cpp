#include <iostream>
#include <list>
#include <vector>
using namespace std;
/////////////////////////////////////////////////TPozycja
class TPozycja
{
	public:
	short x;
	short y;
	TPozycja(short xx, short yy) {x=xx; y=yy;}
	~TPozycja() {}
};
/////////////////////////////////////////////////TSzachownica
class TSzachownica
{
	bool **atak=nullptr;
	vector <TPozycja> vect_hetm;
	public:
	static short n;
	TSzachownica();//Tworzy obiekt i tylko wypełnia zmienną atak zerami (nxn).
	~TSzachownica();
	void wypelnij_atak(short x, short y);//argumenty to pole na którym stoi hetman. Funkcja tworzy obiekt i wpisuje do zmiennej atak jedynkę w pola atakowane przez hetmana na szachownicy
	TSzachownica* alternatywa(TSzachownica &biezacy);//tworzy siatke pol atakowanych przez hetmanów znajdujacych się na pozycjach z vektora i z argumentu funkcji
	short ile_hetm() {return vect_hetm.size();}//zwraca ilość hetmanów w wektorze vect_hetm
	bool wartosc_ataku(short x, short y) {return atak[x][y];}//zwraca wartosc ze zmiennej atak[x][y]
	void pokaz();
	TSzachownica& operator=(const TSzachownica &wzor);
};
/////////////////////////////////////////////////TSzachownica funkcje
TSzachownica::TSzachownica()
{
	atak=new bool*[n];
	for(short i=0; i<n; i++)
	{
		atak[i]=new bool[n];
	}
	for(short i=0; i<n; i++)
	{
		for(short j=0; j<n; j++)
		{
			atak[i][j]=0;
		}
	}
}

TSzachownica::~TSzachownica()
{
	for(short i=0; i<n; i++)
		{delete[] atak[i];}
	delete [] atak;
	vect_hetm.clear();
}

void TSzachownica::wypelnij_atak(short x, short y)
{
	vect_hetm.push_back(TPozycja(x,y));
	for(short i=0; i<n; i++)
	{
		for(short j=0; j<n; j++)
		{
			if(i==x) atak[i][j]=1;
			else if(j==y) atak[i][j]=1;
			else if(((i-x)==(j-y))||((x-i)==(j-y))) atak[i][j]=1;
			else atak[i][j]=0;
		}
	}
}

TSzachownica* TSzachownica::alternatywa(TSzachownica &biezacy)
{
	TSzachownica *temp =new TSzachownica;
	temp->vect_hetm=vect_hetm;
	temp->vect_hetm.push_back(biezacy.vect_hetm[0]);
	for(short i=0; i<n; i++)
	{
		for(short j=0; j<n; j++)
		{
			temp->atak[i][j]=(atak[i][j] || biezacy.atak[i][j]);
		}
	}
	return temp;
}

void TSzachownica:: pokaz()
{
	cout<<"Rozmieszczenie hetmanów na szachownicy:\n";
	
	for(vector<TPozycja>::iterator it=vect_hetm.begin(); it!=vect_hetm.end(); it++)
	{
		cout<<"x:"<<it->x<<", y:"<<it->y<<endl;
	}
	cout<<endl<<endl;
}

TSzachownica& TSzachownica::operator=(const TSzachownica &wzor)
{
	if(&wzor!=this)
	{
		for(short i=0; i<n; i++)
			for(short j=0; j<n; j++)
				atak[i][j]=wzor.atak[i][j];
		vect_hetm=wzor.vect_hetm;
	}
	return *this;
}

/////////////////////////////////////////////////TSzachownica koniec

short TSzachownica::n;

int main()
{
	short N;
	list <TSzachownica*> lista;
  cout<< "Szachownica ma wymiar NxN\nIle ma wynosić N?\nN=";
  cin >> N;
  TSzachownica::n=N;
	TSzachownica zerowa, weryfikacja;
	TSzachownica atakowane_pola[N][N];
	int licznik1=0, licznik2=0;
	for(short i=0; i<N; i++)
	{
		for(short j=0; j<N; j++)
		{
			atakowane_pola[i][j].wypelnij_atak(i,j);
		}
	}
	for(short i=0; i<N; i++)
	{
		for(short j=0; j<N; j++)
		{
			lista.push_back(zerowa.alternatywa(atakowane_pola[i][j]));
			while(!lista.empty())
			{
				licznik1++;
				weryfikacja=(*lista.front());
				if(N==weryfikacja.ile_hetm())
				{
					licznik2++;
					cout<<"Rozmieszczenie "<<N<<" hetmanów na szachownicy znalezione przy "<<licznik1<<" weryfikacji(rozwiązanie nr "<<licznik2<<"):\n";
					lista.front()->pokaz();
					delete lista.front();
					lista.pop_front();
					return 0;
				}
				else
				{
					delete lista.front();
					lista.pop_front();
					for(short k=0; k<N; k++)
					{
						for(short l=0; l<N; l++)
						{
							if(weryfikacja.wartosc_ataku(k,l)==0) lista.push_back(weryfikacja.alternatywa(atakowane_pola[k][l]));
						}
					}
				}
			}
		}
	}
	if(!licznik2) cout<<"Na szachownicy o wymiarze "<<N<<"x"<<N<<" nie można ustawić "<<N<<" hetmanów tak, żeby nie atakowały się.\n";
	else cout<<"Rozmieszczenie "<<N<<" hetmanów na szachownicy, wyniki: "<<licznik1<<" weryfikacji, "<<licznik2<<" rozwiązań.\n";
	return 0;
}

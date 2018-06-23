#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <tchar.h>
#include <strsafe.h>

#define DUZ 60
#define VIS 40

#define UP     '8'
#define DOWN   '5'
#define LEFT   '4'
#define RIGHT  '6'
#define POC_DUZ 4
//ZMIJA v2.9.6

void T(char c, COORD S)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD Written;

	FillConsoleOutputCharacter(hOut, c, 1, S, &Written);
}
/*
void Wait1(int sec)
{
	int i = 0;

	for (i=0; i<sec*10000000; i++);
}
*/
void Wait(int sec)
{
	int i = 0;
	clock_t t, u = 0;

	t = clock();
	while (u<(sec*24))
	{
		t = clock();
		i++;
		u += clock()-t;
	}
}

void Font(HANDLE outcon)
{
	CONSOLE_FONT_INFOEX current = {0};
	COORD FontSize = {8,8};

		//GetCurrentConsoleFontEx(outcon, 0, &current);

	current.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		//current.nFont = 0;
	current.dwFontSize = FontSize;
	current.FontWeight = FW_NORMAL;
	current.FontFamily = FF_DONTCARE;

	SetCurrentConsoleFontEx(outcon, 0, &current);
}

void Podesavanje_prozora()
{
	TCHAR szNewTitle[MAX_PATH];
    HWND wh = GetConsoleWindow();
	HANDLE outHndl = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dim = {DUZ,VIS};
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT r;
	/* Font */
	Font(outHndl);
	/* Velicina i pozicija */
	GetConsoleScreenBufferInfo(outHndl, &csbi);

	r.Left   = 0;
    r.Top    = 0;
    r.Right  = DUZ-1;
    r.Bottom = VIS-1;
	SetConsoleWindowInfo(outHndl, TRUE, &r);

	SetConsoleScreenBufferSize(outHndl, dim);
	MoveWindow(wh,DUZ*6, VIS*2, DUZ*15, VIS*15, TRUE);
	/* Naslov */
	StringCchPrintf(szNewTitle, MAX_PATH, TEXT("ZMIJA %s"), "");
	SetConsoleTitle(szNewTitle);
}

void Prazan_okvir()
{
	int i,j;

	printf("\n");
	putchar(201);
	for (i=1; i<=DUZ-2; i++) putchar(205);
	putchar(187);
	for (i=1; i<=VIS-3; i++)
	{
		putchar(186);
		for (j=1; j<=DUZ-2; j++) putchar(' ');
		putchar(186);
	}
	putchar(200);
	for (i=1; i<=DUZ-2; i++) putchar(205);
	putchar(188);
}

void Zvuk_ispis(short z)
{
	COORD m = {33+8,VIS-1};
	char b;

	if (z==0)
	{
		T('O',m);	
		m.X++;
		T('f',m);	
		m.X++;
		T('f',m);
	}
	else 
	{
		T('O',m);	
		m.X++;
		T('n',m);	
		m.X++;
		T(' ',m);
	}
}

void Kontrole()
{
	char o;
	int m,i;

	if (DUZ>=33+11)
	{
		o = UP;
		printf("%c-%c ",30,o);
		o = DOWN;
		printf("%c-%c ",31,o);
		o = LEFT;
		printf("%c-%c ",17,o);
		o = RIGHT;
		printf("%c-%c ",16,o);
		printf("P-pauza ");
		printf("Esc-meni ");
		printf("Z-zvuk: ");
		/* Skor */
		m = DUZ-33-8-12;
		for (i=0; i<m; i++)
		{
			printf(" ");
		}
		printf("Skor: 00000");
	}
	else 
	{
		if(DUZ>=12)
		{
			m = DUZ-12;
			for (i=0; i<m; i++)
			{
				printf(" ");
			}
			printf("Skor: 00000");
		}

	}
}

void Skor_ispis(int S)
{
	COORD m = {DUZ-1,VIS-1};
	char b;
	int i;
	
	for (i=1; i<=10000; i=i*10)
	{
		m.X--;
		b = '0'+(S%(i*10))/i;
		T(b,m);
	}
}

COORD Hrana(COORD gl, COORD glb, COORD *P, int D)
{
	int i,j;
	COORD h;

	do
	{
		j=0;
		do
		{
			h.X = (short) ((rand()/((float)RAND_MAX))*(DUZ-2)+1);
			h.Y = (short) ((rand()/((float)RAND_MAX))*(VIS-3)+1);
		}
		while (h.Y==gl.Y && h.X==gl.X && h.Y==glb.Y && h.X==glb.X );
	
		for (i=0; i<D; i++)
		{
			if (h.X==P[i].X && h.Y==P[i].Y) j=1;
		}
	}
	while(j==1);
	return h;
}
/*
void Sudar(short zvuk,int l, int duzina, COORD spot, COORD oS, COORD p0, short O, short P)
{
	Wait(1);
	if (zvuk==1) printf("\a\a");
	T('X',spot);

	if (O!=0) T('o',oS);
	else
	{
		if (l!=duzina) T('o',oS);
	}

	if (P!=0)
	{
		if (l==duzina) T(' ',p0);
	}

	Wait(10);
}
*/

void Jezik(char sm, COORD gl)
{
		switch (sm)
			{
				case 'r': gl.X++; T('-',gl); gl.X++; T('-',gl); gl.X++; T('-',gl); gl.X++; T('<',gl); break;
				case 'l': gl.X--; T('-',gl); gl.X--; T('-',gl); gl.X--; T('-',gl); gl.X--; T('>',gl); break;
				case 'u': gl.Y--; T('|',gl); gl.Y--; T('|',gl); gl.Y--; T('|',gl); gl.Y--; T('v',gl); break;
				case 'd': gl.Y++; T('|',gl); gl.Y++; T('|',gl); gl.Y++; T('|',gl); gl.Y++; T('^',gl); break;
			}
}

void main()
{
	int i, duzina, l, nivo, Skor;
	char smer, G;
	short pauza, zvuk = 0;
	char c, z[10];
	COORD spot, oS, start, *p, pp, ps, H;
	COORD xy = {33+8,VIS-1};
	
	Podesavanje_prozora();

	/* Meni */
M:	printf("\n\nIzaberi nivo:\n");
	printf("1 - sa prolazom kroz zidove\n");
	printf("2 - bez prolaza kroz zidove\n");
	printf("0 - izlaz\n");
	nivo = 0;
	while(nivo==0)
	{
		scanf("%s",&z);
		if (strlen(z)!=1)
		{
			printf("**Odabir nije validan\a\n");
			continue;
		}
		else
		{
			if (z[0]!='1' && z[0]!='2' && z[0]!='0')
			{
				printf("**Odabir nije validan\a\n");
				continue;
			}
			else nivo = atoi(z);
			if (z[0]=='0') exit(1);
		}
	}
	//////////
POC:Prazan_okvir();
	Kontrole();
	Zvuk_ispis(zvuk);
	srand(time(NULL));
	start.X = DUZ/2; 
	start.Y = VIS/2;
	spot = start;
	oS = spot;
	oS.X -= 1;
	pauza = 0;
	Skor = 0;
	l = 1;
	duzina = POC_DUZ;
	smer = 'r';
	G = 16;
	H.X = 0; H.Y = 0;
	p = calloc((DUZ-2)*(VIS-3),sizeof(COORD));
	if (p==NULL) exit(1);

	for (;;)
	{
		if (pauza==0) 
		{
			if (H.X==0 && H.Y==0)
			{
				H = Hrana(spot,oS,p,duzina);
				T('x',H);
				/* Sudar sa novim delom repa */
				if (p[0].X==spot.X && p[0].Y==spot.Y)
				{
					Wait(1);
					if (zvuk==1) printf("\a\a");
					T('X',spot);
					T('o',oS);
					Wait(10);
					goto POC;
				}
				///////////////////////////////
			}
			else T('x',H);
			/* Sudar pri stvaranju */
			if (spot.X+1==start.X && spot.Y==start.Y)
			{
				Wait(1);
				if (zvuk==1) printf("\a\a");
				T('X',spot);
				if (l!=duzina) T('o',oS);
				Wait(10);
				goto POC;
			}
			//////////////////////////
			T('o',oS);
			T(G,spot);
			pp = oS;
			if (l==duzina)
			{
				for (i=l-2; i>=0; i--)
				{
					ps = pp;
					pp = p[i];
					p[i] = ps;
				}
				if (pp.X!=spot.X || pp.Y!=spot.Y) T(' ',pp);
				start.X = 0; start.Y = 0;
			}
			else
			{
				p[l-1] = oS;
				l++;
			}
			switch (smer)
			{
				case 'r':	spot.X += 1;
							oS = spot;
							oS.X -= 1;
							break;
				case 'l':	spot.X -= 1;
							oS = spot;
							oS.X += 1;
							break;
				case 'd':	spot.Y += 1;
							oS = spot;
							oS.Y -= 1;
							break;
				case 'u':	spot.Y -= 1;
							oS = spot;
							oS.Y += 1;
							break;
			}
		}
		Wait(1.7);
		if (kbhit())
		{
			c = getch();
			switch (c)
			{
				case LEFT:  if (smer!='r') {smer='l'; G=17;} pauza = 0; break;
				case RIGHT: if (smer!='l') {smer='r'; G=16;} pauza = 0; break;
				case UP:	if (smer!='d') {smer='u'; G=30;} pauza = 0; break;
				case DOWN:  if (smer!='u') {smer='d'; G=31;} pauza = 0; break;
				case ' ': Jezik(smer, spot); pauza = 0; break;
				case 'P':
				case 'p': pauza = !pauza; break;
				case 'Z':
				case 'z': zvuk = !zvuk; Zvuk_ispis(zvuk); break;
				case 27: goto M;
				default: break;
			}
		}
		switch (nivo)
		{
			case 1:
				/* Prolazak kroz zid */
				if (spot.X==0)
				{
					spot.X = DUZ-2;
					oS = spot;
					oS.X = 1;
				}
				if (spot.X==DUZ-1)
				{
					spot.X = 1;
					oS = spot;
					oS.X = DUZ-2;
				}
				if (spot.Y==0)
				{
					spot.Y = VIS-3;
					oS = spot;
					oS.Y = 1;
				}
				if (spot.Y==VIS-2)
				{
					spot.Y = 1;
					oS = spot;
					oS.Y = VIS-3;
				}
				break;
			case 2:
				/* Sudar sa zidom */
				if (spot.X==0 || spot.X==DUZ-1 || spot.Y==0 || spot.Y==VIS-2)
				{
					Wait(1);
					if (zvuk==1) printf("\a\a");
					T('X',spot);
					T('o',oS);
					if (l==duzina) T(' ',p[0]);
					Wait(10);
					goto POC;
				}
				break;
		}

		/* Sudar sa zmijom */
		for (i=1; i<l; i++)
		{
			if (spot.X==p[i].X && spot.Y==p[i].Y)
			{
				Wait(1);
				if (zvuk==1) printf("\a\a");
				T('X',spot);
				T('o',oS);
				if (l==duzina) T(' ',p[0]);
				Wait(10);
				goto POC;
			}
		}
		/* Has */
		if (oS.X==H.X && oS.Y==H.Y)
		{
			if (zvuk==1) printf("\a");
			H.X = 0;
			H.Y = 0;
			duzina++;
			Skor++;
			Skor_ispis(Skor);
		}
	}
}
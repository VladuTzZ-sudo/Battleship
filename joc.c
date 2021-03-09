#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define ARRAY_SIZE(a)(sizeof(a) / sizeof(a[0]))
#define BUFMAX 25

WINDOW* create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void patrate(WINDOW *joc, WINDOW *pc);
void init_wins(int a[][12], WINDOW *nave[], WINDOW *joc, WINDOW *pc, int m[][11], int row, int col, int c[][12]);
void matrice(int m[][11], int b[][11]);

void cmatrice(int a[][12], int c[][12], int row, int col, WINDOW *nave[]);
void creare_glont(WINDOW *glont[], int b[][11], int contorjoc, int li, int coc, int row, int col);
void matrice2(int a[][12], int c[][12]);
void matrice3(int dublajoc1[][11], int dublajoc[][11]);
void c2matrice(int a[][11], int c[][11], int row, int col, WINDOW *nave[], int v[5]);

int main()
{
	int ch, choice;
	int i, j, o;
	int row, col;
	FILE *pFile, *Pjoc, *Ppc;
	char *SALVARE;
	char buffer[BUFMAX];
//	pFile = fopen("/home/vlad/Desktop/JOC/text.txt", "r");
	pFile = fopen("./text.txt", "r");

	char *p;
	int b[11][11], m[11][11];
	int c[12][12] = { 0 };
	int a[12][12];
	j = 0;
	i = 0;
	int z;
	z=0;
	//CITIRE DIN FISIER HARTA JUCATORULUI
	while (fgets(buffer, BUFMAX, pFile) != NULL)
	{
		z=1;
		i++;
		j++;
		for (o = 1; o <= strlen(buffer); o++)
		{
			if (buffer[o] == ' ')
			{
				b[j][i] = 0;
				i++;
			}
			if (buffer[o] == 'x')
			{
				b[j][i] = 1;
				i++;
			}
		}
		i = 0;
	}
	fclose(pFile);
	//IN CAZ CA FISIERUL NU POATE FI DESCHIS - FUNCTIA MAIN RETURNEAZA 1 si afiseaza - eroarea.
	if ( z == 0 ){
		printf ("[Eroare]: Fisierul %s nu poate fi deschis.", "text.txt");
		return 1;
	}

	//INITIALIZARE NCURSES SI AFISARE MENIU
	char list[3][10] = { "New Game",
		"Resume",
		"Quit" };

	char item[10];
	WINDOW *w, *joc, *pc, *final, *nave[80], *glont[30], *random, *kill, *rj, *rp;
	initscr();
	cbreak();	//nu se pastreaza linia in buffer
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_CYAN);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);
	init_pair(7, COLOR_RED, COLOR_BLACK);
	init_pair(9, COLOR_YELLOW, COLOR_YELLOW);
	raw();
	keypad(stdscr, TRUE);
	noecho();

	getmaxyx(stdscr, row, col);
	char str[80] = "BATTLESHIP";
	char str2[80] = "Apasa D/R pentru a activa modul Destroy/Random!!!";
	mvprintw(1, (col - strlen(str)) / 2, str);
	refresh();

	mousemask(ALL_MOUSE_EVENTS, NULL);
	curs_set(0);
	refresh();

	w = newwin(10, 30, row / 4, (col - 30) / 2);
	wborder(w, '>', '<', '^', 'v', '^', '^', 'v', 'v');

	for (i = 0; i < 3; i++)
	{
		if (i == 0)
			wattron(w, A_STANDOUT);	// HIGHLIGHTED 
		else
			wattroff(w, A_STANDOUT);
		sprintf(item, "%-8s", list[i]);
		mvwprintw(w, i + 3, 11, "%s", item);
	}
	wrefresh(w);

	noecho();
	keypad(w, TRUE);
	curs_set(0);
	mousemask(ALL_MOUSE_EVENTS, NULL);
	int ok;
	int k1 = 15;
	int k2 = 19;
	int k3 = 22;
	int k4 = 24;
	i = 0;
	ok = 3;
	int tt, ttt, k10;
	tt = 0;
	ttt = 0;

	//MARK-ul din meniu
	mvwaddch(w, i + 3, 9, '@');

	//ATUNCI CAND SE PASA QUIT - OK devine 0 si programul se termina
	while (ok != 0) 
	{
		//ok == 3 -> INSEAMNA INITIALIZARE DE NEW GAME
		//Ok == 1 -> RESUME
		if (ok == 2 || ok == 1)
		{
			//Salveaaza ecranul inainte de a-l sterge cu clrtoboot();
			scr_dump("/tmp/scr.dump");
			move(0, 0);
			clrtobot();
			getmaxyx(stdscr, row, col);

			char str[80] = "BATTLESHIP";
			char str2[80] = "Apasa D/R pentru a activa modul Destroy/Random!!!";
			mvprintw(4, (col - strlen(str)) / 2, str);
			refresh();

			getmaxyx(stdscr, row, col);
			mousemask(ALL_MOUSE_EVENTS, NULL);
			curs_set(0);
			refresh();

			//CREAZA DIN NOU MENIUL
			w = newwin(10, 30, row / 4, (col - 30) / 2);
			wborder(w, '^', '^', 'V', '^', 'V', 'V', '^', '^');
			keypad(w, TRUE);
			keypad(stdscr, TRUE);
			refresh();
			for (i = 0; i < 3; i++)
			{
				if (i == 0)
					wattron(w, A_STANDOUT);	// highlights the first item.
				else
					wattroff(w, A_STANDOUT);
				sprintf(item, "%-8s", list[i]);
				mvwaddch(w, 3, 9, '@');
				wrefresh(w);
				mvwprintw(w, i + 3, 11, "%s", item);
				refresh();
			}
			noecho();
			wrefresh(w);
			refresh();
			i = 0;
		}
		keypad(w, TRUE);
		while (ch = wgetch(w))
		{
			sprintf(item, "%-8s", list[i]);
			mvwprintw(w, i + 3, 11, "%s", item);
			tt = 0;
			//IN FUNCTIE DE TASTA APASATA SE DEPLASEAZA IN MENIU , STERGE MARK ul ANTERIOR SI IL AFISEAZA PE CEL CURENT
			switch (ch)
			{
				case KEY_UP:
					i--;
					i = (i < 0) ? 2 : i;
					wattron(w, A_STANDOUT);
					sprintf(item, "%-8s", list[i]);
					mvwprintw(w, i + 3, 11, "%s", item);
					wattroff(w, A_STANDOUT);
					mvwaddch(w, 3, 9, ' ');
					mvwaddch(w, 4, 9, ' ');
					mvwaddch(w, 5, 9, ' ');
					mvwaddch(w, 6, 9, ' ');
					mvwaddch(w, 7, 9, ' ');
					mvwaddch(w, 3 + i, 9, '@');
					break;
				case KEY_DOWN:
					i++;
					i = (i > 2) ? 0 : i;
					wattron(w, A_STANDOUT);
					sprintf(item, "%-8s", list[i]);
					mvwprintw(w, i + 3, 11, "%s", item);
					wattroff(w, A_STANDOUT);
					mvwaddch(w, 3, 9, ' ');
					mvwaddch(w, 4, 9, ' ');
					mvwaddch(w, 5, 9, ' ');
					mvwaddch(w, 6, 9, ' ');
					mvwaddch(w, 7, 9, ' ');
					mvwaddch(w, 3 + i, 9, '@');

					break;
				case 10:
					{
						wattron(w, A_STANDOUT);
						sprintf(item, "%-8s", list[i]);
						mvwprintw(w, i + 3, 11, "%s", item);
						wattroff(w, A_STANDOUT);

						char mama[89];
						strcpy(mama, list[i]);
						tt = 1;
						if (strcmp(mama, "New Game") == 0)
						{
							ok = 2;
							ttt = 1;
							//DACA TTT=1 -> SE ACTIVEAZA BUTONUL RESUME
						}

						if (strcmp(mama, "Resume") == 0)
						{
							if (ttt == 1)
							{
								ok = 1;
							}
						}
						if (strcmp(mama, "Quit") == 0)
						{
							ok = 0;
						}

						refresh();
						break;
					}
					break;
			}
			wrefresh(w);
			// DACA TT=1 -> S-A FOLOSIT MENIUL DECI PUTEM INCHIDE WHILE UL
			if (tt == 1)
			{
				break;
			}
		}

		if (ok == 2 || ok == 1)
		{
			// ATAT PENTRU MODUL NEW GAME CAT SI PENTRU RESUME EXISTA PARTI COMUNE PRECUM INITIALIZAREA VARIABILELOR
			int x, y;
			int li, coc;
			int contorjoc;
			int dublajoc[11][11];
			int dublapc[11][11];
			int linie, coloana, io, j;
			int ok3, ok4, ok6;

			MEVENT event;
			destroy_win(w);
			move(0, 0);
			clrtobot();
			refresh();

			getmaxyx(stdscr, row, col);
			mvprintw(4, (col - strlen(str)) / 2, str);
			mvprintw(row - 3, (col - strlen(str2)) / 2, str2);
			if (ok == 2)
			{
				//PENTRU MODUL NEW GAME CREEZ CELE DOUA FERESTRE DE JOC - joc->a jucatorului - pc-> a calculatorului SI RJ SI RP - CE ARATA CAND ESTE RANDUL JUCATORULUI SI RANDUL CALCULATORULUI
				choice = 0;
				joc = create_newwin(21, 41, row / 3, (col - 41) / 7);
				rj = create_newwin(2, 41, row / 3 - 2, (col - 41) / 7);
				pc = create_newwin(21, 41, row / 3, 6 *(col - 41) / 7);
				rp = create_newwin(2, 41, row / 3 - 2, 6 *(col - 41) / 7);

				wbkgd(rj, COLOR_PAIR(9));
				wbkgd(rp, COLOR_PAIR(9));
				wbkgd(rp, COLOR_PAIR(5));

				mousemask(ALL_MOUSE_EVENTS, NULL);
				refresh();
				wrefresh(joc);
				wrefresh(pc);
				wrefresh(rj);
				wrefresh(rp);
				wbkgd(pc, COLOR_PAIR(2));
				wbkgd(joc, COLOR_PAIR(3));
				
				//ACEASTA FUNCTIE PRINTEAZA IN FERESTERELE DE JOC PATRATELELE - TABLA DE JOC	
				patrate(joc, pc);
			        // SALVEAZA MATRICEA PRINTR O COPIE
				matrice(m, b);
	
				//IN ACEASTA FUNCTIE SE VA CREA IMAGINEA DIN SPATELE FERESTREI DE JOC A CALCULATORULUI - SE VOR AFLA NAVELE ASEZATE ALEATOR
				init_wins(a, nave, joc, pc, m, row, col, c);

				wrefresh(joc);
				wrefresh(pc);
				refresh();
				for (i = 0; i <= 10; i++)
				{
					for (j = 0; j <= 10; j++)
					{
						dublajoc[i][j] = 0;
						dublapc[i][j] = 0;
					}
				}
			}

			if (ok == 1)
			{
				//IN MODUL RESUME SE RESTABILESTE ECRANUL 
				scr_restore("/tmp/scr.dump");
				refresh();
			}

			//RANDOM - FEREASTRA BUTONULUI random CARE POATE FI ACTIVATA APASAND TASTA R - ACTIVEAZA MODUL RANDOM
			random = newwin(6, 20, 6, (col - 47) / 2);	//JOS
			wborder(random, '>', '<', '^', 'v', '^', '^', 'v', 'v');
			wrefresh(random);
			kill = newwin(6, 20, 6, (col - 47) / 2 + 25);	//SUS
			
			//KILL - FEREASTRA MODULUI DESTROY
			wborder(kill, '>', '<', '^', 'v', '^', '^', 'v', 'v');
			wrefresh(kill);
			wbkgd(random, COLOR_PAIR(5));
			wbkgd(kill, COLOR_PAIR(6));

			mvwprintw(random, 4, 7, "RANDOM");
			mvwprintw(kill, 4, 7, "DESTROY");
			mousemask(ALL_MOUSE_EVENTS, NULL);
			wrefresh(kill);
			wrefresh(random);

			refresh();
			wrefresh(joc);
			wrefresh(pc);
			k2 = 19;
			choice = 0;
			srand(time(NULL));
			int cj, cp;
			int navajoc,navapc;
			navajoc=0;
			navapc=0;
			cj = 0;
			cp = 0;
			do {

				int k, ok6;
				switch (ch)
				{
					case 1: //NU AM REUSIT SA TERMIN MODUL RANDOM - SI PENTRU CA INCURCA PROGRAMUL AM PUS case 1 si nu case 114
						{
							// SE RESETEAZA IMAGINEA FERESTREI DE JOC - joc 
							joc = create_newwin(21, 41, row / 3, (col - 41) / 7);
							wbkgd(joc, COLOR_PAIR(3));
							getmaxyx(joc, io, coloana);

							// SE ADAUGA PATRATELELE COLORATE
							for (i = 1; i <= 9; i++)
							{
								mvwaddch(joc, i *2, 0, ACS_LTEE);
								mvwhline(joc, i *2, 1, ACS_HLINE, coloana - 1);
								mvwaddch(joc, i *2, coloana - 1, ACS_RTEE);
								wrefresh(joc);
							}

							for (j = 1; j <= 9; j++)
							{
								mvwvline(joc, 1, j *4, ACS_VLINE, io - 2);
								wrefresh(joc);
							}

							// ATUNCI CAND O NAVA ESTE DISTRUSA - MATRICEA DUBLAJOC[10][10] (- CONTINE FORMATAUL FERESTREI JOC -) ESTE 3 IN DREPTUL CASUTEI CARE FACE PARTE
							// DINTR-O NAVA DOBORATA.
							
							//URMATOAREA SECVENTA CITESTE MATRICEA SI IDENTIFICA NAVELE DOBORATE SI LE PRINTEAZAZ PE TABLA DE JOC CU O CULOARE DIFERITA- ACESTEA RAMAN FIXE
							int poz;	//0 - orizontal 1-vertical
							int dir;	//0 - dreapta 1-stanga || 0 - sus 1 - jos
							int cat, directie;	// dimensiunea
							int steag;
							int v[5];
							v[1] = 0;
							v[2] = 0;
							v[3] = 0;
							v[4] = 0;
							int j2, i2;
							k1 = 0;
							k2 = 4;
							k3 = 7;
							k4 = 9;
							int dublajoc1[11][11], joc3[11][11];
							matrice3(dublajoc1, dublajoc);
							for (i = 1; i <= 10; i++)
							{
								for (j = 1; j <= 10; j++)
								{
									li = i;
									coc = j;
									steag = 0;
									if (dublajoc1[i][j] == 3)
									{
										if (dublajoc1[i][j + 1] == 3)
										{
											poz = 0;
										}
										else
										{
											if (dublajoc1[i + 1][j] == 3)
											{
												poz = 1;
											}
											else
											{
												if (dublajoc1[i - 1][j] != 3)
												{
												 										
													//AM GASIT UN PATRAT DE 1
													v[1]++;
													steag = 1;
												
													nave[k1] = create_newwin(3, 5, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(coc - 1));
													wbkgd(nave[k1], COLOR_PAIR(7));
												
													wrefresh(nave[k1]);
													k1++;
												}
											}
										}
									}
									if (steag != 1)
									{
										if (poz == 0)
										{
											cat = 0;
											j2 = j;
											
											while (dublajoc1[i][j] == 3)
											{
												j++;
												cat++;
											}
											switch (cat)
											{
												case 2:
													nave[k2] = create_newwin(3, 9, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(j2 - 1));
													wbkgd(nave[k2], COLOR_PAIR(7));
													
													wrefresh(nave[k2]);
													v[2]++;
													k2++;
													break;
												case 3:
													nave[k3] = create_newwin(3, 13, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(j2 - 1));
													wbkgd(nave[k3], COLOR_PAIR(7));
													
													wrefresh(nave[k3]);
													v[3]++;
													k3++;
													break;
												case 4:
													nave[k4] = create_newwin(3, 17, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(j2 - 1));
													wbkgd(nave[k4], COLOR_PAIR(7));
													
													wrefresh(nave[k4]);
													v[4]++;
													k4++;
													break;
											}
										}
										if (poz == 1)
										{
											cat = 0;
											i2 = i;
											while (dublajoc1[i][j] == 3)
											{
												dublajoc1[i][j] = 0;
												i++;
												cat++;
											}
											switch (cat)
											{
												case 2:
													nave[k2] = create_newwin(5, 5, row / 3 + 2 *(i2 - 1), (col - 41) / 7 + 4 *(coc - 1));
													wbkgd(nave[k2], COLOR_PAIR(7));
													wrefresh(nave[k2]);
													v[2]++;
													k2++;
													break;
												case 3:
													nave[k3] = create_newwin(7, 5, row / 3 + 2 *(i2 - 1), (col - 41) / 7 + 4 *(coc - 1));
													wbkgd(nave[k3], COLOR_PAIR(7));
													wrefresh(nave[k3]);
													v[3]++;
													k3++;
													break;
												case 4:
													nave[k4] = create_newwin(9, 5, row / 3 + 2 *(i2 - 1), (col - 41) / 7 + 4 *(coc - 1));
													wbkgd(nave[k4], COLOR_PAIR(7));
													wrefresh(nave[k4]);
													v[4]++;
													k4++;
													break;
											}
											i = i - cat;
										}
									}
								}
							}

							//MATRICEA ESTE INITIALIZATA CU 0
							for (i = 1; i <= 10; i++)
							{
								for (j = 1; j <= 10; j++)
								{
									joc3[i][j] = 0;
								}
							}

							//COORDONATELE NAVELOR DOBORATE SUNT TRANSMISE NOII MATRICI JOC3 - DIN VALOAREA 3 IN VALOAREA 1 - IN JURUL UNEI NAVE SE PUNE CIFRA 2 PENTRU DELIMITARE
							for (i = 1; i <= 10; i++)
							{
								for (j = 1; j <= 10; j++)
								{
									if (dublajoc[i][j] == 3)
									{
										dublajoc[i][j] = 0;
										b[i][j] = 0;
										joc3[i][j] = 1;
										if (joc3[i - 1][j] != 1)
										{
											joc3[i - 1][j] = 2;
										}
										if (joc3[i][j - 1] != 1)
										{
											joc3[i][j - 1] = 2;
										}
										if (joc3[i + 1][j] != 1)
										{
											joc3[i + 1][j] = 2;
										}
										if (joc3[i][j + 1] != 1)
										{
											joc3[i][j + 1] = 2;
										}
									}
									if (dublajoc[i][j] == 1)
									{
										dublajoc[i][j] = 0;
									}
								}
							}
							//ACESTA FUNCTIE AR TREBUI SA AMESTECE RESTUL NAVELOR
							c2matrice(b, joc3, row, col, nave, v);
						
							//URMATOAREA SECVENTA AR FI TREBUIT SA IDENTIFICE NAVELE NEDOBORATE SI AMESTECATE DIN NOUA MATRICE DE COORDONATE SI SA LE AFISEZE PE TABLA DE JOC
							for (i = 1; i <= 10; i++)
							{
								for (j = 1; j <= 10; j++)
								{
									if (joc3[i][j] == 2)
									{
										joc3[i][j] = 1;
									}
								}
							}
							k1 = 0;
							k2 = 4;
							k3 = 7;
							k4 = 9;
							for (i = 1; i <= 10; i++)
							{
								for (j = 1; j <= 10; j++)
								{
									li = i;
									coc = j;
									steag = 0;
									if (joc3[i][j] == 1)
									{
										if (joc3[i][j + 1] == 1)
										{
											poz = 0;
										}
										else
										{
											if (joc3[i + 1][j] == 1)
											{
												poz = 1;
											}
											else
											{
												if (joc3[i - 1][j] != 1)
												{
													steag = 1;

													nave[k1] = create_newwin(3, 5, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(coc - 1));

													wrefresh(nave[k1]);
													k1++;
												}
											}
										}
									}
									if (steag != 1)
									{
										if (poz == 0)
										{
											cat = 0;
											j2 = j;
											while (joc3[i][j] == 1)
											{
												j++;
												cat++;
											}
											switch (cat)
											{
												case 2:
													nave[k2] = create_newwin(3, 9, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(j2 - 1));
													wrefresh(nave[k2]);
													k2++;
													break;
												case 3:
													nave[k3] = create_newwin(3, 13, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(j2 - 1));
													wrefresh(nave[k3]);
													k3++;
													break;
												case 4:
													nave[k4] = create_newwin(3, 17, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(j2 - 1));
													wrefresh(nave[k4]);
													k4++;
													break;
											}
										}
										if (poz == 1)
										{
											cat = 0;
											i2 = i;
											while (joc3[i][j] == 3)
											{
												joc3[i][j] = 0;
												i++;
												cat++;
											}
											switch (cat)
											{
												case 2:
													nave[k2] = create_newwin(5, 5, row / 3 + 2 *(i2 - 1), (col - 41) / 7 + 4 *(coc - 1));
													wrefresh(nave[k2]);

													k2++;
													break;
												case 3:
													nave[k3] = create_newwin(7, 5, row / 3 + 2 *(i2 - 1), (col - 41) / 7 + 4 *(coc - 1));
													wrefresh(nave[k3]);

													k3++;
													break;
												case 4:
													nave[k4] = create_newwin(9, 5, row / 3 + 2 *(i2 - 1), (col - 41) / 7 + 4 *(coc - 1));
													wrefresh(nave[k4]);

													k4++;
													break;
											}
											i = i - cat;
											v[cat]++;
										}
									}
								}
							}

							wrefresh(joc);
							refresh();
						}
						break;
						//CAZUL DESTROY
					case 100:
						k = 1;
						//PENTRU 10 LOVITURI DE FIECARE JUCATOR
						while (k <= 10)
						{
							int ok6 = 0;
							srand(time(NULL));
							x = rand() % 10 + 1;
							y = rand() % 10 + 1;
							while (dublapc[x][y] == 1)
							{
								x = rand() % 10 + 1;
								y = rand() % 10 + 1;
							}
							//SE CAUTA COORDONATE RANDOM CARE SA NU BIFEZE O CASUTA DEJA BIFATA
							li = x;
							coc = y;
							//DACA SE GASESTE O BUCATA DE NAVA CARE NU A MAI FOST LOVITA SI DACA NU ESTE A 11-A LOVITURA ATUNCI:
							if (a[li][coc] == 1 && dublapc[li][coc] == 0 && k <= 10)
							{
								k++;
								cp++;
								dublapc[li][coc] = 2;
								mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1) + 2, 'X');
								//PRINTAM PE ECRAN NAVA GASITA DACA ACEASTA A FOST DOBORATA CU ULTIMA LOVITURA -GASESC TOATE CAZURILE
								//ACESTE IF-URI ANALIZEAZA DOAR CAZURILE DE NAVE DE 1-2 CASUTE
								if (a[li - 1][coc] != 1 && a[li + 1][coc] != 1 && a[li][coc - 1] != 1 && a[li][coc + 1] != 1)
								{
									nave[k1] = create_newwin(3, 5, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
									k1++;
									ok6 = 1;
									navapc++;
								}
								if ((a[li][coc + 1] == 1 && dublapc[li][coc + 1] == 2 && a[li][coc - 1] != 1 && a[li][coc + 2] != 1))
								{
									nave[k2] = create_newwin(3, 9, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
									k2++;
									mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc) + 2, 'X');
									ok6 = 1;
									navapc++;
								}
								if ((a[li][coc - 1] == 1 && dublapc[li][coc - 1] == 2 && a[li][coc - 2] != 1 && a[li][coc + 1] != 1))
								{
									nave[k2] = create_newwin(3, 9, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 2));
									k2++;
									navapc++;
									ok6 = 1;
									mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 2) + 2, 'X');
								}
								if ((a[li + 1][coc] == 1 && dublapc[li + 1][coc] == 2 && a[li + 2][coc] != 1 && a[li - 1][coc] != 1))
								{
									nave[k2] = create_newwin(5, 5, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
									k2++;
									navapc++;
									ok6 = 1;
									mvwaddch(pc, 2 *(li) + 1, 4 *(coc - 1) + 2, 'X');
								}
								if ((a[li - 1][coc] == 1 && dublapc[li - 1][coc] == 2 && a[li - 2][coc] != 1 && a[li + 1][coc] != 1))
								{
									nave[k2] = create_newwin(5, 5, row / 3 + 2 *(li - 2), 6 *(col - 41) / 7 + 4 *(coc - 1));
									k2++;
									navapc++;
									ok6 = 1;
									mvwaddch(pc, 2 *(li - 2) + 1, 4 *(coc - 1) + 2, 'X');
								}
								//SE CAUTA NAVELE DE 3-4 CASUTE
								if (ok6 != 1)
								{
									i = 0;
									j = 0;
									int ok3 = 1, ok4 = 1;
									while (a[li][coc - i] == 1)
									{
										if (dublapc[li][coc - i] != 2)
										{
											ok3 = 0;
										}
										i++;
									}
									if (ok3 == 1)
									{
										while (a[li][coc + j] == 1)
										{
											if (dublapc[li][coc + j] != 2)
											{
												ok4 = 0;
											}
											j++;
										}
									}
									i = i - 1;
									if (ok4 == 1 && ok3 == 1 && i + j > 2)
									{
										if (i + j == 3)
										{
											navapc++;
											nave[k3] = create_newwin(3, 13, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1 - i));
											k3++;
											//SE AFISEAZA X IN TOATA COMPONENTA NAVEI
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1 - i) + 2, 'X');
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i) + 2, 'X');
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i + 1) + 2, 'X');
										}
										if (i + j == 4)
										{
											navapc++;
											nave[k4] = create_newwin(3, 17, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1 - i));
											k4++;
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1 - i) + 2, 'X');
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i) + 2, 'X');
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i + 1) + 2, 'X');
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i + 2) + 2, 'X');
										}
									}
									i = 0;
									j = 0;
									ok3 = 1, ok4 = 1;
									while (a[li - i][coc] == 1)
									{
										if (dublapc[li - i][coc] != 2)
										{
											ok3 = 0;
										}
										i++;
									}
									if (ok3 == 1)
									{
										while (a[li + j][coc] == 1)
										{
											if (dublapc[li + j][coc] != 2)
											{
												ok4 = 0;
											}
											j++;
										}
									}
									i = i - 1;
									if (ok4 == 1 && ok3 == 1 && i + j + 1 > 2)
									{
										if (i + j == 3)
										{
										 	navapc++;
											nave[k3] = create_newwin(7, 5, row / 3 + 2 *(li - i - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
											k3++;
											mvwaddch(pc, 2 *(li - 1 - i) + 1, 4 *(coc - 1) + 2, 'X');
											mvwaddch(pc, 2 *(li - i) + 1, 4 *(coc - 1) + 2, 'X');
											mvwaddch(pc, 2 *(li - i + 1) + 1, 4 *(coc - 1) + 2, 'X');
										}
										if (i + j == 4)
										{
											navapc++;
											nave[k4] = create_newwin(9, 5, row / 3 + 2 *(li - 1 - i), 6 *(col - 41) / 7 + 4 *(coc - 1));
											k4++;
											mvwaddch(pc, 2 *(li - 1 - i) + 1, 4 *(coc - 1) + 2, 'X');
											mvwaddch(pc, 2 *(li - i) + 1, 4 *(coc - 1) + 2, 'X');
											mvwaddch(pc, 2 *(li - i + 1) + 1, 4 *(coc - 1) + 2, 'X');
											mvwaddch(pc, 2 *(li - i + 2) + 1, 4 *(coc - 1) + 2, 'X');
										}
									}
								}
								refresh();
								wrefresh(pc);
							}
							else
							{
								//DACA ULTIMA TINTA NU A LOVIT O NAVA DAR A FOST O TINTA NOUA SI A FOST INTRE CELE 10 ACCEPTATE ATUNCI SE ACTUALIZEAZA MATRICEA DE COMPONENTE A pc
								if (dublapc[li][coc] != 1 && dublapc[li][coc] != 2 && k <= 10)
								{
									k++;
									dublapc[li][coc] = 1;
									// SE ADAUGA "*" ATUNCI CAND NU SE NIMERESTE
									mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1) + 2, '*');
								}
							}

							refresh();
							wrefresh(pc);
							refresh();
							wrefresh(joc);
							wrefresh(pc);
						}
						k = 1;
						//EXACT ACELASI LUCRU DE INTAMPLA PENTRU FEREASTRA joc - CARE ISI ASTEAPTA RANDUL
						while (k <= 10)
						{
							srand(time(NULL));
							x = (rand() % 53) % 10 + 1;
							y = (rand() % 57) % 10 + 1;
							while (dublajoc[x][y] == 1 || dublajoc[x][y] == 2 || dublajoc[x][y] == 3)
							{
								x = (rand() % 69) % 10 + 1;
								y = (rand() % 89) % 10 + 1;
							}

							
							if (b[x][y] == 1 && dublajoc[x][y] == 0 && k <= 10)
							{
								dublajoc[x][y] = 2;
								cj++;
								k++;
								choice = 1;
								mvwaddch(joc, 2 *(x - 1) + 1, 4 *(y - 1) + 2, 'X');
								creare_glont(glont, b, contorjoc, x, y, row, col);
								wbkgd(glont[contorjoc], COLOR_PAIR(4));
								wrefresh(glont[contorjoc]);
								contorjoc++;
								ok6 = 0;
								li = x;
								coc = y;
								if (b[li - 1][coc] != 1 && b[li + 1][coc] != 1 && b[li][coc - 1] != 1 && b[li][coc + 1] != 1)
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									ok6 = 1;
								}
								if ((b[li][coc + 1] == 1 && dublajoc[li][coc + 1] == 2 && b[li][coc - 1] != 1 && b[li][coc + 2] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li][coc + 1] = 3;
									ok6 = 1;
								}
								if ((b[li][coc - 1] == 1 && dublajoc[li][coc - 1] == 2 && b[li][coc - 2] != 1 && b[li][coc + 1] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li][coc - 1] = 3;
									ok6 = 1;
								}
								if ((b[li + 1][coc] == 1 && dublajoc[li + 1][coc] == 2 && b[li + 2][coc] != 1 && b[li - 1][coc] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li + 1][coc] = 3;
									ok6 = 1;
								}
								if ((b[li - 1][coc] == 1 && dublajoc[li - 1][coc] == 2 && b[li - 2][coc] != 1 && b[li + 1][coc] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li - 1][coc] = 3;
									ok6 = 1;
								}
								if (ok6 != 1)
								{
									i = 0;
									j = 0;
									ok3 = 1, ok4 = 1;
									while (b[li][coc - i] == 1)
									{
										if (dublajoc[li][coc - i] != 2)
										{
											ok3 = 0;
										}
										i++;
									}
									if (ok3 == 1)
									{
										while (b[li][coc + j] == 1)
										{
											if (dublajoc[li][coc + j] != 2)
											{
												ok4 = 0;
											}
											j++;
										}
									}
									i = i - 1;
									if (ok4 == 1 && ok3 == 1 && i + j + 1 > 2)
									{
										if (i + j == 3)
										{
											navajoc++;
											dublajoc[li][coc - i] = 3;
											dublajoc[li][coc - i + 1] = 3;
											dublajoc[li][coc - i + 2] = 3;
										}
										if (i + j == 4)
										{
											navajoc++;
											dublajoc[li][coc - i] = 3;
											dublajoc[li][coc - i + 1] = 3;
											dublajoc[li][coc - i + 2] = 3;
											dublajoc[li][coc - i + 3] = 3;
										}
									}
									i = 0;
									j = 0;
									ok3 = 1, ok4 = 1;
									while (b[li - i][coc] == 1)
									{
										if (dublajoc[li - i][coc] != 2)
										{
											ok3 = 0;
										}
										i++;
									}
									if (ok3 == 1)
									{
										while (b[li + j][coc] == 1)
										{
											if (dublajoc[li + j][coc] != 2)
											{
												ok4 = 0;
											}
											j++;
										}
									}
									i = i - 1;
									if (ok4 == 1 && ok3 == 1 && i + j + 1 > 2)
									{
										if (i + j == 3)
										{
											navajoc++;
											dublajoc[li - i][coc] = 3;
											dublajoc[li - i + 1][coc] = 3;
											dublajoc[li - i + 2][coc] = 3;
										}
										if (i + j == 4)
										{
											navajoc++;
											dublajoc[li - i][coc] = 3;
											dublajoc[li - i + 1][coc] = 3;
											dublajoc[li - i + 2][coc] = 3;
											dublajoc[li - i + 3][coc] = 3;
										}
									}
								}
								refresh();
								wrefresh(joc);
							}
							else
							{
								if (dublajoc[x][y] == 0 && k <= 10)
								{
									dublajoc[x][y] = 1;
									k++;
									mvwaddch(joc, 2 *(x - 1) + 1, 4 *(y - 1) + 2, '*');
									choice = 0;
								}
							}
						}

						refresh();
						wrefresh(joc);
						wrefresh(pc);
						break;
					case KEY_MOUSE:
						// CAZUL JOCULUI NORMAL CU MOUSE UL
						if (getmouse(&event) == OK)
						{
							if (choice == 0)
							{

								ok6 = 0;
								x = event.x;
								y = event.y;
								//DACA MOUSE UL A APASAT INTR-O CASUTA
								if ((y > row / 3) && (y < (row / 3 + 21)) && (x > ((col - 41) *6 / 7)) && (x < ((col - 41) *6 / 7 + 41)))
								{
									if ((event.bstate &BUTTON1_PRESSED) || (event.bstate &BUTTON1_CLICKED))
									{
										//DACA ACTIUNEA A FOST CLICK SAU CLICK APASAT
										wmouse_trafo(pc, &y, &x, FALSE);
										//SE ACTUALIZEASZA COORDONATELE MOUSE ULUI LA COORDONATELE FERESTREI

										li = (y / 2) + 1;
										coc = (x / 4) + 1;
										if (a[li][coc] == 1 && dublapc[li][coc] == 0)
										{
											//DACA ACESTA A NIMERIT O NAVA SE ACTUALIZEAZA COORDONATELE SI SE AFISEAZAZ "x"
											dublapc[li][coc] = 2;
											cp++;
											choice = 0;
											mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1) + 2, 'X');
											//SE VERIFICA CA SI IN MODUL DESTROY DACA NAVA A FOST DOBORATA - ACELASI ALGORITM
											if (a[li - 1][coc] != 1 && a[li + 1][coc] != 1 && a[li][coc - 1] != 1 && a[li][coc + 1] != 1)
											{
												navapc++;
												nave[k1] = create_newwin(3, 5, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
												k1++;
												ok6 = 1;
											}
											if ((a[li][coc + 1] == 1 && dublapc[li][coc + 1] == 2 && a[li][coc - 1] != 1 && a[li][coc + 2] != 1))
											{
												navapc++;
												nave[k2] = create_newwin(3, 9, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
												k2++;
												mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc) + 2, 'X');
												ok6 = 1;
											}
											if ((a[li][coc - 1] == 1 && dublapc[li][coc - 1] == 2 && a[li][coc - 2] != 1 && a[li][coc + 1] != 1))
											{
												navapc++;
												nave[k2] = create_newwin(3, 9, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 2));
												k2++;
												ok6 = 1;
												mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 2) + 2, 'X');
											}
											if ((a[li + 1][coc] == 1 && dublapc[li + 1][coc] == 2 && a[li + 2][coc] != 1 && a[li - 1][coc] != 1))
											{
												navapc++;
												nave[k2] = create_newwin(5, 5, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
												k2++;
												ok6 = 1;
												mvwaddch(pc, 2 *(li) + 1, 4 *(coc - 1) + 2, 'X');
											}
											if ((a[li - 1][coc] == 1 && dublapc[li - 1][coc] == 2 && a[li - 2][coc] != 1 && a[li + 1][coc] != 1))
											{
												navapc++;
												nave[k2] = create_newwin(5, 5, row / 3 + 2 *(li - 2), 6 *(col - 41) / 7 + 4 *(coc - 1));
												k2++;
												ok6 = 1;
												mvwaddch(pc, 2 *(li - 2) + 1, 4 *(coc - 1) + 2, 'X');
											}
											if (ok6 != 1)
											{
												i = 0;
												j = 0;
												ok3 = 1, ok4 = 1;
												while (a[li][coc - i] == 1)
												{
													if (dublapc[li][coc - i] != 2)
													{
														ok3 = 0;
													}
													i++;
												}
												if (ok3 == 1)
												{
													while (a[li][coc + j] == 1)
													{
														if (dublapc[li][coc + j] != 2)
														{
															ok4 = 0;
														}
														j++;
													}
												}
												i = i - 1;
												if (ok4 == 1 && ok3 == 1 && i + j > 2)
												{
													if (i + j == 3)
													{
														navapc++;
														nave[k3] = create_newwin(3, 13, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1 - i));
														k3++;
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1 - i) + 2, 'X');
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i) + 2, 'X');
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i + 1) + 2, 'X');
													}
													if (i + j == 4)
													{
														navapc++;
														nave[k4] = create_newwin(3, 17, row / 3 + 2 *(li - 1), 6 *(col - 41) / 7 + 4 *(coc - 1 - i));
														k4++;
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1 - i) + 2, 'X');
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i) + 2, 'X');
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i + 1) + 2, 'X');
														mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - i + 2) + 2, 'X');
													}
												}
												i = 0;
												j = 0;
												ok3 = 1, ok4 = 1;
												while (a[li - i][coc] == 1)
												{
													if (dublapc[li - i][coc] != 2)
													{
														ok3 = 0;
													}
													i++;
												}
												if (ok3 == 1)
												{
													while (a[li + j][coc] == 1)
													{
														if (dublapc[li + j][coc] != 2)
														{
															ok4 = 0;
														}
														j++;
													}
												}
												i = i - 1;
												if (ok4 == 1 && ok3 == 1 && i + j + 1 > 2)
												{
													if (i + j == 3)
													{
														navapc++;
													 										
														nave[k3] = create_newwin(7, 5, row / 3 + 2 *(li - i - 1), 6 *(col - 41) / 7 + 4 *(coc - 1));
														k3++;
														mvwaddch(pc, 2 *(li - 1 - i) + 1, 4 *(coc - 1) + 2, 'X');
														mvwaddch(pc, 2 *(li - i) + 1, 4 *(coc - 1) + 2, 'X');
														mvwaddch(pc, 2 *(li - i + 1) + 1, 4 *(coc - 1) + 2, 'X');
													}
													if (i + j == 4)
													{
													 		
														navapc++;									
														nave[k4] = create_newwin(9, 5, row / 3 + 2 *(li - 1 - i), 6 *(col - 41) / 7 + 4 *(coc - 1));
														k4++;
														mvwaddch(pc, 2 *(li - 1 - i) + 1, 4 *(coc - 1) + 2, 'X');
														mvwaddch(pc, 2 *(li - i) + 1, 4 *(coc - 1) + 2, 'X');
														mvwaddch(pc, 2 *(li - i + 1) + 1, 4 *(coc - 1) + 2, 'X');
														mvwaddch(pc, 2 *(li - i + 2) + 1, 4 *(coc - 1) + 2, 'X');
													}
												}
											}
											refresh();
											wrefresh(pc);
										}
										else
										{
											if (dublapc[li][coc] != 1 && dublapc[li][coc] != 2)
											{
												dublapc[li][coc] = 1;
												mvwaddch(pc, 2 *(li - 1) + 1, 4 *(coc - 1) + 2, '*');
												choice = 1;
												//DE FIECARE DATA CAND SE SCHIMBA RANDUL - SE ACTUALIZEAZA FEREASTRA DE RAND CU GALBEN-
												wbkgd(rp, COLOR_PAIR(9));
												wbkgd(rj, COLOR_PAIR(5));
												wrefresh(rp);
												wrefresh(rj);
											}
										}
										refresh();
										wrefresh(pc);
									}
								}
							}
							refresh();
							wrefresh(joc);
							wrefresh(pc);
						}
						while (choice == 1)
						{
							srand(time(NULL));
							//SE GASESC COORDONATE RANDOM
							x = rand() % 10 + 1;
							y = rand() % 10 + 1;
							while (dublajoc[x][y] == 1 || dublajoc[x][y] == 2 || dublajoc[x][y] == 3)
							{
								x = rand() % 10 + 1;
								y = rand() % 10 + 1;
							}
							sleep(3);
							if (b[x][y] == 1 && dublajoc[x][y] == 0)
							{
								//DACA NAVA A FOST NIMERITA SE ACTUALIZEAZA MATRICEA DE COORDONATE SI SE AFISEAZAZ "X"
								cj++;
								dublajoc[x][y] = 2;
								choice = 1;
								mvwaddch(joc, 2 *(x - 1) + 1, 4 *(y - 1) + 2, 'X');
								creare_glont(glont, b, contorjoc, x, y, row, col);
								wbkgd(glont[contorjoc], COLOR_PAIR(4));
								wrefresh(glont[contorjoc]);
								contorjoc++;
								//DACA NAVA A FOST DOBORATA NU SE SCHIMBA NIMIC IN EXTERIOR DAR SE INREGISTREAZA IN MATRICE DIFERIT - TOATA NAVA PRIMESTE CIFRA 3
								li = x;
								coc = y;
								if (b[li - 1][coc] != 1 && b[li + 1][coc] != 1 && b[li][coc - 1] != 1 && b[li][coc + 1] != 1)
								{
									navajoc++;
									dublajoc[li][coc] = 3;
								}
								if ((b[li][coc + 1] == 1 && dublajoc[li][coc + 1] == 2 && b[li][coc - 1] != 1 && b[li][coc + 2] != 1))
								{
									navajoc++;

									dublajoc[li][coc] = 3;
									dublajoc[li][coc + 1] = 3;
								}
								if ((b[li][coc - 1] == 1 && dublajoc[li][coc - 1] == 2 && b[li][coc - 2] != 1 && b[li][coc + 1] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li][coc - 1] = 3;
								}
								if ((b[li + 1][coc] == 1 && dublajoc[li + 1][coc] == 2 && b[li + 2][coc] != 1 && b[li - 1][coc] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li + 1][coc] = 3;
								}
								if ((b[li - 1][coc] == 1 && dublajoc[li - 1][coc] == 2 && b[li - 2][coc] != 1 && b[li + 1][coc] != 1))
								{
									navajoc++;
									dublajoc[li][coc] = 3;
									dublajoc[li - 1][coc] = 3;
								}
								if (ok6 != 1)
								{
									i = 0;
									j = 0;
									ok3 = 1, ok4 = 1;
									while (b[li][coc - i] == 1)
									{
										if (dublajoc[li][coc - i] != 2)
										{
											ok3 = 0;
										}
										i++;
									}
									if (ok3 == 1)
									{
										while (b[li][coc + j] == 1)
										{
											if (dublajoc[li][coc + j] != 2)
											{
												ok4 = 0;
											}
											j++;
										}
									}
									i = i - 1;
									if (ok4 == 1 && ok3 == 1 && i + j > 2)
									{
										if (i + j == 3)
										{
											navajoc++;
											dublajoc[li][col - i] = 3;
											dublajoc[li][col - i + 1] = 3;
											dublajoc[li][col - i + 2] = 3;
										}
										if (i + j == 4)
										{
											navajoc++;
											dublajoc[li][col - i] = 3;
											dublajoc[li][col - i + 1] = 3;
											dublajoc[li][col - i + 2] = 3;
											dublajoc[li][col - i + 3] = 3;
										}
									}
									i = 0;
									j = 0;
									ok3 = 1, ok4 = 1;
									while (b[li - i][coc] == 1)
									{
										if (dublajoc[li - i][coc] != 2)
										{
											ok3 = 0;
										}
										i++;
									}
									if (ok3 == 1)
									{
										while (b[li + j][coc] == 1)
										{
											if (dublajoc[li + j][coc] != 2)
											{
												ok4 = 0;
											}
											j++;
										}
									}
									i = i - 1;
									if (ok4 == 1 && ok3 == 1 && i + j + 1 > 2)
									{
										if (i + j == 3)
										{
											navajoc++;
											dublajoc[li - i][col] = 3;
											dublajoc[li - i + 1][col] = 3;
											dublajoc[li - i + 2][col] = 3;
										}
										if (i + j == 4)
										{
											navajoc++;
											dublajoc[li - i][col] = 3;
											dublajoc[li - i + 1][col] = 3;
											dublajoc[li - i + 2][col] = 3;
											dublajoc[li - i + 3][col] = 3;
										}
									}
								}
								wrefresh(joc);
								refresh();
							}
							else
							{
								if (dublajoc[x][y] == 0)
								{
									dublajoc[x][y] = 1;
									mvwaddch(joc, 2 *(x - 1) + 1, 4 *(y - 1) + 2, '*');
									choice = 0;
									//SE SCHIMBA FEREASTRA DE RAND
									wbkgd(rj, COLOR_PAIR(9));
									wbkgd(rp, COLOR_PAIR(9));
									wbkgd(rp, COLOR_PAIR(5));
									wrefresh(rp);
									wrefresh(rj);
								}
							}
						}

						refresh();
						wrefresh(joc);
						wrefresh(pc);
						break;			
				}
				//SE VERIFICA DACA JOCUL S-A TERMINAT!!
				//AICI A CASTIGAT CALCULATORUL SI AM FACUT BONUS O FEREASTRA NOUA PRIN CARE ARAT ACEST LUCRU
				if (cj == 20 && cp != 20)
				{
					move(0, 0);
					clrtobot();
					getmaxyx(stdscr, row, col);
					char str[80] = "BATTLESHIP";
					mvprintw(4, (col - strlen(str)) / 2, str);
					char str2[50] = "CALCULATORUL A CASTIGAT!";
					char str8[20] = "SCOR:      ";
                                        mvwprintw(stdscr, row / 2 - 5, (col - strlen(str8)) / 2, str8);
                                        mvwprintw(stdscr, row / 2 - 3, (col - 22) / 2, "%d - %d TINTE NIMERITE", cp, cj);
                                        wrefresh(stdscr);
                                        refresh();
					refresh();
					mvprintw(row / 2, (col - strlen(str2)) / 2, str2);
					mvwprintw(stdscr, row / 2 - 10, (col - strlen(str8)) / 2, str8);
					mvwprintw(stdscr, row / 2 - 8, (col - 30) / 2, "%d - %d NAVE NIMERITE - PC-JOC", navapc, navajoc);
					refresh();
				}
				//AICI A CASTIGAT JUCATORUL SI EXISTA O IMAGINE NOUA PENTRU A CELEBRA ACEST LUCRU - IMPREUNA CU SCORUL JOCULUI
				if (cj != 20 && cp == 20)
				{
					move(0, 0);
					clrtobot();
					getmaxyx(stdscr, row, col);
					init_pair(10, COLOR_BLACK, COLOR_BLACK);
					refresh();
					char str[80] = "BATTLESHIP";
					mvwprintw(stdscr, 4, (col - strlen(str)) / 2, str);
					char str2[80] = "&^%$#@!    ATI CASTIGAT!      *&^%$#@!";
					refresh();
					mvwprintw(stdscr, row / 2, (col - strlen(str2)) / 2, str2);
					char str8[20] = "SCOR:      ";
					mvwprintw(stdscr, row / 2 - 5, (col - strlen(str8)) / 2, str8);
					mvwprintw(stdscr, row / 2 - 3, (col - 22) / 2, "%d - %d TINTE NIMERITE", cp, cj);
					mvwprintw(stdscr, row / 2 - 10, (col - strlen(str8)) / 2, str8);
                                        mvwprintw(stdscr, row / 2 - 8, (col - 30) / 2, "%d - %d NAVE NIMERITE - PC-JOC", navapc, navajoc);

					wrefresh(stdscr);
					refresh();
				}
				//IN CAZ DE EGALITATE
				/*
				if (cj == 20 && cp == 20)
				{
					move(0, 0);
					clrtobot();
					getmaxyx(stdscr, row, col);
					char str[80] = "EGALITATE :(";
					mvprintw(4, (col - strlen(str)) / 2, str);
					refresh();
				}*/
				//DIN ACEST MOMENT NE PUTEM INTOARCE LA MENIU APSAND Q - SI APOI QUIT.
			} while ((ch = wgetch(stdscr)) != 113);
		}
	}

	endwin();
	return 0;
}

//FUNCTIE CARE CREAZAZ O NOUA FEREASTRA
WINDOW* create_newwin(int height, int width, int starty, int startx)
{
	WINDOW * local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0);
	wrefresh(local_win);

	/*Arata boxa          */
	return local_win;
}

//FUNCTIE CARE STERGE O FEREASTRA
void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}

//FUNCTIE CARE AFISEAZA CAROURILE -  TABLA DE JOC
void patrate(WINDOW *joc, WINDOW *pc)
{
	int i;
	int linie, coloana, io, j;
	getmaxyx(joc, io, coloana);

	for (i = 1; i <= 9; i++)
	{
		mvwaddch(joc, i *2, 0, ACS_LTEE);
		mvwhline(joc, i *2, 1, ACS_HLINE, coloana - 1);
		mvwaddch(joc, i *2, coloana - 1, ACS_RTEE);
		wrefresh(joc);
	}
	for (j = 1; j <= 9; j++)
	{
		mvwvline(joc, 1, j *4, ACS_VLINE, io - 2);
		wrefresh(joc);
	}
	for (i = 1; i <= 9; i++)
	{
		mvwaddch(pc, i *2, 0, ACS_LTEE);
		mvwhline(pc, i *2, 1, ACS_HLINE, coloana - 1);
		mvwaddch(pc, i *2, coloana - 1, ACS_RTEE);
		wrefresh(pc);
	}
	for (j = 1; j <= 9; j++)
	{
		mvwvline(pc, 1, j *4, ACS_VLINE, io - 2);
		wrefresh(pc);
	}
}

//FUNCTIE MAI COMPLEXA CARE CREAAZA MATRICEA PC - SI ARANJEAZA NAVELE
void init_wins(int a[][12], WINDOW *nave[], WINDOW *joc, WINDOW *pc, int m[][11], int row, int col, int c[][12])
{
	int linie, coloana;
	linie = row / 3;
	coloana = (col - 41) / 7;
	//AFISEAZA NAVELE PENTRU TABLA DE JOC A JUCATORULUI
	// ALGORITMUL A MAI FOST FOLOSIT IN TRECUT
	//AFLA POZITIILE SARACIILOR
	int i, j;
	int poz;	//0 - orizontal 1-vertical
	int dir;	//0 - dreapta 1-stanga || 0 - sus 1 - jos
	int cat;	// dimensiunea
	int steag;
	int v[5] = { 0 };
	int k1, k2, k3, k4;
	int j2, i2;
	k1 = 0;
	k2 = 4;
	k3 = 7;
	k4 = 9;
	for (i = 1; i <= 10; i++)
	{
		for (j = 1; j <= 10; j++)
		{
			steag = 0;
			if (m[i][j] == 1)
			{
				if (m[i][j + 1] == 1)
				{
					poz = 0;
				}
				else
				{
					if (m[i + 1][j] == 1)
					{
						poz = 1;
					}
					else
					{
						//AM GASIT UN PATRAT DE 1
						v[1]++;
						steag = 1;
						nave[k1] = create_newwin(3, 5, linie + 2 *(i - 1), coloana + 4 *(j - 1));
						k1++;
					}
				}
			}
			if (steag != 1)
			{
				if (poz == 0)
				{
					cat = 0;
					j2 = j;
					while (m[i][j] == 1)
					{
						j++;
						cat++;
					}
					switch (cat)
					{
						case 2:
							nave[k2] = create_newwin(3, 9, linie + 2 *(i - 1), coloana + 4 *(j2 - 1));
							k2++;
							break;
						case 3:
							nave[k3] = create_newwin(3, 13, linie + 2 *(i - 1), coloana + 4 *(j2 - 1));
							k3++;
							break;
						case 4:
							nave[k4] = create_newwin(3, 17, linie + 2 *(i - 1), coloana + 4 *(j2 - 1));
							k4++;
							break;
					}
					v[cat]++;
				}
				if (poz == 1)
				{
					cat = 0;
					i2 = i;
					while (m[i][j] == 1)
					{
						m[i][j] = 0;
						i++;
						cat++;
					}
					switch (cat)
					{
						case 2:
							nave[k2] = create_newwin(5, 5, linie + 2 *(i2 - 1), coloana + 4 *(j - 1));
							k2++;
							break;
						case 3:
							nave[k3] = create_newwin(7, 5, linie + 2 *(i2 - 1), coloana + 4 *(j - 1));
							k3++;
							break;
						case 4:
							nave[k4] = create_newwin(9, 5, linie + 2 *(i2 - 1), coloana + 4 *(j - 1));
							k4++;
							break;
					}
					i = i - cat;
					v[cat]++;
				}
			}
		}
	}


	cmatrice(a, c, row, col, nave);

	//Un mic refresh
	wrefresh(nave[0]);
	wrefresh(nave[1]);
	wrefresh(nave[2]);
	wrefresh(nave[3]);
	wrefresh(nave[4]);
	wrefresh(nave[5]);
	wrefresh(nave[6]);
	wrefresh(nave[7]);
	wrefresh(nave[8]);
	wrefresh(nave[9]);
	wrefresh(joc);
	wrefresh(pc);
	refresh();
}

//CREAZA O COPIE
//PENTRU CA ATUNCI CAND SE INTERPRETEAZAZ MATRICEA - ALGORITMUL DE MAI SUS STERGE NAVELE ASEZATE VERTICAL
void matrice(int m[][11], int b[][11])
{
	int i, j;
	for (i = 1; i <= 10; i++)
	{
		for (j = 1; j <= 10; j++)
		{
			m[i][j] = b[i][j];
		}
	}
}

//ACEASTA FUNCTIE CREAZA O MATRICE RANDOM CU NAVELE CALCULATORULUI - ACEASTA VA FI INTERPRETATA SI APOI NAVELE VOR FI AFISATE PE ECRAN ATUNCI CAND ELE SUNT DOBORATE
void cmatrice(int a[][12], int c[][12], int row, int col, WINDOW *nave[])
{
	int ok = 0;
	int directie;
	int linie, coloana;
	int i, j;
	//SE REINITIALIZEAZA CU 0 IN CAZ CA MERGEM IN MENIU SI DAM NEW GAME SI NAVELE CALCULATORULUI SA SE REAMESTECE
	for (i = 0; i <= 10; i++)
	{
		for (j = 0; j <= 10; j++)
		{
			c[i][j] = 0;
		}
	}

	srand(time(NULL));
	
	linie = rand() % 1000;
	linie = linie % 13 * 4 *3 / 2 + rand() % 10;
	linie = linie % 9 + 1;

	coloana = rand() % 10 * 4;
	coloana = coloana % 9 + 1;

	directie = rand() *6;	// 0 - orizontal, 1 - vertical
	directie = directie* rand() *4 / 3;
	directie = rand() % 2;
	// SE GASESTE O DIRECTIE SI COORDONATE CARE SA CORESPUNDA CERINTELOR - SA SE INCADREZE IN CADRAN SI SA NU LASE O NAVA SA FIE LIPITA DE ALTA
	// PENTRU A REUSI ACEST LUCRU IN JURUL FIECARE NAVE ASEZATE DEJA IN MATRICE FORMAM UN CONTUR DE CIFRA 2 - PE CARE NU AVEM VOIE SA PUNEM O NAVA
	// SE GASESTE NAVA DE 4
	if (directie == 0)
	{
		while (coloana + 4 > 10)
		{
			coloana = rand() % 70 * 3;
			coloana = coloana % 9 + 1;
		}
		if (coloana + 4 <= 10)
		{
			c[linie][coloana] = 1;
			c[linie][coloana - 1] = 2;
			c[linie + 1][coloana] = 2;
			c[linie - 1][coloana] = 2;
			c[linie - 1][coloana - 1] = 2;
			c[linie + 1][coloana - 1] = 2;

			c[linie][coloana + 1] = 1;
			c[linie - 1][coloana + 1] = 2;
			c[linie + 1][coloana + 1] = 2;

			c[linie][coloana + 2] = 1;
			c[linie - 1][coloana + 2] = 2;
			c[linie + 1][coloana + 2] = 2;

			c[linie - 1][coloana + 3] = 2;
			c[linie + 1][coloana + 3] = 2;
			c[linie][coloana + 3] = 1;
			c[linie][coloana + 4] = 2;
			c[linie - 1][coloana + 4] = 2;
			c[linie + 1][coloana + 4] = 2;
		}
	}
	else
	{
		while (linie + 4 > 10)
		{
			linie = rand() % 70 * 3;
			linie = linie % 9 + 1;
		}
		if (linie + 4 <= 10)
		{
			c[linie][coloana] = 1;
			c[linie][coloana - 1] = 2;
			c[linie - 1][coloana - 1] = 2;
			c[linie - 1][coloana] = 2;
			c[linie - 1][coloana + 1] = 2;
			c[linie][coloana + 1] = 2;

			c[linie + 1][coloana] = 1;
			c[linie + 1][coloana - 1] = 2;
			c[linie + 1][coloana + 1] = 2;

			c[linie + 2][coloana] = 1;
			c[linie + 2][coloana - 1] = 2;
			c[linie + 2][coloana + 1] = 2;

			c[linie + 4][coloana] = 2;
			c[linie + 4][coloana + 1] = 2;
			c[linie + 3][coloana] = 1;
			c[linie + 3][coloana - 1] = 2;
			c[linie + 3][coloana + 1] = 2;
			c[linie + 4][coloana - 1] = 2;
		}
	}

	srand(time(NULL));
	int yyup = 1;
	// SE GASESC NAVELE DE 3
	for (i = 1; i <= 2; i++)
	{
		if (yyup == 1)
		{
			directie = rand() *55 * 40 % 2;
			yyup = 0;
		}
		if (yyup == 0)
		{
			directie = rand() *5 % 2;
		}
		if (directie == 0)
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while (coloana + 3 > 10 || (c[linie][coloana] == 1 || c[linie][coloana] == 2) || (c[linie][coloana + 1] == 1 || c[linie][coloana + 1] == 2) || (c[linie][coloana + 2] == 1 || c[linie][coloana + 2] == 2))
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}
		else
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while (linie + 3 > 10 || (c[linie][coloana] == 1 || c[linie][coloana] == 2) || (c[linie + 1][coloana] == 1 || c[linie + 1][coloana + 1] == 2) || (c[linie + 2][coloana + 2] == 1 || c[linie + 2][coloana + 2] == 2))
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}

		if (directie == 0)
		{
			if (coloana + 3 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie + 1][coloana - 1] = 2;

				c[linie][coloana + 1] = 1;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana + 1] = 2;

				c[linie - 1][coloana + 2] = 2;
				c[linie + 1][coloana + 2] = 2;
				c[linie][coloana + 2] = 1;
				c[linie][coloana + 3] = 2;
				c[linie - 1][coloana + 3] = 2;
				c[linie + 1][coloana + 3] = 2;
			}
		}
		else
		{
			if (linie + 3 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 2;

				c[linie + 1][coloana] = 1;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana + 1] = 2;

				c[linie + 3][coloana] = 2;
				c[linie + 3][coloana + 1] = 2;
				c[linie + 2][coloana] = 1;
				c[linie + 2][coloana - 1] = 2;
				c[linie + 2][coloana + 1] = 2;
				c[linie + 3][coloana - 1] = 2;
			}
		}
	}

	srand(time(NULL));
	//SE GASESC NAVELE DE 2
	for (i = 1; i <= 3; i++)
	{
		directie = rand() *123 * 20 % 2;
		directie = rand() / 3* rand() / 5* rand() / 6* rand() / 7 % 2;
		if (directie == 0)
		{

			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while (c[linie][coloana] == 1 || c[linie][coloana] == 2 || c[linie][coloana + 1] == 1 || c[linie][coloana + 1] == 2 || coloana + 2 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}
		else
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while ((c[linie][coloana] == 1 || c[linie][coloana] == 2) || (c[linie + 1][coloana] == 1 || c[linie + 1][coloana + 1] == 2) || linie + 2 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}

		if (directie == 0)
		{
			if (coloana + 2 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie + 1][coloana - 1] = 2;

				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 1;
				c[linie][coloana + 2] = 2;
				c[linie - 1][coloana + 2] = 2;
				c[linie + 1][coloana + 2] = 2;
			}
		}
		else
		{
			if (linie + 2 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 2;

				c[linie + 2][coloana] = 2;
				c[linie + 2][coloana + 1] = 2;
				c[linie + 1][coloana] = 1;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana + 1] = 2;
				c[linie + 2][coloana - 1] = 2;
			}
		}
	}
	srand(time(NULL));
	//SE GASESC NAVELE DE 1
	for (i = 1; i <= 4; i++)
	{
		directie = rand() *33 * 65 % 2;
		directie = rand() *123 % 67 % 2;
		if (directie == 0)
		{

			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while ((c[linie][coloana] == 1 || c[linie][coloana] == 2) || coloana + 1 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}
		else
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while ((c[linie][coloana] == 1 || c[linie][coloana] == 2) || linie + 1 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}

		if (directie == 0)
		{
			if (coloana + 1 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie][coloana + 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie + 1][coloana + 1] = 2;
			}
		}
		else
		{
			if (linie + 1 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie][coloana + 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie + 1][coloana + 1] = 2;
			}
		}
	}
	matrice2(a, c);
}

// ACEASTA FUNCTIE FACE CA ATUNCI CAND O NAVA ESTE NIMERITA IN FEREASTRA joc ACEASTA SA DEVINA GALBENA
void creare_glont(WINDOW *glont[], int b[][11], int contorjoc, int li, int coc, int row, int col)
{
	glont[contorjoc] = create_newwin(3, 5, row / 3 + 2 *(li - 1), (col - 41) / 7 + 4 *(coc - 1));
	if ((b[li][coc + 1] == 1) && (b[li][coc - 1] == 1))
	{
		mvwhline(glont[contorjoc], 0, 0, ACS_HLINE, 5);
		mvwhline(glont[contorjoc], 2, 0, ACS_HLINE, 5);
	}
	else
	{
		if ((b[li][coc + 1] == 1))
		{
			mvwhline(glont[contorjoc], 0, 1, ACS_HLINE, 4);
			mvwhline(glont[contorjoc], 2, 1, ACS_HLINE, 4);
		}
		if (b[li][coc - 1] == 1)
		{
			mvwhline(glont[contorjoc], 0, 0, ACS_HLINE, 4);
			mvwhline(glont[contorjoc], 2, 0, ACS_HLINE, 4);
		}
	}
	if ((b[li - 1][coc] == 1) && (b[li + 1][coc] == 1))
	{
		mvwvline(glont[contorjoc], 0, 0, ACS_VLINE, 3);
		mvwvline(glont[contorjoc], 0, 4, ACS_VLINE, 3);
	}
	else
	{
		if ((b[li - 1][coc] == 1))
		{
			mvwvline(glont[contorjoc], 0, 0, ACS_VLINE, 2);
			mvwvline(glont[contorjoc], 0, 4, ACS_VLINE, 2);
		}
		if (b[li + 1][coc] == 1)
		{
			mvwvline(glont[contorjoc], 1, 0, ACS_VLINE, 2);
			mvwvline(glont[contorjoc], 1, 4, ACS_VLINE, 2);
		}
	}
}

//ACEASTA FUNCTIE FACE O COPIE - PENTRU ACEL ALGORITM CARE STERGE NAVELE DE PE VERTICALA DIN MATRICE
void matrice2(int a[][12], int c[][12])
{
	int i, j;
	for (i = 1; i <= 10; i++)
	{
		for (j = 1; j <= 10; j++)
		{
			a[i][j] = c[i][j];
		}
	}
}

//ACEASTA FUNCTIE FACE PARTE DIN GRUPUL -MATRICE1,MATRICE2
void matrice3(int dublajoc1[][11], int dublajoc[][11])
{
	int i, j;
	for (i = 1; i <= 10; i++)
	{
		for (j = 1; j <= 10; j++)
		{
			dublajoc1[i][j] = dublajoc[i][j];
		}
	}
}

//ACESTA FUNCTIE ESTE MENITA SA GASEASCA O MATRICE RANDOM- ADICA SA AMESTECE NAVELE ( IN MODUL RANDOM) - DOAR CELE CARE NU AU FOST DOBORATE DEJA
//FOLOSESTE ACELASI ALGORITM CA FUNCITA CMATRICE
void c2matrice(int a[][11], int c[][11], int row, int col, WINDOW *nave[], int v[5])
{
	int ok = 0;
	int directie;
	int linie, coloana;
	int i, j;

	srand(time(NULL));

	linie = rand() % 1000;
	linie = linie % 13 * 4 *3 / 2 + rand() % 10;
	linie = linie % 9 + 1;

	coloana = rand() % 10 * 4;
	coloana = coloana % 9 + 1;

	directie = rand() *6;	// 0 - orizontal, 1 - vertical
	directie = directie* rand() *4 / 3;
	directie = rand() % 2;  
	if (v[4] == 0)
	{
		if (directie == 0)
		{
			while (coloana + 4 > 10)
			{
				coloana = rand() % 70 * 3;
				coloana = coloana % 9 + 1;
			}
			if (coloana + 4 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie + 1][coloana - 1] = 2;

				c[linie][coloana + 1] = 1;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana + 1] = 2;

				c[linie][coloana + 2] = 1;
				c[linie - 1][coloana + 2] = 2;
				c[linie + 1][coloana + 2] = 2;

				c[linie - 1][coloana + 3] = 2;
				c[linie + 1][coloana + 3] = 2;
				c[linie][coloana + 3] = 1;
				c[linie][coloana + 4] = 2;
				c[linie - 1][coloana + 4] = 2;
				c[linie + 1][coloana + 4] = 2;
			}
		}
		else
		{
			while (linie + 4 > 10)
			{
				linie = rand() % 70 * 3;
				linie = linie % 9 + 1;
			}
			if (linie + 4 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 2;

				c[linie + 1][coloana] = 1;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana + 1] = 2;

				c[linie + 2][coloana] = 1;
				c[linie + 2][coloana - 1] = 2;
				c[linie + 2][coloana + 1] = 2;

				c[linie + 4][coloana] = 2;
				c[linie + 4][coloana + 1] = 2;
				c[linie + 3][coloana] = 1;
				c[linie + 3][coloana - 1] = 2;
				c[linie + 3][coloana + 1] = 2;
				c[linie + 4][coloana - 1] = 2;
			}
		}
	}
	srand(time(NULL));
	int yyup = 1;

	for (i = 1; i <= 2 - v[3]; i++)
	{
		if (yyup == 1)
		{
			directie = rand() *55 * 40 % 2;
			yyup = 0;
		}
		if (yyup == 0)
		{
			directie = rand() *5 % 2;
		}
		if (directie == 0)
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while (coloana + 3 > 10 || (c[linie][coloana] == 1 || c[linie][coloana] == 2) || (c[linie][coloana + 1] == 1 || c[linie][coloana + 1] == 2) || (c[linie][coloana + 2] == 1 || c[linie][coloana + 2] == 2))
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}
		else
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while (linie + 3 > 10 || (c[linie][coloana] == 1 || c[linie][coloana] == 2) || (c[linie + 1][coloana] == 1 || c[linie + 1][coloana + 1] == 2) || (c[linie + 2][coloana + 2] == 1 || c[linie + 2][coloana + 2] == 2))
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}

		if (directie == 0)
		{
			if (coloana + 3 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie + 1][coloana - 1] = 2;

				c[linie][coloana + 1] = 1;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana + 1] = 2;

				c[linie - 1][coloana + 2] = 2;
				c[linie + 1][coloana + 2] = 2;
				c[linie][coloana + 2] = 1;
				c[linie][coloana + 3] = 2;
				c[linie - 1][coloana + 3] = 2;
				c[linie + 1][coloana + 3] = 2;
			}
		}
		else
		{
			if (linie + 3 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 2;

				c[linie + 1][coloana] = 1;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana + 1] = 2;

				c[linie + 3][coloana] = 2;
				c[linie + 3][coloana + 1] = 2;
				c[linie + 2][coloana] = 1;
				c[linie + 2][coloana - 1] = 2;
				c[linie + 2][coloana + 1] = 2;
				c[linie + 3][coloana - 1] = 2;
			}
		}
	}

	srand(time(NULL));
	for (i = 1; i <= 3 - v[2]; i++)
	{
		directie = rand() % 2;
		directie = rand() / 3* rand() / 5* rand() / 6* rand() / 7 % 2;
		if (directie == 0)
		{

			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while (c[linie][coloana] == 1 || c[linie][coloana] == 2 || c[linie][coloana + 1] == 1 || c[linie][coloana + 1] == 2 || coloana + 2 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}
		else
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while ((c[linie][coloana] == 1 || c[linie][coloana] == 2) || (c[linie + 1][coloana] == 1 || c[linie + 1][coloana + 1] == 2) || linie + 2 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}

		if (directie == 0)
		{
			if (coloana + 2 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie + 1][coloana - 1] = 2;

				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 1;
				c[linie][coloana + 2] = 2;
				c[linie - 1][coloana + 2] = 2;
				c[linie + 1][coloana + 2] = 2;
			}
		}
		else
		{
			if (linie + 2 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie][coloana + 1] = 2;

				c[linie + 2][coloana] = 2;
				c[linie + 2][coloana + 1] = 2;
				c[linie + 1][coloana] = 1;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana + 1] = 2;
				c[linie + 2][coloana - 1] = 2;
			}
		}
	}
	srand(time(NULL));
	for (i = 1; i <= 4 - v[1]; i++)
	{
		directie = rand() *33 * 65 % 2;
		directie = rand() *123 % 67 % 2;
		if (directie == 0)
		{

			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while ((c[linie][coloana] == 1 || c[linie][coloana] == 2) || coloana + 1 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}
		else
		{
			linie = rand() % 1000;
			linie = linie % 13 * 4 *3 / 2 + rand() % 10;
			linie = linie % 9 + 1;

			coloana = rand() % 10 * 4;
			coloana = coloana % 9 + 1;

			while ((c[linie][coloana] == 1 || c[linie][coloana] == 2) || linie + 1 > 10)
			{
				linie = rand() % 1000;
				linie = linie % 13 * 4 *3 / 2 + rand() % 10;
				linie = linie % 9 + 1;

				coloana = rand() % 10 * 4;
				coloana = coloana % 9 + 1;
			}
		}

		if (directie == 0)
		{
			if (coloana + 1 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie][coloana + 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie + 1][coloana + 1] = 2;
			}
		}
		else
		{
			if (linie + 1 <= 10)
			{
				c[linie][coloana] = 1;
				c[linie][coloana - 1] = 2;
				c[linie][coloana + 1] = 2;
				c[linie - 1][coloana - 1] = 2;
				c[linie - 1][coloana] = 2;
				c[linie - 1][coloana + 1] = 2;
				c[linie + 1][coloana - 1] = 2;
				c[linie + 1][coloana] = 2;
				c[linie + 1][coloana + 1] = 2;
			}
		}
	}
}

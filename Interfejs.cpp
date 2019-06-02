#include "Interfejs.h"

using namespace std;

void UI::wyborMetodyGenerowaniaGrafu() {

    while (naPoczatek) {
        cout << "Menu generowania grafu:" << endl << "1. Wygeneruj losowy graf programowo" << endl
             << "2. Wygeneruj graf z pliku" << endl;
        cin >> wybor;
        switch (wybor) {
            case 1: {
                grafLosowy();
                wyborFunkcji();
            }
                break;
            case 2: {
                grafZPliku();
                wyborFunkcji();
            }
                break;
        }
    }
}

void UI::wyborFunkcji() {
    while (!naPoczatek) {

        cout << endl << "Menu wyboru algorytmu:" << endl
             << "1. Wyswietl macierz incydencji i reprezentacja listowa" << endl
             << "2. Algorytm Kruskala " << endl
             << "3. Algorytm Dijkstry" << endl
             << "4. Algorytm Prima" << endl
             << "5. Nowy graf" << endl;
        cin >> wybor;
        switch (wybor) {
            case 1: {
                graf->wyswietl();
                cout << endl;
            }
                break;
            case 2: {
                

                while (true) {
                    int wyborReprezentacji;
                    cout << "Jakiej reprezentacji uzyc w algorytmie?" << endl << "1. Macierz Incydecji" << endl
                         << "2. Lista poprzednikow i nastepnikoww" << endl;
                    cin >> wyborReprezentacji;
                    if (wyborReprezentacji == 1) {
                       graf->KruskalMacierz();
                      
                        break;
                    } else if (wyborReprezentacji == 2) {
                       
                        graf->KruskalLista();
						break;
                    }
                }
            }
                break;

            case 3: {
                while (true) {
                    cout << "Zdefiniuj wierzcholek poczatkowy: ";
                    cin >> b;
                    if (b < graf->wierzcholki) break;
                    else cout << "Podaj prawidlowy numer wierzcholka!" << endl;
                }

                		graf->wyswietl();
                		cout << endl;
                        graf->Dijkstra(b);
                      
                
            }
                break;


            case 4: {
                while (true) {
                    int wyborReprezentacji;
                    cout << "Wybierz sposob przedstawienia: " << endl << "1. Macierz Incydecji" << endl
                         << "2. Lista poprzednikow i nastepnikow" << endl;
                    cin >> wyborReprezentacji;
                    if (wyborReprezentacji == 1) {
                        graf->macierz_Prim();
                        
                        break;
                    } else if (wyborReprezentacji == 2) {
                        graf->lista_Prim();
                        break;
                    }
                }

            }
                break;

            case 5: {
                delete graf;
                naPoczatek = true;
            }
                break;
        }
    }
}

void UI::grafLosowy() {
    naPoczatek = false;
    while (true) {
        cout << "Zdefiniuj ilosc wierzcholkow grafu: ";
        cin >> w;
        if (w > 1) break;
        else cout << "Liczba wierzcholkow nie moze byc mniejsza lub rowna 1." << endl;
    }


    int maxK = w * (w - 1);
    double minG = ceil((((double) w - 1) * 100) / (double) maxK);

    while (true) {

        cout << "Zdefiniuj gestosc grafu (minimum " << minG << "%): ";
        cin >> g;
        if (g < minG || g > 100)
            cout << "Podana gestosc nie pozwala na stworzenie grafu." << endl;
        else
            break;
    }
    double krawedzie = ceil((maxK * g) / 100);
    graf = new Graf(w, krawedzie);
    graf->losujGraf();
}

void UI::grafZPliku() {
    naPoczatek = false;
    ElementListy *e1;
    string s, nazwa, linia;
    fstream  plik;
    int a = 0;
    int krawedzie, wierzcholki;
    cout << "Nazwa pliku (bez rozszerzenia!): ";
    cin >> nazwa;
    nazwa = nazwa + ".txt";
    plik.open( nazwa.c_str() );
    if (!plik) {
        cout << "Nie udalo sie otworzyc pliku, sprobuj ponownie." << endl;
        naPoczatek = true;

    } else {

        {	
            plik >> krawedzie >> wierzcholki;
            if (!plik || krawedzie < wierzcholki - 1 || wierzcholki <= 1 ||
                krawedzie > (wierzcholki * (wierzcholki - 1))) {
                cout << "Plik jest uszkodzony, lub dane w nim sa w zlym formacie." << endl;

                naPoczatek = true;
            } else {
                graf = new Graf(wierzcholki, krawedzie);
                while (!plik.eof()&&a<krawedzie) {
                	
					
				
                    plik >> graf->K[a].wp >> graf->K[a].wk >> graf->K[a].waga;
                    		
                    if (graf->K[a].wp >= wierzcholki || graf->K[a].wk >= wierzcholki ||
                        graf->K[a].waga < 1) {
                     
                        cout << "Krawedzie w pliku sa nieprawidlowo okreslone." << endl;
                        naPoczatek = true;
                        break;
                    }

                    	if (plik) {
                        a++;
                    } else {
                        cout << "Krawedzie w pliku sa nieprawidlowo okreslone." << endl;
                        naPoczatek = true;
                        break;
                    }	
                }
                if (a == krawedzie) {
                    for (int i = 0; i < wierzcholki; i++)
                        graf->macierzIncydencji[i] = new int[krawedzie];
                    for (int i = 0; i < wierzcholki; i++)
                        for (int j = 0; j < krawedzie; j++) {
                            graf->macierzIncydencji[i][j] = 0;
                        }

                    for (int i = 0; i < wierzcholki; i++)
                        graf->listySasiedztwa[i] = NULL;

                    for (int i = 0; i < krawedzie; i++) {
                        int wp = graf->K[i].wp;
                        int wk = graf->K[i].wk;
                        e1 = new ElementListy;
                        e1->w = wk;
                        e1->waga = graf->K[i].waga;
                        e1->nastepny = graf->listySasiedztwa[wp];
                        graf->listySasiedztwa[wp] = e1;
                        graf->macierzIncydencji[wp][i] = 1;
                        graf->macierzIncydencji[wk][i] = -1;
                    }
                    plik.close();
                    graf->zamienGrafNaNieskierowany();
                    if (!(graf->sprawdzSpojnosc())) {
                        cout << "Zawarty w pliku graf jest niespojny, nie moznna wykonaac algorytmow!" << endl;
                        naPoczatek = true;
                        delete graf;
                    } else {
                        wyborFunkcji();
                    }

                } else if (!naPoczatek) {
                    naPoczatek = true;
                    cout << "W pliku brakuje danych dotyczacych krawedzi grafu." << endl;
                }
            
			
		}
        }
    }
    	plik.close();
}

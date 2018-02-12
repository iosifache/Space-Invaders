# Space invaders
Acest proiect a fost dezvoltat cu ajutorul limbajului de programare C, folosindu-se urmatoarele librarii:
* **stdio.h**, **stdlib.h**, **conio.h**: functii standard, de IO si random
* **math.h**, **time.h**, **limits.h**: functii matematice, de determinare a limitelor unui tip de variabile si de lucru cu timpul
* **Windows.h**, **WinBase.h**, **mmsystem.h**: functii de exploatare a consolei din Windows si de redare a sunetelor

S-au folosit comentarii si inainte de a defini functiile pentru a preciza utilizarea acestora in program.

Codul C a fost structurat folosind ca delimitatoare comentarii, dupa cum urmeaza:
* includere de librarii
* declararea de constante pentru codul caracterelor si al culorilor
* declararea de variabile
* declararea de functii
* functia **MAIN**
* definirea de functii

Functionalitatile suplimentare, pe langa cele din cerinta jocului, sunt:
* afisarea unui **ecran de start** la intrarea in joc cu optiuni de ajutor si istoric
* implementare de **sunete** specifice unor actiuni
* limitarea **munitiei** jucatorului pentru a creste dificultatea
* generarea unor **fisiere de logging** pentru a ajuta la urmarirea comportamentului de joc a utilizatorilor
* controlarea **ratei de refresh**
* afisarea in timp real a **parametrilor jocului**

Utilizatorului ii sunt puse la dispozitie tastele urmatoare pentru a controla jocul:
* **a**: deplasare spre stanga
* **d**: deplasare spre dreapta
* **space**: folosire munitie
* **p**: pauza
* **e**: iesire din joc
* **n**: micsorare rata de refresh
* **m**: marire rata de refresh
* **k**: micsorare viteza a blocului de invaders
* **l**: marire viteza a blocului de invaders